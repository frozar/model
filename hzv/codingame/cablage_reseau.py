import sys
import math

def get_y1_y2( h_y ) :
    tot_y     = sum( h_y.values() )
    mid_tot_y = float( tot_y ) / 2
    
    elected_y = None
    cpt_y     = 0
    
    y1 = None

    for y2 in sorted( h_y.keys() ) :
        cpt_y += h_y[ y2 ]
        if( cpt_y >= mid_tot_y ) :
            elected_y = ( y1, y2 )
            break
            
        y1 = y2

    return elected_y


def compute_res( all_x, all_y, elected_y ) :
    res   = max( all_x ) - min( all_x )
    for y in all_y :
        res += abs( elected_y - y )

    return res

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

pts = []

n = int(raw_input())
for i in xrange(n):
    x, y = [int(j) for j in raw_input().split()]
    pts.append( ( x, y ) )

# print >> sys.stderr, "pts", pts

h_y = {}

all_x = map( lambda pt : pt[ 0 ], pts )
all_y = map( lambda pt : pt[ 1 ], pts )

for y in all_y :
    h_y[ y ] = h_y.get( y, 0 ) + 1
 
y1, y2 = get_y1_y2( h_y )

# print >> sys.stderr, "y1, y2", y1, y2

res_1, res_2 = 0, 0

if( y1 is not None ):
    res_1 = compute_res( all_x, all_y, y1 )

res_2 = compute_res( all_x, all_y, y2 )

# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

# print "answer"
if( res_1 < res_2 ) :
    print res_1
else :
    print res_2
