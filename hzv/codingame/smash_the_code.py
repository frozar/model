import sys
import math

import copy as cp
import numpy as np

HAS_ENOUGH_SPACE = True

def print_grid( grid ):
    for row in grid :
        print >> sys.stderr, row

def is_full( grid, x ):
    return grid[ 1 ][ x ] != '.'

def ratio_space( grid ):
    nb_case = 12 * 6

    nb_space = sum( map( lambda row : row.count( '.' ), grid ) )

    return float( nb_space ) / nb_case

def is_almost_full( grid ):

    return ratio_space( grid ) < 0.5

def get_lines_from_grid( grid ):
    lines = []

    for i in xrange( 6 ):
        lines.append( [] )

    for row in grid:
        for i, c in enumerate( row ):
            line = lines[ i ]
            if ( line is None ):
                line = []
            line.append( c )

    return lines

def get_grid_from_lines( lines ):
    grid = []

    for i in xrange( 12 ):
        grid.append( [] )

    for line in lines:
        for i, c in enumerate( line ):
            row = grid[ i ]
            if ( row is None ):
                row = []
            row.append( c )

    return grid

def test_fallen_piece( grid, piece, j, verbose = False ):
    i_last_space = 11
    for i, row in enumerate( grid ):
        # print >> sys.stderr, "row", row
        if ( row[ j ] != -1 ):
            i_last_space = i - 1
            break

    succed = None
    if ( 1 <= i_last_space ):
        grid[ i_last_space - 1 ][ j ], grid[ i_last_space ][ j ] = piece
        succed = True
    else:
        succed = False

    if ( verbose ):
        print >> sys.stderr, "j", j
        print >> sys.stderr, "piece", piece
        print_grid( grid )
        print >> sys.stderr, "succed", succed
        print >> sys.stderr, ""
        
    return succed

def get_color_groups( l_position_color ):

    groups = []
    
    while( l_position_color ):
        init_pos = l_position_color.pop( 0 )

        wk_group = [ init_pos ]
        res_group = []
    
        while( wk_group ):
            pos = wk_group.pop( 0 )
            res_group.append( pos )
            
            for inc in [ (1, 0), (-1, 0), (0, 1), (0, -1) ]:
                wk_pos = map( lambda x : sum( x ), zip( pos, inc ) )
                wk_pos = tuple( wk_pos )
        
                if ( wk_pos in l_position_color ):
                    wk_group.append( wk_pos )
                    l_position_color.remove( wk_pos )

        groups.append( res_group )

    return groups

def may_explosed_color( l_position_color ):

    if ( l_position_color == [] ):
        return False

    groups = get_color_groups( l_position_color )

    res = map( lambda x : len( x ), groups )

    return 4 <= max( res )
    
    # res = []

    # while( l_position_color ):
    #     init_pos = l_position_color.pop( 0 )
    #     cpt = 1

    #     pos_to_visit = [ init_pos ]
    
    #     while( pos_to_visit ):
    #         pos = pos_to_visit.pop( 0 )
            
    #         for inc in [ (1, 0), (-1, 0), (0, 1), (0, -1) ]:
    #             wk_pos = map( lambda x : sum( x ), zip( pos, inc ) )
    #             wk_pos = tuple( wk_pos )
        
    #             if ( wk_pos in l_position_color ):
    #                 cpt += 1
    #                 pos_to_visit.append( wk_pos )
    #                 l_position_color.remove( wk_pos )

    #     res.append( cpt )

    # if ( res ):
    #     return 4 <= max( res )
    # else:
    #     return False

def get_colors( grid ):

    colors = []
    for i in xrange( 1, 6 ):
        colors.append( np.where( grid == i ) )
    
    # print >> sys.stderr, "colors", colors

    return colors
    
    # for i, row in enumerate( grid ):
    #     # print >> sys.stderr, "row", row
    #     for j, c in enumerate( row ):
    #         # print >> sys.stderr, "c", c
    #         if ( c != -1 ):
    #             l_color_position = colors[ c ]
    #             l_color_position[ np.where( a == -1 )[ 0 ][ : 2 ] ] = i, j
                
    # exit()
    
    # colors = []
    # for i in xrange( 6 ):
    #     colors.append( [] )

    # # print_grid( grid )

    # # print >> sys.stderr, "grid", grid
    # # print >> sys.stderr, "grid[ 0 ]", grid[ 0 ]
    
    # for i, row in enumerate( grid ):
    #     # print >> sys.stderr, "row", row
    #     for j, c in enumerate( row ):
    #         # print >> sys.stderr, "c", c
    #         if ( c.isdigit() ):
    #         # if ( isinstance( c, int ) ):
    #             colors[ int( c ) ].append( ( i, j ) )

    # return colors

