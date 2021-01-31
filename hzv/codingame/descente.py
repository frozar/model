import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

d_mountains = {}

# game loop
while 1:
    space_x, space_y = [int(i) for i in raw_input().split()]
    for i in xrange(8):
        mountain_h = int(raw_input()) # represents the height of one mountain, from 9 to 0. Mountain heights are provided from left to right.

        d_mountains[ i ] = mountain_h

    # Reverse the dictionary
    l_res = [ ( v, k ) for ( k, v ) in d_mountains.items() ]
    l_res.sort( reverse = True )

    target_mountain = l_res[ 0 ][ 1 ]
    # print >> sys.stderr, "d_mountains", d_mountains
    # print >> sys.stderr, "l_res", l_res
    # print >> sys.stderr, "target_mountain", target_mountain

    # either:  FIRE (ship is firing its phase cannons) or HOLD (ship is not firing).
    # print "HOLD"
    if ( space_x == target_mountain ):
        print "FIRE"
    else:
        print "HOLD"
