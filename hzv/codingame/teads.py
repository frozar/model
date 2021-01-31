import sys
import math

def get_a_leaf( graphe ) :
    l = [ ( len( graphe[ n ] ), n ) for n in graphe.keys() ]
    l.sort()
    return l[ 0 ][ 1 ]


def get_graphe_deep( graphe, src, forgiven ) :
    # print >> sys.stderr, "src", src
    # print >> sys.stderr, "graphe[ src ]", graphe[ src ]
    
    tmp_res = set()
    for n in graphe[ src ] :
        if not ( n in forgiven ) :
            forgiven.add( n )
            tmp_res.add( 1 + get_graphe_deep( graphe, n, forgiven ) )
                
    if ( not tmp_res ) :
        return 0
    else :
        return max( tmp_res ) 
    

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

graphe = {}

n = int(raw_input())  # the number of adjacency relations
for i in xrange(n):
    # xi: the ID of a person which is adjacent to yi
    # yi: the ID of a person which is adjacent to xi
    x, y = [int(j) for j in raw_input().split()]

    graphe[ x ] = graphe.get( x, [] ) + [ y ]
    graphe[ y ] = graphe.get( y, [] ) + [ x ]
    
# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."


leaf = get_a_leaf( graphe )

print >> sys.stderr, "n", n
print >> sys.stderr, "leaf", leaf
# print >> sys.stderr, "graphe", graphe

# The minimal amount of steps required to completely propagate the advertisement
res = get_graphe_deep( graphe, leaf, set( [ leaf ] ) )

if ( res % 2 == 0 ) :
    print res / 2
else : 
    print ( res / 2 ) + 1

# print "1"