def may_explosed( grid ):
    colors = get_colors( grid )
    
    # print >> sys.stderr, "colors", colors
    res = []
    for i_color in xrange( 1, 6 ):
        res.append( may_explosed_color( colors[ i_color ] ) )

    return reduce( lambda x, y : x or y, res )

def first_space_position( line ):
    r_line = reduce( lambda x, y : x + y, line[ :: -1 ] )
    i_space = r_line.find( '.' )
    
    if( i_space == -1 ):
        i_space = 12

    # print >> sys.stderr, "r_line", r_line
    # print >> sys.stderr, "i_space", i_space

    return i_space

def get_lowest_position_in_lines( grid ):
    lines = get_lines_from_grid( grid )

    res = []
    for line in lines:
        i_space = first_space_position( line )
        res.append( i_space )

    # print >> sys.stderr, "res", res
    return res.index( min( res ) )

def get_explosed_x( grid, piece ):
    x = None
    lines = get_lines_from_grid( grid )
    for i in range( 6 ):
        # If there is not enough place for a piece
        if ( lines[ i ][ 1 ] != '.' ):
            continue
        
        # print >> sys.stderr, "piece", piece
        wk_grid, succed = test_fallen_piece( grid, piece, i )

        # TODO: get the better explosion
        if ( may_explosed( wk_grid ) ):
            x = i
            break

    # print >> sys.stderr, "x", x
    if ( x is None ):
        x = get_lowest_position_in_lines( grid )
        
    return x

def get_close_x( grid, piece ):
    x = None
    l_pos = []
    # print >> sys.stderr, "grid", grid
    lines = get_lines_from_grid( grid )
    # print >> sys.stderr, "lines", lines
    for i in range( 6 ):

        # If there is not enough place for a piece
        if ( lines[ i ][ 1 ] != '.' ):
            continue
        
        # print >> sys.stderr, "piece", piece
        wk_grid, succed = test_fallen_piece( grid, piece, i )

        # print_grid( wk_grid )
        # print >> sys.stderr, "may_explosed( wk_grid )", may_explosed( wk_grid )
        if ( not may_explosed( wk_grid ) ):
            l_pos.append( i )

    # print >> sys.stderr, "l_pos", l_pos

    # print_grid( grid )
    # print >> sys.stderr, "lines", lines
    
    dist_index_i = []
    for i in l_pos :
        i_space = first_space_position( lines[ i ] )
        dist_index_i.append( ( i_space, i ) )
        # print >> sys.stderr, "i", i
        # print >> sys.stderr, "i_space", i_space

    # dist_index_i.sort( reverse = True )
    dist_index_i.sort( )
    # print >> sys.stderr, "dist_index_i", dist_index_i

    if ( dist_index_i ):
        x = dist_index_i[ 0 ][ 1 ]

    else :
        x = get_lowest_position_in_lines( grid )

    return x

def apply_gravity( grid ):
    # print >> sys.stderr, "apply_gravity"

    # print_grid( grid.T )
    for line in grid.T:
        new_line = line[ line != -1 ]
        # print >> sys.stderr, "new_line", new_line
        line[ np.arange( 12 - len( new_line ) ) ] = -1
        line[ np.arange( 12 - len( new_line ), 12 ) ] = new_line

    # print_grid( grid )
    return
        
    # exit()
    # lines = get_lines_from_grid( grid )

    # for line in lines:
    #     no_space = filter( lambda x : x != '.', line )
    #     line     = [ '.' ] * ( 12 - len( no_space ) ) + no_space
    
    # # print >> sys.stderr, 'lines', lines
    # grid = get_grid_from_lines( lines )

    # # print_grid( grid )
    # return grid

def reduce_grid_atomic( grid ):
    colors = get_colors( grid )

    # print >> sys.stderr, "colors", colors
    # print >> sys.stderr, zip( colors[ 0 ][ 0 ], colors[ 0 ][ 1 ] )
    # get_color_groups( zip( colors[ 0 ][ 0 ], colors[ 0 ][ 1 ] ) )

    has_change = False
    for i_color in xrange( 5 ):
        # groups = get_color_groups( colors[ i_color ] )
        groups = get_color_groups( zip( colors[ i_color ][ 0 ],
                                        colors[ i_color ][ 1 ] ) )
        to_explose_group = []

        for gr in groups :
            if ( 4 <= len( gr ) ):
                has_change = True
                to_explose_group.append( gr )

        for gr in to_explose_group:
            # print >> sys.stderr, "gr", gr
            # print_grid( grid )

            for pos in gr :
                i, j = pos
                # grid[ i ][ j ] = '.'
                grid[ i ][ j ] = -1

                # Take into account the skulls
                for inc in [ (1, 0), (-1, 0), (0, 1), (0, -1) ]:
                    wk_pos = map( lambda x : sum( x ), zip( pos, inc ) )
                    i, j = wk_pos
                    i = max( 0, min( 11, i ) )
                    j = max( 0, min( 4, j ) )
                    if ( grid[ i ][ j ] == 0 ):
                        grid[ i ][ j ] = -1

            # print_grid( grid )
            # print >> sys.stderr, "gr", gr

    # exit()
    # grid = apply_gravity( grid )
    apply_gravity( grid )

    # return grid, has_change
    return has_change

