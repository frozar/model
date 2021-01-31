from __future__ import print_function
import sys

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

lines = []
for line in sys.stdin:
    lines.append(line.rstrip('\n'))

lines.pop(0)
local_print(lines)

