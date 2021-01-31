import sys
import math


def update_parents( src, n_parents, n_weight ) :
    # print >> sys.stderr, "src, n_parents, n_weight", src, n_parents, n_weight
        
    for p in n_parents.get( src, [] ) :
        n_weight[ p ] = max( n_weight.get( p, 0 ), n_weight[ src ] + 1 )

    for p in n_parents.get( src, [] ) :
        update_parents( p, n_parents, n_weight )

    return
        
# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

n_weight  = {}
n_parents = {}

n = int(raw_input())  # the number of relationships of influence
for i in xrange(n):
    # x: a relationship of influence between two people (x influences y)
    x, y = [int(j) for j in raw_input().split()]
    # print >> sys.stderr, "x, y", x, y
        
    n_weight[ y ] = n_weight.get( y, 1 )
    n_parents[ y ] = n_parents.get( y , [] ) + [ x ]

    update_parents( y, n_parents, n_weight )
        
# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

# print >> sys.stderr, "n_weight", n_weight
# print >> sys.stderr, "n_parents", n_parents

# The number of people involved in the longest succession of influences
print max( n_weight.values() )