def reduce_grid( grid ):
    nb_chaine = 0

    # grid, has_change = reduce_grid_atomic( grid )
    has_change = reduce_grid_atomic( grid )
    while ( has_change ):
        nb_chaine += 1
        # grid, has_change = reduce_grid_atomic( grid )
        has_change = reduce_grid_atomic( grid )

    # return grid, nb_chaine
    return nb_chaine

inc_vector = None

def get_next_submission_pos_2( origin_submission_pos, submission_pos, pieces ):
    global inc_vector

    res = None
    
    # if( submission_pos is None ) :
    if( submission_pos is None ) :
        # submission_pos = map( lambda x : x[ 0 ], pieces )
        # res = map( lambda x : x[ 0 ], pieces )
        res = origin_submission_pos[ : ]
        inc_vector = [ 0 ] * len( pieces )
        # print >> sys.stderr, "pieces", pieces
        # print >> sys.stderr, "submission_pos", submission_pos
    else :
        for i in reversed( range( len( pieces ) ) ):
            if ( inc_vector[ i ] == 0 ):
                inc_vector[ i ] = -1
                inc_vector[ i + 1 : ] = [ 0 ] * len( inc_vector[ i + 1 : ] )
                break
            else :
                inc_vector[ i ] = 0

        # print >> sys.stderr, "inc_vector", inc_vector
        if ( inc_vector == [ 0 ] * len( pieces ) ):
            return []

        res = origin_submission_pos[ : ]
        for i in xrange( len( pieces ) ):
            # submission_pos[ i ] += inc_vector[ i ]
            res[ i ] += inc_vector[ i ]

    # return submission_pos
    return res

def get_next_submission_pos( submission_pos, nb_piece ):
    if ( submission_pos is None ):
        return [ 0 ] * nb_piece
    if( submission_pos == [ 5 ] * nb_piece ):
        return []
    else:
        str_positions = reduce( lambda x, y : x + y,
                                map(lambda pos : str( pos ), submission_pos ) )
        cur_nb = int( str_positions, 6 )
        next_nb = cur_nb + 1
        str_nb = np.base_repr( next_nb, 6 )
        str_nb = '0' * ( nb_piece - len( str_nb ) ) + str_nb
        # res = [ int( pos ) for pos in list( np.base_repr( next_nb, 6 ) ) ]
        res = [ int( pos ) for pos in list( str_nb ) ]
        return res

def get_max_height_grid( grid ):
    max_height = 0
    for line in grid.T :
        max_height = max( max_height, len( line[ line != -1 ] ) )
    # print >> sys.stderr, grid[ grid != -1 ]
    # print >> sys.stderr, type( grid )
    # print >> sys.stderr, "max_height", max_height
    return max_height

def get_global_height_grid( grid ):

    global_height = 0
    for line in grid.T :
        global_height += len( line[ line != -1 ] )
    # print >> sys.stderr, grid[ grid != -1 ]
    # print >> sys.stderr, type( grid )
    # print >> sys.stderr, "global_height", global_height
    return global_height

i_global = 1

