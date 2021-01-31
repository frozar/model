import sys
import math

x_len = 35
x_max = x_len - 1

y_len = 20
y_max = y_len - 1

sqr_len_max = 9
sqr_len = sqr_len_max

seuil_free_cell    = 70
seuil_search_scope = 100

direction_x, direction_y = None, None
to_draw_rect = None

cur_game_round  = None
prev_game_round = None

stay_bool = True

def norm_x ( x ):
    res = max( x, 0 )
    res = min( res, x_max )
    return res

def norm_y ( y ):
    res = max( y, 0 )
    res = min( res, y_max )
    return res


def get_correct_order( x0, y0, x1, y1 ):
    l_x0 = x0
    l_y0 = y0
    l_x1 = x1
    l_y1 = y1

    if ( x0 < x1 ) :
        l_x0, l_x1 = x0, x1
    else :
        l_x0, l_x1 = x1, x0

    if ( y0 < y1 ):
        l_y0, l_y1 = y0, y1
    else:
        l_y0, l_y1 = y1, y0

    return l_x0, l_y0, l_x1, l_y1


def mine_line( g, x0, y0, x1, y1, v = False ):
    l_x0, l_y0, l_x1, l_y1 = get_correct_order( x0, y0, x1, y1 )

    if ( l_x0 == l_x1 ):
        if ( v ):
            # print >> sys.stderr, "mine_line : ", g[ y0 : y1 + 1 ][ x0 ]
            print >> sys.stderr, "mine_line : ", 
            for y in range( l_y0, l_y1 + 1 ):
                print >> sys.stderr, g[ y ][ l_x0 ], 

        for y in range( l_y0, l_y1 + 1 ):
            if ( not g[ y ][ l_x0 ] in [ "0" ] ):
                return False

    if ( l_y0 == l_y1 ):
        if ( v ):
            print >> sys.stderr, "mine_line : ", g[ l_y0 ][ l_x0 : l_x1 + 1 ]

        for x in range( l_x0, l_x1 + 1 ):
            if ( not g[ l_y0 ][ x ] in [ "0" ] ):
                res = False
                return False

    return True

def draw_line( g, x0, y0, x1, y1 ):
    l_x0, l_y0, l_x1, l_y1 = get_correct_order( x0, y0, x1, y1 )

    if ( l_x0 == l_x1 ):
        for y in range( l_y0, l_y1 + 1 ):
            if ( not g[ y ][ l_x0 ] in [ "0" ] ):
                return l_x0, y

    if ( l_y0 == l_y1 ):
        for x in range( l_x0, l_x1 + 1 ):
            if ( not g[ l_y0 ][ x ] in [ "0" ] ):
                return x, l_y0


def display_grille( g, x0, y0, x1, y1 ):
    for y in range( y0, y1 + 1 ) :
        print >> sys.stderr, g[y][ x0 : x1 + 1 ]


def on_the_path( cur_x, cur_y, x0, y0, x1, y1 ):
    l_x0, l_y0, l_x1, l_y1 = get_correct_order( x0, y0, x1, y1 )

    for y in range( l_y0, l_y1 + 1 ):
        if ( ( cur_x, cur_y ) == ( x0, y ) ) :
            return True

    for y in range( l_y0, l_y1 + 1 ):
        if ( ( cur_x, cur_y ) == ( x1, y ) ):
            return True

    for x in range( l_x0, l_x1 + 1 ):
        if ( ( cur_x, cur_y ) == ( x, y0 ) ):
            return True

    for x in range( l_x0, l_x1 + 1 ):
        if ( ( cur_x, cur_y ) == ( x, y1 ) ):
            return True

    return False


