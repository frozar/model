import sys
import math

# Don't let the machines win. You are humanity's last hope...

grille = []

width = int(raw_input()) # the number of cells on the X axis
height = int(raw_input()) # the number of cells on the Y axis
for i in xrange(height):
    line = raw_input() # width characters, each either 0 or .
    grille.append( list( line ) )
    
# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

# print >> sys.stderr, "grille", grille

nodes = {}

for i in range( height ) :
    for j in range( width ) :

        x1, y1 = j, i
        # print >> sys.stderr, "x1, y1", x1, y1
        
        if ( grille[ i ][ j ] == "0" ) :

            # find the next node to the right
            x2, y2 = ( None, None )
            for jj in range( j+1, width ) :
                if ( grille[ i ][ jj ] == "0" ) :
                    x2, y2 = jj, i
                    break
            
            if ( ( x2, y2 ) == ( None, None ) ) :
                x2, y2 = -1, -1
            # print >> sys.stderr, "x2, y2", x2, y2

            # find the next node below
            x3, y3 = ( None, None )
            for ii in range( i+1, height ) :
                if ( grille[ ii ][ j ] == "0" ) :
                    x3, y3 = j, ii
                    break

            if ( ( x3, y3 ) == ( None, None ) ) :
                x3, y3 = -1, -1
            # print >> sys.stderr, "x3, y3", x3, y3
            
            nodes[ ( x1, y1 ) ] = ( x2, y2, x3, y3 )

# print >> sys.stderr, "nodes", nodes

# Three coordinates: a node, its right neighbor, its bottom neighbor
# print "0 0 1 0 0 1"

for k, v in nodes.items():
    print k[ 0 ], k[ 1 ], v[ 0 ], v[ 1 ], v[ 2 ], v[ 3 ]
