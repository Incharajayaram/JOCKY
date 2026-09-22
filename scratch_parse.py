import re
import json

raw_text = """
1 1.sys Hash deny 2 1 1 0
2 2.sys Hash deny 2 1 1 0
3 32-bit dell dbutil.sys Hash deny 4 1 1 2
4 64-bit dell dbutil.sys Hash deny 4 1 1 2
5 80.sys Hash deny 2 1 1 0
6 81.sys Hash deny 6 3 3 0
7 Agent64\\05f052_4045ae_694848_8cb62c_b1d962 Hash deny 4 1 1 2
8 AMD AODDriver\\478bcb750017cb6541f3dd0d08a47370f3c92eec998bc3825b5d8e08ee831b70 Hash deny 4 1 1 2
9 AMD AODDriver\\5daa8fa3b5db2e6225a2effea41af95fe7ffc579550c4081c8028ed33bc023b8 Hash deny 4 1 1 2
10 AMD PDFWKRNL.sys\\0cf84400c09582ee2911a5b1582332c992d1cd29fcf811cb1dc00fcd61757db0 Hash deny 1 0 0 0
"""
