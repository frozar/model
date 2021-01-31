import sys
import math

def get_next_coord( grille, x, y, pos ) :
    res = None, None
    block_t = grille[ y ][ x ]

    if ( block_t == 0 ) :
        res = x, y
        
    elif ( block_t == 1 ) :
        res = x, y + 1
            
    elif ( block_t == 2 ) :
        if( pos == "LEFT" ) :
            res = x + 1, y
        elif( pos == "RIGHT" ) :
            res = x - 1, y
                    
    elif ( block_t == 3 ) :
        res = x, y + 1
        
    elif ( block_t == 4 ) :
        if( pos == "TOP" ) :
            res = x - 1, y
        elif( pos == "RIGHT" ) :
            res = x, y + 1

    elif ( block_t == 5 ) :
        if( pos == "TOP" ) :
            res = x + 1, y
        elif( pos == "LEFT" ) :
            res = x, y + 1

    elif ( block_t == 6 ) :
        if( pos == "LEFT" ) :
            res = x + 1, y
        elif( pos == "RIGHT" ) :
            res = x - 1, y
        
    elif ( block_t == 7 ) :
        if( pos == "TOP" ) :
            res = x, y + 1
        elif( pos == "RIGHT" ) :
            res = x, y + 1
        
    elif ( block_t == 8 ) :
        res = x, y + 1
        
    elif ( block_t == 9 ) :
        if( pos == "TOP" ) :
            res = x, y + 1
        elif( pos == "LEFT" ) :
            res = x, y + 1
        
    elif ( block_t == 10 ) :
        res = x - 1, y
        
    elif ( block_t == 11 ) :
        res = x + 1, y

    elif ( block_t == 12 ) :
        res = x, y + 1

    elif ( block_t == 13 ) :
        res = x, y + 1

    return res
        
# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

grille = []

# w: number of columns.
# h: number of rows.
w, h = [int(i) for i in raw_input().split()]
# print >> sys.stderr, "w, h", w, h

for i in xrange(h):
    line = raw_input()  # represents a line in the grid and contains W integers. Each integer represents one room of a given type.
    grille.append( map( lambda x : int( x ), line.split() ) )
    
ex = int(raw_input())  # the coordinate along the X axis of the exit (not useful for this first mission, but must be read).

# print >> sys.stderr, "grille", grille

# game loop
while True:
    x, y, pos = raw_input().split()
    x = int( x )
    y = int( y )

    # Write an action using print
    # To debug: print >> sys.stderr, "Debug messages..."

    # One line containing the X Y coordinates of the room in which you believe Indy will be on the next turn.
    xx, yy = get_next_coord( grille, x, y, pos )
    print xx, yy
