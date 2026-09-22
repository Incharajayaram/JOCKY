import json
import os
import sys
import subprocess
import shutil
import hashlib

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

    # 3. Fallback to Winbindex for missing drivers
    if target_hashes:
        print(f"\n[*] {len(target_hashes)} drivers not found in GitHub repos. Generating Winbindex links...")
        
        with open('winbindex_missing_links.txt', 'w') as f:
            f.write("=== Missing Drivers (Winbindex Lookup) ===\n")
            f.write("Click these links to manually search Winbindex and download the files from Microsoft Symbol Servers.\n\n")
            for sha256, filename in target_hashes.items():
                # Generate a Winbindex search URL
                link = f"https://winbindex.m417z.com/?file={filename}&sha256={sha256}"
                f.write(f"Driver: {filename}\n")
                f.write(f"SHA256: {sha256}\n")
                f.write(f"Link:   {link}\n")
                f.write("-" * 50 + "\n")
                
        print("[*] Missing drivers written to 'winbindex_missing_links.txt'.")

if __name__ == "__main__":
    main()
