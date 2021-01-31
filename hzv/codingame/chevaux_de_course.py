import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.
l_puissances = list()

n = int(raw_input())
for i in xrange(n):
    pi = int(raw_input())
    l_puissances.append( pi )

l_puissances.sort()
# print >> sys.stderr, "l_puissances", l_puissances

l_dist = []

if ( len( l_puissances ) <= 1 ):
    l_dist.append( 0 ) 
else :
    l_dist = [ abs( p1 - p2 ) for ( p1, p2 ) in zip( l_puissances[ : -1 ], l_puissances[ 1 : ] ) ]

# print >> sys.stderr, "l_dist", l_dist
print min( l_dist )