def nearest_point( cur_x, cur_y, x0, y0, x1, y1, g ):
    l_x0, l_y0, l_x1, l_y1 = get_correct_order( x0, y0, x1, y1 )

    res_pt_x, res_pt_y = None, None
    dist = 1000000

    for y in range( l_y0, l_y1 + 1 ):
        if ( g[ y ][ x0 ] != "." ):
            continue

        l_dist = dist_point( cur_x, cur_y , x0, y )
        if ( l_dist < dist ) :
            dist = l_dist
            res_pt_x, res_pt_y = x0, y

    for y in range( l_y0, l_y1 + 1 ):
        if ( g[ y ][ x1 ] != "." ):
            continue

        l_dist = dist_point( cur_x, cur_y , x1, y )
        if ( l_dist < dist ) :
            dist = l_dist
            res_pt_x, res_pt_y = x1, y

    for x in range( l_x0, l_x1 + 1 ):
        if ( g[ y0 ][ x ] != "." ):
            continue

        l_dist = dist_point( cur_x, cur_y , x, y0 )
        if ( l_dist < dist ) :
            dist = l_dist
            res_pt_x, res_pt_y = x, y0

    for x in range( l_x0, l_x1 + 1 ):
        if ( g[ y1 ][ x ] != "." ):
            continue

        l_dist = dist_point( cur_x, cur_y , x, y1 )
        if ( l_dist < dist ) :
            dist = l_dist
            res_pt_x, res_pt_y = x, y1

    return res_pt_x, res_pt_y


def nearest_point_opponent( op_x, op_y, x0, y0, x1, y1, g ):
    l_x0, l_y0, l_x1, l_y1 = get_correct_order( x0, y0, x1, y1 )

    res_pt_x, res_pt_y = None, None
    dist = 1000000

    for y in range( l_y0, l_y1 + 1 ):
        if ( not g[ y ][ x0 ] in [ ".", "0" ] ):
            continue

        l_dist = dist_point( op_x, op_y , x0, y )
        if ( l_dist < dist ) :
            dist = l_dist
            res_pt_x, res_pt_y = x0, y

    for y in range( l_y0, l_y1 + 1 ):
        if ( not g[ y ][ x1 ] in [ ".", "0" ] ):
            continue

        l_dist = dist_point( op_x, op_y , x1, y )
        if ( l_dist < dist ) :
            dist = l_dist
            res_pt_x, res_pt_y = x1, y

    for x in range( l_x0, l_x1 + 1 ):
        if ( not g[ y0 ][ x ] in [ ".", "0" ] ):
            continue

        l_dist = dist_point( op_x, op_y , x, y0 )
        if ( l_dist < dist ) :
            dist = l_dist
            res_pt_x, res_pt_y = x, y0

    for x in range( l_x0, l_x1 + 1 ):
        if ( not g[ y1 ][ x ] in [ ".", "0" ] ):
            continue

        l_dist = dist_point( op_x, op_y , x, y1 )
        if ( l_dist < dist ) :
            dist = l_dist
            res_pt_x, res_pt_y = x, y1

    if ( res_pt_x, res_pt_y == None, None ):
        print >> sys.stderr, "near opponent:", l_x0, l_y0, l_x1, l_y1

    return res_pt_x, res_pt_y


def draw_rectangle_2( cur_x, cur_y, x0, y0, x1, y1, g ):
    global direction_x, direction_y

    x0 = norm_x( x0 )
    x1 = norm_x( x1 )
    y0 = norm_x( y0 )
    y1 = norm_x( y1 )

    if ( direction_x, direction_y ) == ( cur_x, cur_y ) :
        direction_x, direction_y == None, None

    if ( ( direction_x, direction_y ) != ( None, None ) ) and \
            ( ( direction_x, direction_y ) != ( cur_x, cur_y ) )  :
        print >> sys.stderr, "1: direction_x, direction_y", direction_x, direction_y
        return direction_x, direction_y

    if ( not on_the_path( cur_x, cur_y, x0, y0, x1, y1 ) ):
        if ( ( direction_x, direction_y ) == ( None, None ) ):
            direction_x, direction_y = nearest_point( cur_x, cur_y, x0, y0, x1, y1, g )

        print >> sys.stderr, "2: direction_x, direction_y", direction_x, direction_y
        return direction_x, direction_y
            
        # return nearest_point( cur_x, cur_y, x0, y0, x1, y1, g )
    else :
        direction_x, direction_y = None, None
        if ( on_the_path( cur_x, cur_y - 1, x0, y0, x1, y1 ) and 
             ( g[cur_y - 1][cur_x] != "0") ):
            return cur_x, cur_y - 1
        elif ( on_the_path( cur_x - 1, cur_y, x0, y0, x1, y1 ) and 
             ( g[cur_y][cur_x - 1] != "0") ):
            return cur_x - 1, cur_y
        elif ( on_the_path( cur_x, cur_y + 1, x0, y0, x1, y1 ) and 
             ( g[cur_y + 1][cur_x] != "0") ):
            return cur_x, cur_y + 1
        elif ( on_the_path( cur_x + 1, cur_y, x0, y0, x1, y1 ) and 
             ( g[cur_y][cur_x + 1] != "0") ):
            return cur_x + 1, cur_y

        else :
            return ( cur_x + 1 ) % x_len, ( cur_y + 1 ) % y_len


