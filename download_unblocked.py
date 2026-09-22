import json
import requests
import os
import sys

# MalwareBazaar API Key - Get yours from https://bazaar.abuse.ch/
API_KEY = 'YOUR_API_KEY_HERE'
HEADERS = {'API-KEY': API_KEY}
MB_URL = 'https://mb-api.abuse.ch/api/v1/'

def main():
    if not os.path.exists('loldrivers.json'):
        print("Please ensure loldrivers.json is in the same directory.")
        sys.exit(1)
        
    with open('loldrivers.json', 'r') as f:
        loldrivers = json.load(f)

    # We will read final_unblocked.txt to get the IDs we want to download
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
    
    # Collect SHA256 hashes
    hashes_to_download = []
    for d in loldrivers:
        if d.get('Id') in unblocked_ids:
            samples = d.get('KnownVulnerableSamples', [])
            for s in samples:
                sha256 = s.get('SHA256')
                filename = s.get('Filename')
                if sha256 and sha256 != "-":
                    hashes_to_download.append((sha256, filename if filename else f"{sha256}.sys"))

    print(f"Found {len(hashes_to_download)} unique samples (SHA256 hashes) to download.")

    if API_KEY == 'YOUR_API_KEY_HERE':
        print("\n[!] IMPORTANT: You need to insert your MalwareBazaar API Key in this script to download files.")
        print("You can register for free at https://bazaar.abuse.ch/ to get an API key.")
        
        # Save hashes to a file for convenience anyway
        with open('unblocked_hashes.txt', 'w') as out_f:
            for sha256, _ in hashes_to_download:
                out_f.write(sha256 + "\n")
        print("[*] Saved the list of SHA256 hashes to 'unblocked_hashes.txt'")
        sys.exit(0)

    # Download loop
    os.makedirs('drivers_out', exist_ok=True)
    for sha256, filename in hashes_to_download:
        print(f"Downloading {filename} ({sha256})...")
        data = {
            'query': 'get_file',
            'sha256_hash': sha256
        }
        try:
            response = requests.post(MB_URL, data=data, headers=HEADERS, timeout=15)
            if 'file_not_found' in response.text:
                print(f" [-] File not found on MalwareBazaar: {sha256}")
                continue
                
            # The API returns a zip file containing the malware sample. 
            # The password for the zip is always 'infected'.
            out_path = os.path.join('drivers_out', f"{sha256}.zip")
            with open(out_path, 'wb') as f:
                f.write(response.content)
            
            # Unzip mechanism with password placeholder
            ZIP_PASSWORD = 'infected' # Password for MalwareBazaar zips
            try:
                # Using system unzip as it natively supports all encryption types
                os.system(f"unzip -P {ZIP_PASSWORD} -q {out_path} -d drivers_out/")
                print(f" [+] Saved and extracted to drivers_out/ (Password used: {ZIP_PASSWORD})")
                os.remove(out_path) # Clean up zip after extraction
            except Exception as unzip_err:
                print(f" [!] Error extracting {out_path}: {unzip_err}")
                
        except Exception as e:
            print(f" [!] Error downloading {sha256}: {e}")

if __name__ == "__main__":
    main()
