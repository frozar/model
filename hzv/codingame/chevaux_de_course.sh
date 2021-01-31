import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

# n: the total number of nodes in the level, including the gateways
# l: the number of links
# e: the number of exit gateways
n, l, e = [int(i) for i in raw_input().split()]
for i in xrange(l):
    # n1: N1 and N2 defines a link between these nodes
    n1, n2 = [int(j) for j in raw_input().split()]
for i in xrange(e):
    ei = int(raw_input())  # the index of a gateway node

# game loop
while True:
    si = int(raw_input())  # The index of the node on which the Skynet agent is positioned this turn

    # Write an action using print
    # To debug: print >> sys.stderr, "Debug messages..."

    # Example: 0 1 are the indices of the nodes you wish to sever the link between
    print "1 2"