def mine_rectangle( g, x0, y0, x1, y1, v = False ):
    res = True
    for y in range( y0, y1 + 1 ):
        if ( v ):
            print >> sys.stderr, "mine: g[",y, "]", g[y][ x0 : x1 + 1 ]

        for x in range( x0, x1 + 1 ):
            if ( not g[ y ][ x ] in [ "0" ] ):
                res = False
            if ( not res ):
                break

    return res


def still_free_rectangle( g, x0, y0, x1, y1, v = False ):
    res = True
    for y in range( y0, y1 + 1 ):
        if ( v ):
            print >> sys.stderr, "still: g[", y, "]", g[y][ x0 : x1 + 1 ]

        for x in range( x0, x1 + 1 ):
            if ( not g[ y ][ x ] in [ ".", "0" ] ):
                res = False
            if ( not res ):
                break

    return res


def dist_point( x0, y0, x1, y1 ):
    # return math.sqrt( ( x0 - x1 ) * ( x0 - x1 ) + ( y0 - y1 ) * ( y0 - y1 ) )
    return abs( x0 - x1 ) + abs( y0 - y1 )


def dist_rectangle( cur_x, cur_y, x0, y0, x1, y1, g ):
    # l_dist = dist_point( cur_x, cur_y, float( x0 + x1 ) / 2, float( y0 + y1 ) / 2 )
    # l_dist = dist_point( cur_x, cur_y, ( x0 + x1 ) / 2, ( y0 + y1 ) / 2 )

    res_pt_x, res_pt_y = nearest_point( cur_x, cur_y, x0, y0, x1, y1, g )
    l_dist             = dist_point( cur_x, cur_y, res_pt_x, res_pt_y )

    return l_dist


def potentiel_rect( x0, y0, x1, y1, g ):
    l_x0, l_y0, l_x1, l_y1 = get_correct_order( x0, y0, x1, y1 )

    count = 0
    for y in range( l_y0, l_y1 + 1 ):
        for x in range( l_x0, l_x1 + 1 ):
            count += ( g[ y ][ x ] == "." ) + float( g[ y ][ x ] == "0" ) * 0.3

    return count


def dist_rectangle_opponent( l_x0, l_y0, l_x1, l_y1, g, opponent_pos ):
    dist_op = 0

    for l_op in opponent_pos:
        op_x, op_y         = l_op[ 0 ], l_op[ 1 ]
        res_pt_x, res_pt_y = nearest_point_opponent( op_x, op_y, 
                                                     x0, y0, x1, y1, g )
        l_dist             = dist_point( op_x, op_y, res_pt_x, res_pt_y )

        dist_op += l_dist

    return dist_op


