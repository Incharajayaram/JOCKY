import os
import json
import csv
import glob
import cmd
from litellm import completion

class DeepZeroREPL(cmd.Cmd):
    intro = 'Welcome to the DeepZero Analysis REPL (Powered by Gemini).\nType help or ? to list commands.\nType "chat <question>" to ask the LLM about the findings.'
    prompt = '(DeepZero) '

    def __init__(self, master_data):
        super().__init__()
        self.master_data = master_data
        # Limit context to avoid blowing up token limits
        self.context_str = json.dumps(master_data)[:60000]

    def do_summary(self, arg):
        """Show a quick summary of the aggregated findings."""
        print(f"Total Driver Reports Processed: {len(self.master_data)}")
        # Heuristic count of potentially vulnerable drivers
        vuln = sum(1 for d in self.master_data if "vulnerable" in str(d).lower() or "exploitable" in str(d).lower())
        print(f"Potentially Exploitable Targets: {vuln}")

    def do_chat(self, arg):
        """Ask Gemini a question about the findings: chat <question>"""
        if not arg:
            print("Please provide a question.")
            return
            
        print("Asking Gemini...")
        messages = [
            {"role": "system", "content": "You are an expert cybersecurity vulnerability researcher analyzing output from DeepZero for Windows kernel drivers. Here is the JSON summary of findings:\n" + self.context_str},
            {"role": "user", "content": arg}
        ]
        
        try:
            response = completion(
                model=os.getenv("LITELLM_MODEL", "gemini/gemini-1.5-pro-latest"),
                messages=messages
            )
            print("\n" + response.choices[0].message.content + "\n")
        except Exception as e:
            print(f"\n[!] LLM Error: {e}\nDid you export GEMINI_API_KEY?")

    def do_exit(self, arg):
        """Exit the REPL"""
        print("Goodbye!")
        return True

def aggregate_results():
    # Crawl standard output paths
    base_dirs = ["DeepZero/work", "DeepZero/state", "DeepZero/results", "DeepZero/.state"]
    target_dir = None
    for d in base_dirs:
        if os.path.isdir(d):
            target_dir = d
            break
            
    if not target_dir:
        print("[!] Could not find DeepZero state/results directory. Have you run deepzero_runner.sh yet?")
        print("[i] We will start the REPL with empty data for now.")
        return []

    print(f"[*] Aggregating results from {target_dir}...")
    json_files = glob.glob(os.path.join(target_dir, "**/*.json"), recursive=True)
    
    master_data = []
    for fpath in json_files:
        try:
            with open(fpath, 'r') as f:
                data = json.load(f)
                master_data.append(data)
        except Exception as e:
            print(f"[-] Failed to read {fpath}: {e}")

    # Write JSON aggregation
    with open('master_report.json', 'w') as f:
        json.dump(master_data, f, indent=4)
        
    # Write CSV aggregation
    if master_data:
        keys = set()
        for d in master_data:
            if isinstance(d, dict):
                keys.update(d.keys())
            
        if keys:
            with open('master_report.csv', 'w', newline='') as f:
                writer = csv.DictWriter(f, fieldnames=list(keys))
                writer.writeheader()
                for d in master_data:
                    if isinstance(d, dict):
                        # Flatten complex nested structures for the CSV
                        row = {k: str(v) if isinstance(v, (dict, list)) else v for k, v in d.items()}
                        writer.writerow(row)

    print(f"[+] Aggregation complete! Processed {len(json_files)} JSON reports.")
    print("[+] Generated master_report.json and master_report.csv")
    return master_data

if __name__ == '__main__':
    print("==========================================")
    print("     DeepZero Result Aggregator & REPL    ")
    print("==========================================")
    
    if not os.getenv("GEMINI_API_KEY"):
        print("[!] WARNING: GEMINI_API_KEY is not set. The 'chat' REPL functionality will fail.")
        print("    Please run: export GEMINI_API_KEY='your-key'")
        
    data = aggregate_results()
    DeepZeroREPL(data).cmdloop()
