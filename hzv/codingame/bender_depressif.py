import sys
import math

def get_move( grille, cur_i, cur_j ) :
    global l_priorities, cur_direc

    i, j, move = None, None, None

    # print >> sys.stderr, "l_priorities", l_priorities
    
    next_char = None
    if ( cur_direc == 'S' ) :
        next_char = grille[ cur_i + 1 ][ cur_j ]
    elif ( cur_direc == 'E' ) :
        next_char = grille[ cur_i ][ cur_j + 1 ]
    elif ( cur_direc == 'N' ) :
        next_char = grille[ cur_i - 1 ][ cur_j ]
    elif ( cur_direc == 'W' ) :
        next_char = grille[ cur_i ][ cur_j - 1 ]

    if not ( ( next_char == '#' ) or \
             ( ( next_char == 'X' ) and ( not have_a_beer ) ) ) :

        if ( cur_direc == 'S' ) :
            i, j, move = cur_i + 1, cur_j, 'SOUTH'
        elif ( cur_direc == 'E' ) :
            i, j, move = cur_i, cur_j + 1, 'EAST'
        elif ( cur_direc == 'N' ) :
            i, j, move = cur_i - 1, cur_j, 'NORTH'
        elif ( cur_direc == 'W' ) :
            i, j, move = cur_i, cur_j - 1, 'WEST'

    else :
        for direc in l_priorities :

            next_char = None
            if ( direc == 'S' ) :
                next_char = grille[ cur_i + 1 ][ cur_j ]
            elif ( direc == 'E' ) :
                next_char = grille[ cur_i ][ cur_j + 1 ]
            elif ( direc == 'N' ) :
                next_char = grille[ cur_i - 1 ][ cur_j ]
            elif ( direc == 'W' ) :
                next_char = grille[ cur_i ][ cur_j - 1 ]
    
            # print >> sys.stderr, "next_char", next_char
                
            if ( next_char == '#' ) :
                continue
            elif ( ( next_char == 'X' ) and ( not have_a_beer ) ) :
                continue
            else :
                if ( direc == 'S' ) :
                    i, j, move = cur_i + 1, cur_j, 'SOUTH'
                elif ( direc == 'E' ) :
                    i, j, move = cur_i, cur_j + 1, 'EAST'
                elif ( direc == 'N' ) :
                    i, j, move = cur_i - 1, cur_j, 'NORTH'
                elif ( direc == 'W' ) :
                    i, j, move = cur_i, cur_j - 1, 'WEST'
                
                cur_direc = direc
    
                break
    
    return i, j, move


def find_pos_init( grille ) :
    init_i, init_j = None, None

    for i, row in enumerate( grille ) :
        for j, char in enumerate( row ) :
            if ( char == '@' ) :
                init_i, init_j = i, j
                break

    return init_i, init_j


def find_teleporter( grille ) :
    t1_i, t1_j, t2_i, t2_j = None, None, None, None

    for i, row in enumerate( grille ) :
        for j, char in enumerate( row ) :
            if ( char == 'T' ) :
                if ( t1_i is None ) :
                    t1_i, t1_j = i, j
                else :
                    t2_i, t2_j = i, j
                    break

    return t1_i, t1_j, t2_i, t2_j

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

grille = []

l, c = [int(i) for i in raw_input().split()]
for i in xrange(l):
    row = raw_input()
    row = list( row )
    grille.append( row )

print >> sys.stderr, "l", l
print >> sys.stderr, "c", c
print >> sys.stderr, "grille"
for row in grille :
    print >> sys.stderr, row

init_i, init_j         = find_pos_init( grille )
t1_i, t1_j, t2_i, t2_j = find_teleporter( grille )

# $ : cabien suicide
# I : inverse
# B : biere
# T : teleporteur
# # : obstacle incassable
# X : obstacle cassable avec biere

l_priorities = [ 'S', 'E', 'N', 'W' ]
cur_direc    = 'S'

cur_char     = "@"
cur_i, cur_j = init_i, init_j
l_move       = []
is_a_loop    = False
have_a_beer  = False

d_moves = {}

i_dbg = 0

while( not is_a_loop ) :
    
    # print >> sys.stderr, "pass0"
    
    if ( cur_char == '$' ) :
        # print >> sys.stderr, "pass1"
        break

    elif( ( cur_char == ' ' ) or \
          ( cur_char == '@' ) ) :
        # print >> sys.stderr, "pass2"
        cur_i, cur_j, move = get_move( grille, cur_i, cur_j )
        l_move.append( move )
        
    elif( cur_char == 'I' ) :
        # print >> sys.stderr, "pass3"
        l_priorities.reverse()
        cur_i, cur_j, move = get_move( grille, cur_i, cur_j )
        l_move.append( move )

    elif( cur_char == 'B' ) :
        # print >> sys.stderr, "pass4"
        have_a_beer = not have_a_beer
        cur_i, cur_j, move = get_move( grille, cur_i, cur_j )
        l_move.append( move )

    elif( cur_char == 'T' ) :
        # print >> sys.stderr, "pass5"
        if ( ( cur_i, cur_j ) == ( t1_i, t1_j ) ) :
            cur_i, cur_j = t2_i, t2_j
        else :
            cur_i, cur_j = t1_i, t1_j

        cur_i, cur_j, move = get_move( grille, cur_i, cur_j )
        l_move.append( move )

    elif( cur_char == 'S' ) :
        cur_direc = 'S'
        cur_i, cur_j, move = cur_i + 1, cur_j, 'SOUTH'
        l_move.append( move )
        
    elif( cur_char == 'E' ) :
        cur_direc = 'E'
        cur_i, cur_j, move = cur_i, cur_j + 1, 'EAST'
        l_move.append( move )
        
    elif( cur_char == 'N' ) :
        cur_direc = 'N'
        cur_i, cur_j, move = cur_i - 1, cur_j, 'NORTH'
        l_move.append( move )
        
    elif( cur_char == 'W' ) :
        cur_direc = 'W'
        cur_i, cur_j, move = cur_i, cur_j - 1, 'WEST'
        l_move.append( move )
        
    d_moves[ ( cur_i, cur_j ) ] = d_moves.get( ( cur_i, cur_j ), 0 ) + 1

    if ( 30 < d_moves[ ( cur_i, cur_j ) ] ) :
        is_a_loop = True
        break

    cur_char = grille[ cur_i ][ cur_j ]

    if( cur_char == 'X' ) :
        grille[ cur_i ][ cur_j ] = ' '
            
    # print >> sys.stderr, cur_i, cur_j, "[", cur_char, "]"#, i_dbg
    # i_dbg += 1
    # if ( 30 < i_dbg ) :
    #     break

# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

if( is_a_loop ):
    print "LOOP"
else :
    for m in l_move :
        print m

