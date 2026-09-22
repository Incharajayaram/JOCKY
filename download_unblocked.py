import json
import requests
import os
import sys
import subprocess
import shutil
import hashlib
import time

# MalwareBazaar API Key - Get yours from https://bazaar.abuse.ch/
API_KEY = 'YOUR_API_KEY_HERE'
HEADERS = {'Auth-Key': API_KEY}
MB_URL = 'https://mb-api.abuse.ch/api/v1/'

REPOS = [
    "https://github.com/CaledoniaProject/drivers-binaries.git",
    "https://github.com/Sh3lldon/Vulnerable-WinKern-Drivers.git",
    "https://github.com/JodisKripe/BYOVD.git",
    "https://github.com/KOSEC-LLC/BYOVD-Research.git"
]

def get_sha256(filepath):
    h = hashlib.sha256()
    try:
        with open(filepath, 'rb') as f:
            while chunk := f.read(8192):
                h.update(chunk)
        return h.hexdigest().lower()
    except Exception:
        return None

def main():
    if not os.path.exists('loldrivers.json'):
        print("Please ensure loldrivers.json is in the same directory.")
        sys.exit(1)
        
    with open('loldrivers.json', 'r') as f:
        loldrivers = json.load(f)

    if not os.path.exists('final_unblocked.txt'):
        print("Please ensure final_unblocked.txt is in the same directory.")
        sys.exit(1)
        
    unblocked_ids = set()
    with open('final_unblocked.txt', 'r') as f:
        for line in f:
            parts = line.split('|')
            if len(parts) >= 1:
                unblocked_ids.add(parts[0].strip())

    print(f"Loaded {len(unblocked_ids)} unblocked driver IDs.")
    
    # Collect SHA256 hashes into a dictionary: sha256 -> filename
    target_hashes = {}
    for d in loldrivers:
        if d.get('Id') in unblocked_ids:
            samples = d.get('KnownVulnerableSamples', [])
            for s in samples:
                sha256 = s.get('SHA256')
                filename = s.get('Filename')
                if sha256 and sha256 != "-":
                    sha256 = sha256.lower()
                    target_hashes[sha256] = filename if filename else f"{sha256}.sys"

    print(f"Found {len(target_hashes)} unique samples (SHA256 hashes) to download.")

    if API_KEY == 'YOUR_API_KEY_HERE' or not API_KEY:
        print("\n[!] IMPORTANT: You need to insert your MalwareBazaar API Key in this script for the fallback downloads.")
        sys.exit(1)

    os.makedirs('drivers_out', exist_ok=True)
    os.makedirs('repo_cache', exist_ok=True)

    # 1. Clone or pull the repositories
    print("\n[*] Updating local GitHub driver repositories...")
    for url in REPOS:
        repo_name = url.split('/')[-1].replace('.git', '')
        repo_dir = os.path.join('repo_cache', repo_name)
        try:
            if not os.path.exists(repo_dir):
                print(f"    Cloning {repo_name}...")
                subprocess.run(["git", "clone", "--depth", "1", url, repo_dir], check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            else:
                print(f"    Pulling {repo_name}...")
                subprocess.run(["git", "-C", repo_dir, "pull"], check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        except subprocess.CalledProcessError:
            print(f"    [!] Failed to clone/pull {repo_name}. Skipping...")

    # 2. Search repositories for matching hashes
    print("\n[*] Scanning repositories for target drivers...")
    found_in_repos = 0
    for root, dirs, files in os.walk('repo_cache'):
        if '.git' in dirs:
            dirs.remove('.git') # don't visit .git directories
            
        for file in files:
            if not target_hashes:
                break
                
            filepath = os.path.join(root, file)
            file_hash = get_sha256(filepath)
            
            if file_hash in target_hashes:
                filename = target_hashes[file_hash]
                out_path = os.path.join('drivers_out', filename)
                shutil.copy2(filepath, out_path)
                print(f" [+] Found in GitHub: {filename} ({file_hash})")
                del target_hashes[file_hash]
                found_in_repos += 1

    print(f"\n[*] Successfully extracted {found_in_repos} drivers from GitHub repositories.")

    # 3. Fallback to MalwareBazaar
    if target_hashes:
        print(f"[*] {len(target_hashes)} drivers not found in GitHub repos. Falling back to MalwareBazaar...")
        
        for sha256, filename in list(target_hashes.items()):
            print(f"Downloading {filename} ({sha256})...")
            data = {
                'query': 'get_file',
                'sha256_hash': sha256
            }
            try:
                response = requests.post(MB_URL, data=data, headers=HEADERS, timeout=15)
                if 'file_not_found' in response.text:
                    print(f" [-] File not found on MalwareBazaar")
                    # Delay to avoid 502 errors
                    time.sleep(1)
                    continue
                    
                zip_path = os.path.join('drivers_out', f"{sha256}.zip")
                with open(zip_path, 'wb') as f:
                    f.write(response.content)
                
                ZIP_PASSWORD = 'infected'
                ret_code = os.system(f"unzip -P {ZIP_PASSWORD} -q {zip_path} -d drivers_out/")
                
                if ret_code == 0:
                    print(f" [+] Saved and extracted from MalwareBazaar")
                    os.remove(zip_path)
                    
                    # Rename the extracted .sys file to the correct filename if needed
                    # Unzip usually extracts it as sha256.sys, but we'll rename it just in case
                    extracted_file = os.path.join('drivers_out', f"{sha256}.sys")
                    target_file = os.path.join('drivers_out', filename)
                    if os.path.exists(extracted_file) and extracted_file != target_file:
                        os.rename(extracted_file, target_file)
                        
                else:
                    print(f" [!] Error extracting zip (Exit code {ret_code})")
                    try:
                        with open(zip_path, 'r', errors='ignore') as err_f:
                            content = err_f.read(150)
                            if "{" in content or "<html" in content:
                                print(f"     => API Response: {content.strip()}")
                    except:
                        pass
                        
            except Exception as e:
                print(f" [!] Error downloading: {e}")
                
            # Sleep to prevent 502 Bad Gateway / rate limiting from MalwareBazaar
            time.sleep(1)

if __name__ == "__main__":
    main()
