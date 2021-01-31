from __future__ import print_function
import sys

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

lines = []
for line in sys.stdin:
    lines.append(line.rstrip('\n'))

lines.pop(0)
local_print(lines)

N = len( lines[ 0 ] )
local_print(N)

coords = []
for i in range( N ):
    for j in range( N ):
        if ( lines[i][j] == "X" ):
            coords.append( (i, j) )

local_print(coords)

count = 0
already_count = []
for i, j in coords:
    i_min = max( 0, i - 1 )
    j_min = max( 0, j - 1 )
    i_max = min( N - 1, i + 1 )
    j_max = min( N - 1, j + 1 )

    # local_print( i_min )
    # local_print( j_min )
    # local_print( i_max )
    # local_print( j_max )

    for i in range( i_min, i_max + 1 ):
        for j in range( j_min, j_max + 1 ):
            if ( ( (i, j) not in already_count ) and ( lines[ i ][ j ] == "." ) ):
                already_count.append( ( i, j ) )
                count += 1

    # local_print( count )
    
    # break

print ( count )
