import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

r = int(raw_input())
l = int(raw_input())

# print >> sys.stderr, "r", r
# print >> sys.stderr, "l", l

wk_l  = [ r ]
for i in range( l - 1 ) :

    # print >> sys.stderr, "wk_l", wk_l

    tmp_l    = []
    compteur = 0
    prev_v   = wk_l[ 0 ]

    for v in wk_l :
        if ( v == prev_v ) :
            compteur += 1
        else :
            tmp_l.append( compteur )
            tmp_l.append( prev_v )

            compteur = 1
            prev_v   = v

    # Tail code
    tmp_l.append( compteur )
    tmp_l.append( v )
    
    wk_l = tmp_l[ : ]

# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

for i in wk_l :
    print i,
