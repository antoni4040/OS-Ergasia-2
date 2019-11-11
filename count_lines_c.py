import os

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

line_number = 0

for root, dirs, files in os.walk("."):
    path = root.split(os.sep)
    for file in files:
        if(file.endswith(".c") or file.endswith(".h")):
            print(file)
            line_number += file_len(os.path.join(root, file))

print("Total C lines number: ", str(line_number), "\n")