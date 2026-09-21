import re
import sys

def get_libs(filepath):
    libs = []
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
        # Look for things like -lws2_32 in comments
        for line in content.split('\n'):
            if '//' in line:
                matches = re.findall(r'-l[a-zA-Z0-9_]+', line)
                libs.extend(matches)
    return list(set(libs))

if __name__ == '__main__':
    print(" ".join(get_libs(sys.argv[1])))