def get_target_rectangle_3( g, cur_x, cur_y, game_round, opponent_pos ):
    global x0, y0, x1, y1, sqr_len

    big_sqr_len = sqr_len

    print >> sys.stderr, "target_3: sqr_len", sqr_len

    looking_in_x = None
    looking_in_y = None

    if ( game_round < seuil_search_scope ):
        big_sqr_len = sqr_len
        print >> sys.stderr, "target_3: big_sqr_len", big_sqr_len

        x_median = x_max / 2
        y_median = y_max / 2

        if ( cur_x <= x_median ):
            looking_in_x = range( 0, x_median + 1 )
        else:
            looking_in_x = range( x_median + 1, x_max + 1 )
            
        if ( cur_y <= y_median ):
            looking_in_y = range( 0, y_median + 1 )
        else:
            looking_in_y = range( y_median + 1, y_max + 1 )
            
    else:
        looking_in_x = range( 0, x_max + 1 )
        looking_in_y = range( 0, y_max + 1 )

    max_sqr_len_possible = 0
    max_rect = 700

    d_rect = {}
    # for l_sqr_len in range( sqr_len, -1,  -1 ):
    for l_sqr_len in range( big_sqr_len, -1,  -1 ):
        for x in looking_in_x :
            for y in looking_in_y :

                if ( g[ y ][ x ] != "." ):
                    continue

                marge = 4
                
                for rect_width in range( l_sqr_len - marge, l_sqr_len + marge + 1 ):
                    if ( sum( \
                        map( lambda x : not x in [ ".", "0" ], g[ y ][ x : min ( x_max, x + rect_width ) ] ) ) > 0 ):
                        continue

                    # if ( rect_width == l_sqr_len ):
                    #     continue

                    rect_height = 2 * l_sqr_len - rect_width

                    if ( rect_height < 0 ) or ( rect_width < 0 ):
                        continue

                    if ( ( x + rect_width > x_max ) or ( y + rect_height > y_max ) ):
                        continue

                    l_x0 = norm_x( x )
                    l_y0 = norm_y( y )
                    l_x1 = norm_x( x + rect_width )
                    l_y1 = norm_y( y + rect_height )
        
                    if ( still_free_rectangle( g, l_x0, l_y0, l_x1, l_y1 ) and \
                        not mine_rectangle( g, l_x0, l_y0, l_x1, l_y1 ) ) :

                        max_sqr_len_possible = max( l_sqr_len, max_sqr_len_possible )

                        pot  = potentiel_rect( l_x0, l_y0, l_x1, l_y1, g )
                        size = l_sqr_len
                        dist = dist_rectangle( cur_x, cur_y, l_x0, l_y0, l_x1, l_y1, g )
                        dist_op = dist_rectangle_opponent( l_x0, l_y0, l_x1, l_y1, g, opponent_pos )

#                        d_rect[ (l_x0, l_y0, l_x1, l_y1) ] = ( pot + size * 0.4, -dist )
#                        d_rect[ (l_x0, l_y0, l_x1, l_y1) ] = ( pot + size * 0.4 - 0.40 * dist )
                        # d_rect[ (l_x0, l_y0, l_x1, l_y1) ] = ( pot + size * 0.4 - dist )
                        d_rect[ (l_x0, l_y0, l_x1, l_y1) ] = ( pot + size * 0.4 - dist + dist_op )

                if ( len ( d_rect.keys() ) > max_rect ):
                    break

            if ( len ( d_rect.keys() ) > max_rect ):
                break
        
        if ( len ( d_rect.keys() ) > max_rect ):
            break

    sqr_len = min( max_sqr_len_possible, sqr_len )

    all_rect = []
    for k, v in d_rect.items():
        all_rect.append( ( v, k ) )

    all_rect.sort( reverse=True )

    if ( all_rect ):
        print >> sys.stderr, "target_3: k, v : ", all_rect[ 0 ][ 0 ], all_rect[ 0 ][ 1 ]
        rect_target = all_rect[ 0 ][ 1 ]
        return rect_target
    else :
        print >> sys.stderr, "target_3: no rectangle !"
        return ( ( cur_x ) % x_len, ( cur_y ) % y_len, ( cur_x + 1 ) % x_len, ( cur_y + 1 ) % y_len )


def count_nb_free_cell( g ):
    count = 0

    for y in range( y_len ):
        for x in range( x_len ):
            count += ( g[y][x] == "." )

    return count

def closer_free_cell( cur_x, cur_y, g ):
    
    dist      = 10000000
    closer_pt = None
    
    for y in range( y_len ):
        for x in range( x_len ):
            if ( g[y][x] == "." ):
                l_dist = dist_point( cur_x, cur_y, x, y )
                if ( l_dist < dist ):
                    dist = l_dist
                    closer_pt = ( x, y )
                    
    return closer_pt


# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

opponent_count = int(raw_input()) # Opponent count
opponent_pos   = [ None ] * opponent_count

x0, y0 = 0, 0
x1, y1 = sqr_len, sqr_len

