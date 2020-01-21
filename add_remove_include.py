#!/bin/env python3

import sys

if len(sys.argv) < 3:
    print("usage: " + sys.argv[0] + " <file> <add|remove>")
    exit(1)

lines = open(sys.argv[1], 'r').readlines()

if sys.argv[2] == "add":
    last = -1
    for i in range(len(lines)):
        if '#include' in lines[i]:
            last = i
    lines.insert(last + 1, '#include "malloc_test.h"\n')
elif sys.argv[2] == "remove":
    lines = list(filter(lambda l: '#include "malloc_test.h"' not in l, lines))
else:
    print("second argument should be add or remove")
    exit(1)

file = open(sys.argv[1], 'w')
for line in lines:
    file.write(line)
