from __future__ import print_function
import sys

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

lines = []
for line in sys.stdin:
	lines.append(line.rstrip('\n'))

# local_print(lines)
N = int( lines[0] )

grid = list()
for i in range(N):
    grid.append( '=' * N )

def print_grid( grid ):
    for line in grid:
        local_print(line)

def set_grid( grid, t ):
    # local_print(t[0])
    # grid[ t[0] ][ t[1] ] = '#'
    grid[ t[0] ] = grid[ t[0] ][ : t[1] ] + '#' + grid[ t[0] ][ t[1] + 1 : ] 

# direction:
# 0 : to the left
# 1 : to the bottom
# 2 : to the right
# 3 : to the top

direction = 0
cur_pos = ( N//2, N//2 )
set_grid(grid, cur_pos)
# print_grid( grid )
# local_print( cur_pos )
seg_len = 1
while (True):
    i, j = cur_pos
    
    if ( direction == 0 ):
        i_end = i
        j_end = j - seg_len
    elif ( direction == 1 ):
        i_end = i + seg_len
        j_end = j
    elif ( direction == 2 ):
        i_end = i
        j_end = j + seg_len
    elif ( direction == 3 ):
        i_end = i - seg_len
        j_end = j

    is_finish = False
    if (not( ( 0 <= i_end ) and ( i_end < N ) and
             ( 0 <= j_end ) and ( j_end < N ) ) ):
        is_finish = True

    if ( is_finish ):
        i_end = max( i_end, 0 )
        i_end = min( i_end, N - 1 )
        j_end = max( j_end, 0 )
        j_end = min( j_end, N - 1 )

    next_pos = ( i_end, j_end )

    if ( i == i_end ):
        if ( j_end < j ):
            (j, j_end) = (j_end, j)

        for tmp_j in range(j, j_end + 1):
            set_grid( grid, (i, tmp_j) )

    elif ( j == j_end ):
        if ( i_end < i ):
            (i, i_end) = (i_end, i)

        for tmp_i in range(i, i_end + 1):
            set_grid( grid, (tmp_i, j) )

    cur_pos = next_pos
    # print_grid( grid )
    # local_print( cur_pos )

    seg_len  += 1
    direction = ( direction + 1 ) % 4

    if ( is_finish ):
        break

for i in range( N ) :
    print(grid[ i ])
