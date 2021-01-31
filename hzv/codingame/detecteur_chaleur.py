import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

def go_up ( py, lim_up ) :
    return py - int( math.ceil( float( py - lim_up ) / 2 ) )

def go_down( py, lim_down ) :
    return py + int( math.ceil( float( lim_down - py ) / 2 ) )

def go_left( px, lim_left ) :
    return px - int( math.ceil( float( px - lim_left ) / 2 ) )

def go_right( px, lim_right ) :
    return px + int( math.ceil( float( lim_right - px ) / 2 ) )

# w: width of the building.
# h: height of the building.
w, h = [int(i) for i in raw_input().split()]
n = int(raw_input())  # maximum number of turns before game over.
x0, y0 = [int(i) for i in raw_input().split()]

# print >> sys.stderr, "w, h, n", w, h, n

px, py = x0, y0

lim_up, lim_down, lim_left, lim_right = 0, h - 1, 0, w - 1

# game loop
while True:
    bomb_dir = raw_input()  # the direction of the bombs from batman's current location (U, UR, R, DR, D, DL, L or UL)

    x, y = None, None
    if ( bomb_dir == "U" ) :
        lim_down = py
        
        x = px
        y = go_up ( py, lim_up )

    elif ( bomb_dir == "D" ) :
        lim_up = py

        x = px
        y = go_down( py, lim_down )
        
    elif ( bomb_dir == "L" ) :
        lim_right = px

        x = go_left( px, lim_left )
        y = py

    elif ( bomb_dir == "R" ) :
        lim_left = px

        x = go_right( px, lim_right )
        y = py
        
    elif ( bomb_dir == "UR" ) :
        lim_left = px
        lim_down = py

        x = go_right( px, lim_right )
        y = go_up ( py, lim_up )

    elif ( bomb_dir == "UL" ) :
        lim_right = px
        lim_down = py

        x = go_left( px, lim_left )
        y = go_up ( py, lim_up )

    elif ( bomb_dir == "DR" ) :
        lim_left = px
        lim_up = py

        x = go_right( px, lim_right )
        y = go_down( py, lim_down )

    elif ( bomb_dir == "DL" ) :
        lim_right = px
        lim_up = py
    
        x = go_left( px, lim_left )
        y = go_down( py, lim_down )

    px, py = x, y
        
    # Write an action using print
    # To debug: print >> sys.stderr, "Debug messages..."

    # the location of the next window Batman should jump to.
    # print "0 0"
    print x, y