def look_for_chain( grid, pieces ):

    # nb_piece = 5
    nb_piece = 5
    # Cut off the number of pieces to deal with
    pieces = pieces[ : nb_piece ]
    
    res_to_sort = []
    submission_pos = None

    default_pos = np.arange( 1, 6 )
    
    # min_height = 12
    wk_grid = cp.deepcopy( grid )
    
    origin_submission_pos = map( lambda x : x[ 0 ], pieces )
    i_dbg = 0
    while ( submission_pos != [] ):
        i_dbg += 1
        # submission_pos = get_next_submission_pos( submission_pos, nb_piece )
        submission_pos = get_next_submission_pos_2( origin_submission_pos,
                                                    submission_pos,
                                                    pieces )

        if( submission_pos == [] ):
            break
        #     print >> sys.stderr, "submission_pos", submission_pos
        #     print >> sys.stderr, "i_dbg", i_dbg
            
        # print >> sys.stderr, "submission_pos", submission_pos
        # print >> sys.stderr, "i_dbg", i_dbg
            
        score = 0
    
        # wk_grid = cp.deepcopy( grid )
        # wk_grid = grid.copy()
        np.copyto( wk_grid, grid )
        # wk_grid = grid

        continue_the_test = True
        for piece, pos in zip( pieces, submission_pos ):
            # wk_grid, succed = test_fallen_piece( wk_grid, piece, pos )
            # if ( i_global == 106 ):
            #     print >> sys.stderr, "zip( pieces, submission_pos )", zip( pieces, submission_pos )
            #     succed = test_fallen_piece( wk_grid, piece, pos, True )
            # else :
            #     succed = test_fallen_piece( wk_grid, piece, pos )
                
            succed = test_fallen_piece( wk_grid, piece, pos )
            
            if ( not succed ):
                continue_the_test = False
                break

            # wk_grid, nb_chaine = reduce_grid( wk_grid )
            nb_chaine = reduce_grid( wk_grid )

            # The score increse only if chaine are detected
            if ( 1 < nb_chaine ):
                score += nb_chaine

        if ( not continue_the_test ):
            continue

        # i_dbg += 1

        # if ( i_dbg % 10 == 0 ):
        #     print >> sys.stderr, "res_to_sort", res_to_sort
        #     print_grid( wk_grid )

        # print >> sys.stderr, type( wk_grid )

        # global_height = get_global_height_grid( wk_grid )
        height = get_max_height_grid( wk_grid )

        # print >> sys.stderr, "height", height
        
        # print_grid( wk_grid )
        
        # if ( height < min_height ):
        #     min_height = height
            # print >> sys.stderr, "min_height", min_height
        
        res_to_sort.append( ( score, height, submission_pos ) )

    def f_cmp( x, y ):
        if ( x[ 0 ] < y[ 0 ] ):
            return 1
        elif( x[ 0 ] > y[ 0 ] ):
            return -1
        else :
            if ( x[ 1 ] < y[ 1 ] ):
                return -1
            elif( x[ 1 ] > y[ 1 ] ):
                return 1
            else :
                return 0
        
    res_to_sort.sort( cmp = f_cmp )

    # test_fallen_piece( grid, piece, 4, True )
    score, height, better_submission = None, None, None
    if ( res_to_sort ):
        score, height, better_submission = res_to_sort[ 0 ]
    else:
        score, height, better_submission = 0, 666, [ 0 ] * nb_piece

    # print >> sys.stderr, "res_to_sort", res_to_sort[ : 4 ]
    # print >> sys.stderr, "score", score
    # print >> sys.stderr, "global_height", height
    # print >> sys.stderr, "better_submission", better_submission

    # exit()
    
    # print_grid( wk_grid )
    # print >> sys.stderr, "submission_pos", submission_pos

    # for i in better_submission :
    #     print i

    return score, better_submission
        
def get_next_x( pieces, grid ):

    global HAS_ENOUGH_SPACE
    
    score, better_submission = look_for_chain( grid, pieces )

    return better_submission[ 0 ]
    
    # print >> sys.stderr, "grid", grid
    
    # if ( 1 < score ):
    #     print >> sys.stderr, "score", score
    #     print >> sys.stderr, "better_submission", better_submission
    #     return better_submission[ 0 ]
    
    # one_piece = pieces[ 0 ]

    # x = None
    # # if ( is_almost_full( grid ) and ( not HAS_ENOUGH_SPACE ) ):
    # if ( not HAS_ENOUGH_SPACE ):
    #     print >> sys.stderr, "is_almost_full"
    #     x = get_explosed_x( grid, one_piece )

    #     if ( ratio_space( grid ) < 0.3 ):
    #         HAS_ENOUGH_SPACE = True

    # else:
    #     print >> sys.stderr, "get_close_x"
    #     x = get_close_x( grid, one_piece )

    #     if ( ratio_space( grid ) < 0.7 ):
    #         HAS_ENOUGH_SPACE = False

    # return x

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

color_a, color_b = None, None

x = 0
my_grid  = []
his_grid = []

which_way = True

my_grid = np.zeros( ( 12, 6 ), dtype = np.int8 )

# game loop
while True:

    pieces = []
    for i in xrange(8):
        # color_a: color of the first block
        # color_b: color of the attached block
        # color_a, color_b = [int(j) for j in raw_input().split()]
        color_a, color_b = [int( j ) for j in raw_input().split()]
        pieces.append( ( color_a, color_b ) )

    # my_grid = []
    my_grid = np.zeros( ( 12, 6 ) )

    for i in xrange(12):
        row = raw_input()
        # my_grid.append( row )
        for j, c in enumerate( row ):
            if ( c == '.' ):
                my_grid[ i ][ j ] = -1
            else:
                my_grid[ i ][ j ] = int( c )

    # print >> sys.stderr, "my_grid", my_grid
        
    his_grid = []
    for i in xrange(12):
        row = raw_input()
        his_grid.append( row )
        # One line of the map ('.' = empty, '0' = skull block, '1' to '5' = colored block)

    # print >> sys.stderr, "one_piece", one_piece
    #print_grid( my_grid )

    x = get_next_x( pieces, my_grid )
    print x

    # print pieces[0][0]

    i_global += 1