# game loop
while 1:
    game_round = int(raw_input())

    prev_game_round = cur_game_round
    cur_game_round  = game_round

    # print >> sys.stderr, "main: cur_game_round", cur_game_round
    # print >> sys.stderr, "main: prev_game_round", prev_game_round
    
    print >> sys.stderr, "main: game_round", game_round

    if ( game_round == 1 ):
        if ( opponent_count == 3 ):
            sqr_len            = 8
            seuil_free_cell    = 80
            seuil_search_scope = 60

        elif ( opponent_count == 2 ):
            sqr_len            = 10
            seuil_free_cell    = 60
            seuil_search_scope = 80

        elif ( opponent_count == 1 ):
            sqr_len            = 12
            seuil_free_cell    = 30
            seuil_search_scope = 100

     # x: Your x position
     # y: Your y position
     # back_in_time_left: Remaining back in time
    cur_x, cur_y, back_in_time_left = [int(i) for i in raw_input().split()]
    # print >> sys.stderr, "cur_x, cur_y, back_in_time_left", cur_x, cur_y, back_in_time_left

    for i in xrange(opponent_count):
         # opponent_x: X position of the opponent
         # opponent_y: Y position of the opponent
         # opponent_back_in_time_left: Remaining back in time of the opponent
        opponent_x, opponent_y, opponent_back_in_time_left = [int(j) for j in raw_input().split()]
        opponent_pos[ i ] = ( opponent_x, opponent_y, opponent_back_in_time_left )

    grille = []
    for i in xrange(20):
        line = raw_input() # One line of the map ('.' = free, '0' = you, otherwise the id of the opponent)
        grille.append( line )

    target_x, target_y = cur_x, cur_y

    if ( cur_game_round < prev_game_round ) :
        to_draw_rect = None
        # continue

    # if ( ( cur_game_round != None ) and ( prev_game_round != None ) ):
    #     if ( cur_game_round < prev_game_round ) :
    #         if ( back_in_time_left > 0 ) :
    #             # print "BACK", back_in_time_left
    #             print "BACK", 25
    #             to_draw_rect = None
    #             continue
    #         else :
    #             to_draw_rect = None

    nb_free_cell = count_nb_free_cell( grille )
    print >> sys.stderr, "nb_free_cell:", nb_free_cell

    if ( nb_free_cell == 0 ):
        print >> sys.stderr, "NO FREE CELL !"
        print target_x, target_y

    elif ( 0 < nb_free_cell ) and ( nb_free_cell <= seuil_free_cell ):

        target_x, target_y = closer_free_cell( cur_x, cur_y, grille )
            
        if ( ( ( target_x, target_y ) == ( cur_x, cur_y ) ) and 
               stay_bool ) :
            stay_bool = not stay_bool
            print cur_x, cur_y
            
        elif ( ( ( target_x, target_y ) == ( cur_x, cur_y ) ) and 
               not stay_bool ) :
            stay_bool = not stay_bool
            print (target_x + 1) % x_len, (target_y + 1) % y_len

        else:
            print target_x, target_y
    
    else: 
        if ( to_draw_rect ):
            if ( mine_rectangle( grille, *to_draw_rect ) ):
                to_draw_rect = None

        if ( to_draw_rect and not still_free_rectangle( grille, *to_draw_rect ) ):
            to_draw_rect = None

        if ( not to_draw_rect ):
            direction_x, direction_y = None, None
            # to_draw_rect = get_target_rectangle_2( grille, cur_x, cur_y )
            to_draw_rect = get_target_rectangle_3( grille, 
                                                   cur_x, cur_y, 
                                                   game_round, 
                                                   opponent_pos )

        x0, y0, x1, y1 = to_draw_rect
        print >> sys.stderr, "main: x0, y0, x1, y1", x0, y0, x1, y1
        print >> sys.stderr, "main: sqr_len", sqr_len

        target_x, target_y = draw_rectangle_2( cur_x, cur_y, 
                                               x0, y0, x1, y1, grille )

        # action: "x y" to move or "BACK rounds" to go back in time
        # print "17 10"
        # print "0 0"
        # print min( target_x + 1, 34 ), min( target_y + 1, 19 )

        if ( (cur_x, cur_y) == (target_x, target_y) ):
            print (target_x + 1) % x_len, (target_y + 1) % y_len
        else:
            print target_x, target_y

    # if ( game_round == 100 ):
    #     if ( back_in_time_left > 0 ):
    #         print "BACK 25"
