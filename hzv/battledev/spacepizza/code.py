from __future__ import print_function
import sys

from operator import itemgetter
from math import sqrt
from math import floor

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

lines = []
for line in sys.stdin:
    lines.append(line.rstrip('\n'))

lines.pop(0)
local_print(lines)
coords = list( map( lambda s : list( map( lambda c : float( c ) , s.split( ) ) ), lines ) )
coords = list( map( lambda l : tuple( l ), coords ) )

coords.sort( key = itemgetter( 1 ) )

local_print(coords)

def dist( c0, c1 ):
    return sqrt( (c0[0] - c1[0]) ** 2 + (c0[1] - c1[1]) ** 2 + (c0[2] - c1[2]) ** 2 )

start_pos = coords[ 0 ]
end_pos   = coords[ -1 ]

coords.pop( 0 )

tot_dist = 0
cur_pos = start_pos
local_print( "BEGIN FIRST LOOP" )
while (cur_pos != end_pos):
    min_dist = None
    next_pos = None
    for coord in coords :
        if ( coord[ 1 ] < cur_pos[ 1 ] ):
            continue
        tmp_dist = dist( coord, cur_pos )
        if ( min_dist is None ):
            min_dist = tmp_dist
            next_pos = coord
            continue
        
        if ( tmp_dist < min_dist ):
            min_dist = tmp_dist
            next_pos = coord

    coords.remove( next_pos )
    cur_pos = next_pos
    tot_dist += min_dist

    local_print( next_pos )
    local_print( tot_dist )

local_print( "END FIRST LOOP" )
local_print( tot_dist )
local_print( coords )

cur_pos = end_pos
while ( coords != [] ):
    min_dist = None
    next_pos = None
    for coord in coords :
        if ( cur_pos[ 1 ] < coord[ 1 ] ):
            continue
        tmp_dist = dist( coord, cur_pos )
        local_print( tmp_dist )
        if ( min_dist is None ):
            min_dist = tmp_dist
            next_pos = coord
            continue

        if ( tmp_dist < min_dist ):
            min_dist = tmp_dist
            next_pos = coord

    local_print( next_pos )
    coords.remove( next_pos )
    cur_pos = next_pos
    tot_dist += min_dist

local_print( tot_dist )
local_print( cur_pos )
local_print( dist( cur_pos, start_pos ) )

tot_dist += dist( cur_pos, start_pos )
local_print( tot_dist )
print( floor( tot_dist )) 
