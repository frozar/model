from __future__ import print_function
import sys

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

lines = []
for line in sys.stdin:
    lines.append(line.rstrip('\n'))

lines.pop(0)
lines = list( map( lambda s : list( map (lambda x : int( x ), s.split()) ), lines) )
# local_print(lines)

count_a = 0
count_b = 0
for a, b in lines:
    local_print( a )
    local_print( b )
    if ( a < b ):
        count_b += 1
    elif ( b < a ):
        count_a += 1

    local_print( count_a )
    local_print( count_b )
    local_print( "" )

if ( count_a < count_b ):
    print( "B" )
elif ( count_b < count_a ):
    print( "A" )
else:
    print( "impossible" )
