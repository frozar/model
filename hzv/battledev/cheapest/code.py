from __future__ import print_function
import sys

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

lines = []
for line in sys.stdin:
    lines.append(line.rstrip('\n'))

lines.pop(0)
to_find = lines[ 0 ]
lines.pop(0)
# local_print(lines)

new_lines = []

min_price = None
for line in lines:
    obj, price = line.split()
    price = int ( price )
    # local_print( obj, price )
    if ( obj == to_find ):
        if ( min_price is None ):
            min_price = price
        else:
            if ( price < min_price ):
                min_price = price

print( min_price )
