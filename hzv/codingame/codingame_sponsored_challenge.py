from sys import stderr
from operator import itemgetter

import copy as cp

# x_max = int(raw_input())
# y_max = int(raw_input())

y_max = int(raw_input())
x_max = int(raw_input())
nb_pt = int(raw_input())

x_max, y_max = x_max - 1, y_max - 1

# print >> stderr, "x_max", x_max
# print >> stderr, "y_max", y_max
# print >> stderr, "nb_pt", nb_pt

class Pt( object ) :

    def __init__( self, x, y ):
        self.x = x
        self.y = y


    def __sub__( self, pt ):
        return Pt( self.x - pt.x, self.y - pt.y )


    def __str__( self ):
        return "(%d, %d)" % ( self.x, self.y )


    def __eq__( self, pt ):
        return ( ( self.x == pt.x ) and ( self.y == pt.y ) )


def dist( pt0, pt1 ) :
    return abs( pt0.x - pt1.x ) + abs ( pt0.y - pt1.y )


def update_grid( grid, l_pt, right_obs, up_obs, left_obs, down_obs ) :
    for pt in l_pt :
        x, y = pt.x, pt.y
        grid[ y ][ x ] = 0

    my_pt = l_pt[ -1 ]

    if ( 0 <= my_pt.y -1 ) :
        if ( down_obs ) :
            grid[ my_pt.y -1 ][ my_pt.x ] = 1
        else :
            grid[ my_pt.y -1 ][ my_pt.x ] = 0
        
    if ( my_pt.x +1 < x_max ) :
        if ( right_obs ) :
            grid[ my_pt.y ][ my_pt.x +1 ] = 1
        else :
            grid[ my_pt.y ][ my_pt.x +1 ] = 0

    if ( my_pt.y +1 < y_max ) :
        if ( up_obs ) :
            grid[ my_pt.y +1 ][ my_pt.x ] = 1
        else :
            grid[ my_pt.y +1 ][ my_pt.x ] = 0

    if ( 0 <= my_pt.x -1 ) :
        if ( left_obs ) :
            grid[ my_pt.y ][ my_pt.x -1 ] = 1
        else :
            grid[ my_pt.y ][ my_pt.x -1 ] = 0


def l_str( l ):
    return ' '.join( map( lambda x : str( x ), l ) )


def str_grid_line( l ) :
    res = ""
    for nb in l :
        if ( nb == -1 ) :
            res += "?"
        if ( nb == 0 ) :
            res += "."
        if ( nb == 1 ) :
            res += "#"
        if ( nb == 10 ) :
            res += "X"
        if ( nb == 100 ) :
            res += "Z"

    return res


def print_grid( grid, l_pt ) :
    str_grid = cp.deepcopy( grid )

    my_pt = get_my_pt( l_pt )

    str_grid[ my_pt.y ][ my_pt.x ] = 100
    for pt in l_pt[ : -1 ] :
        str_grid[ pt.y ][ pt.x ] = 10
    
    print >> stderr, ' ', ''.join( map( lambda x : str( x % 10 ), range( x_max ) ) )
    for i in xrange( y_max ) :
        print >> stderr, i % 10, str_grid_line( str_grid[ i ] )
    print >> stderr, ' ', ''.join( map( lambda x : str( x % 10 ), range( x_max ) ) )


def get_my_pt( l_pt ) :
    return l_pt[ -1 ]

def get_nearest_pt( l_pt ) :
    l_pt = l_pt[ : ]
    my_pt = get_my_pt( l_pt )
    # print >> stderr, "my_pt", my_pt

    l_pt = l_pt[ : -1 ]
    l_pt.sort( key = lambda pt : dist( my_pt, pt ) )

    nearest_pt = l_pt[ 0 ]
    # print >> stderr, "nearest_pt", nearest_pt

    return nearest_pt


def possible_pt( x, y ) :
    if ( ( 0 <= x ) and ( x < x_max ) and ( 0 <= y ) and ( y < y_max ) ) :
        return Pt( x, y )
    else :
        return None


def get_neighbour( grid, pt ):
    res = []
    
    down_pt  = pt.x, pt.y -1
    right_pt = pt.x +1, pt.y
    up_pt    = pt.x, pt.y +1
    left_pt  = pt.x -1, pt.y

    for x, y in [ down_pt, right_pt, up_pt, left_pt ] :
        pt = possible_pt( x, y )
        if ( pt is not None ) :
            res.append( pt )
            
    return res


def explore( grid, l_pt ) :

    # print >> stderr, "l_pt", \
    #     ' | '.join( map( lambda pt : str( pt ),
    #                      l_pt ) )

    my_pt = get_my_pt( l_pt )
    l_opponent_pt = l_pt[ : -1 ]

    # print >> stderr, "l_opponent_pt", \
    #     ' | '.join( map( lambda pt : str( pt ),
    #                      l_opponent_pt ) )

    l_near_unknown = []
    l_next_pt = [ ( my_pt, None, 0 ) ]
    l_already = []
    # min_dist  = None

    l_reachable_opponent = []

    # while( ( l_next_pt ) and ( len( l_near_unknown ) < 8 ) ) :
    while( l_next_pt ) :
        cur_pt, neighbour_pt, path_len = l_next_pt.pop( 0 )
        l_already.append( cur_pt )
        # print >> stderr, "cur_pt", cur_pt        
        # print >> stderr, "l_already", l_str( l_already )

        # if ( min_dist is not None ) :
        #     if ( min_dist < dist( my_pt, cur_pt ) ) :
        #         continue

        tmp_l_op_pt = map( lambda op_pt : ( dist( cur_pt, op_pt ), op_pt ), \
                           l_opponent_pt )
        tmp_l_op_pt.sort( )

        # if ( tmp_l_op_pt[ 0 ][ 0 ] == 1 ) :
        #     if ( l_reachable_opponent is None ) :
        #         if ( neighbour_pt is None ) :
        #             neighbour_pt = tmp_l_op_pt[ 0 ][ 1 ]
        #         l_reachable_opponent.append( ( tmp_l_op_pt[ 0 ][ 1 ],
        #                                      neighbour_pt,
        #                                      path_len ) )

        for couple in tmp_l_op_pt :
            distance, op_pt = couple
            if ( op_pt not in map( itemgetter( 0 ), l_reachable_opponent ) ) :
                if ( distance == 1 ) :
                    if ( neighbour_pt is None ) :
                        l_reachable_opponent.append( ( op_pt, 
                                                       op_pt,
                                                       path_len + 1 ) )
                    else :
                        l_reachable_opponent.append( ( op_pt, 
                                                       neighbour_pt,
                                                       path_len + 1 ) )

        l_neighbour = get_neighbour( grid, cur_pt )

        # print >> stderr, "l_neighbour", l_str( l_neighbour )

        for pt in l_neighbour :

            # Abort if the neighbour pt is the position of a opponent pt
            if ( pt in l_opponent_pt ) :
                continue
            
            value = grid[ pt.y ][ pt.x ]

            if ( value == -1 ) :
                # if ( min( map( lambda op_pt : dist( cur_pt, op_pt ), \
                #                l_opponent_pt ) ) < 2 ) :
                #         l_reachable_opponent = True

                # if ( ( ( cur_pt, neighbour_pt ) not in l_near_unknown ) and \
                if ( ( cur_pt not in map( itemgetter( 0 ), l_near_unknown ) ) and \
                     ( min( map( lambda op_pt : dist( cur_pt, op_pt ), \
                                 l_opponent_pt ) ) >= 2 ) \
                     # and \
                     # ( reduce( lambda x, y : x or y, \
                     #           map( lambda op_pt : ( op_pt.x == cur_pt.x ) or ( op_pt.y == cur_pt.y ), \
                     #                                 l_opponent_pt ) ) )
                ) :

                    l_near_unknown.append( ( cur_pt, neighbour_pt, path_len ) )

                # if ( min_dist is None ) :
                #     min_dist = dist( my_pt, cur_pt )
                # else :
                #     min_dist = min( min_dist, dist( my_pt, cur_pt ) )

            elif ( ( value == 0 ) and ( pt not in l_already ) ) :
                if ( neighbour_pt is None ) :
                    l_next_pt.append( ( pt, pt, path_len + 1 ) )
                else :
                    l_next_pt.append( ( pt, neighbour_pt, path_len + 1 ) )

        # print >> stderr, "l_next_pt", l_str( l_next_pt )

    # l_near_unknown.sort( key = lambda c : c[ 1 ].x, reverse = True )
    # l_near_unknown.sort( key = lambda c : dist( my_pt, c[ 0 ] ) )

    l_near_unknown.sort( key = itemgetter( 2 ) )

    # l_near_unknown.sort( key = lambda c : sum( map( \
    #                                 lambda op_pt : dist( op_pt, c[ 0 ] ),
    #                                 l_opponent_pt ) ),
    #                      reverse = True )

    l_near_unknown.sort( key = lambda c : min( map( \
                                    lambda op_pt : dist( op_pt, c[ 0 ] ),
                                                    l_opponent_pt ) ) * ( 1 + 1.0 / c[ 2 ] ) ,
                         reverse = True )

    print >> stderr, "l_near_unknown", \
        ' | '.join( map( lambda c : str( c[ 0 ] ) + " " + str( c[ 1 ] ) + " %2.1f" % ( min( map( lambda op_pt : dist( op_pt, c[ 0 ] ), l_opponent_pt ) ) * ( 1 + 1.0 / c[ 2 ] ) ),
                         l_near_unknown[ : 5 ] ) )

    # print >> stderr, "len( l_near_unknown )", len( l_near_unknown )

    if ( l_near_unknown == [] ) :
        return wait, l_reachable_opponent

    final_pt, neighbour_pt, path_len = l_near_unknown[ 0 ]

    # final_pt, neighbour_pt = None, None

    # for f_pt, n_pt in l_near_unknown :
    #     let_continue = False
    #     for op_pt in l_opponent_pt :
    #         if ( dist( n_pt, op_pt ) == 1 ) :
    #             let_continue = True
    #             break

    #     if ( let_continue ) :
    #         continue
    #     else :
    #         final_pt, neighbour_pt = f_pt, n_pt
    #         break

    print >> stderr, "final_pt, neighbour_pt", str( final_pt ), str( neighbour_pt )
    
    if ( ( final_pt is None ) and ( neighbour_pt is  None ) ) :
        return wait, l_reachable_opponent

    d_x, d_y = neighbour_pt.x - my_pt.x, neighbour_pt.y - my_pt.y

    if d_x == 1 :
        return go_right, l_reachable_opponent
    if d_x == -1 :
        return go_left, l_reachable_opponent
    if d_y == 1 :
        return go_up, l_reachable_opponent
    if d_y == -1 :
        return go_down, l_reachable_opponent
    

def run_away( grid, l_pt, l_reachable_opponent ) :
    my_pt = get_my_pt( l_pt )

    l_neighbour = get_neighbour( grid, my_pt )

    # l_neighbour = map( lambda n_pt : ( dist( n_pt, l_reachable_opponent ), n_pt ),
    #                    l_neighbour )

    l_neighbour = filter( lambda pt : grid[ pt.y ][ pt.x ] == 0, l_neighbour )

    l_reachable_opponent.sort( key = itemgetter( 2 ) )
    op_pt, n_pt, path_len = l_reachable_opponent[ 0 ]

    # print >> stderr, "n_pt", str( n_pt )

    # If the opponent is at a distance of 1 from my_pt
    if ( op_pt == n_pt ) :
        l_neighbour.remove( n_pt )
    
    # d_x, d_y = n_pt.x - my_pt.x, n_pt.y - my_pt.y

    # opposite_n_pt = Pt( my_pt.x - d_x, my_pt.y - d_y )

    # print >> stderr, "opposite_n_pt", str( opposite_n_pt )
    
    # d_x, d_y = opposite_n_pt.x - my_pt.x, opposite_n_pt.y - my_pt.y
    # l_neighbour.sort( key = lambda n_pt : dist( n_pt, opposite_n_pt ) )
    l_neighbour.sort( key = lambda pt : dist( pt, n_pt ),
                      reverse = True )

    # print >> stderr, "l_neighbour", l_str( l_neighbour )
    
    next_pt = l_neighbour[ 0 ]

    print >> stderr, "next_pt", str( next_pt )
    
    d_x, d_y = next_pt.x - my_pt.x, next_pt.y - my_pt.y

    # l_neighbour.sort( key = lambda n_pt : dist( n_pt, op_pt ),
    #                   reverse = True )

    # next_pt = l_neighbour[ 0 ]
    
    # d_x, d_y = next_pt.x - my_pt.x, next_pt.y - my_pt.y

    if d_x == 1 :
        return go_right
    if d_x == -1 :
        return go_left
    if d_y == 1 :
        return go_up
    if d_y == -1 :
        return go_down
    
    
def get_output( grid, l_pt ) :
    l_pt_origin = l_pt[ : ]

    # my_pt = l_pt.pop( )
    # print >> stderr, "my_pt", my_pt

    # l_pt.sort( key = lambda pt : dist( my_pt, pt ) )

    # # print >> stderr, "1 l_pt", l_str( l_pt )

    # nearest_pt = l_pt[ 0 ]

    # print >> stderr, "nearest_pt", nearest_pt

    my_pt = get_my_pt( l_pt )
    # nearest_pt = get_nearest_pt( l_pt[ : ] )
    nearest_pt = get_nearest_pt( l_pt )

    disp_vector = nearest_pt - my_pt

    # print >> stderr, "disp_vector", disp_vector

    longer_in_x, longer_in_y = False, False

    if ( abs( disp_vector.x ) > abs( disp_vector.y ) ) :
        longer_in_x = True
    elif ( abs( disp_vector.x ) < abs( disp_vector.y ) ) :
        longer_in_y = True

    x_privilege, y_privilege = False, False

    if ( longer_in_x ) :
        x_privilege = True
    elif ( longer_in_y ) :
        y_privilege = True
    else :
        if ( x_max > y_max ) :
            x_privilege = True
        elif ( x_max < y_max ) :
            y_privilege = True
        else :
            x_privilege = True

    # print >> stderr, "x_privilege, y_privilege", x_privilege, y_privilege

    if ( x_privilege ) :
        if ( ( disp_vector.x > 0 ) and not right_obs ) :
            return go_right
        elif ( ( disp_vector.x < 0 ) and not left_obs ) :
            return go_left
        elif ( ( disp_vector.y > 0 ) and not up_obs ) :
            return go_up
        elif ( ( disp_vector.y < 0 ) and not down_obs ) :
            return go_down
        else :
            if ( disp_vector.y == 0 ) :
                return go_down
            else :
                return wait

    elif( y_privilege ) :
        if ( ( disp_vector.y > 0 ) and not up_obs ) :
            return go_up
        elif ( ( disp_vector.y < 0 ) and not down_obs ) :
            return go_down
        elif ( ( disp_vector.x > 0 ) and not right_obs ) :
            return go_right
        elif ( ( disp_vector.x < 0 ) and not left_obs ) :
            return go_left
        else :
            if ( disp_vector.x == 0 ) :
                return go_left
            else :
                return wait
                

# A : right
# B : nothing
# C : down
# D : up
# E : left

go_right = "A"
go_down  = "C"
go_up    = "D"
go_left  = "E"
wait     = "B"

# -1 : unknown
#  0 : possible
#  1 : wall

grid = [ -1 ] * y_max
for i in xrange( y_max ):
    grid[ i ] = [ -1 ] * x_max

# game loop
while True:
    # first_input = raw_input()   # down
    # second_input = raw_input()  # right
    # third_input = raw_input()   # up
    # fourth_input = raw_input()  # left

    down_obs  = raw_input()  # down
    right_obs = raw_input()  # right
    up_obs    = raw_input()  # up
    left_obs  = raw_input()  # left

    down_obs, right_obs, up_obs, left_obs = \
            tuple( map( lambda x : True if x == "#" else False,
                        [ down_obs, right_obs, up_obs, left_obs ] ) )

    # print >> stderr, "Dir:", first_input, second_input, \
    #     third_input, fourth_input

    # print >> stderr, "obs down  - C", down_obs
    # print >> stderr, "obs right - A", right_obs
    # print >> stderr, "obs up    - D", up_obs
    # print >> stderr, "obs left  - E", left_obs

    l_pt = []
    
    # for i in xrange(third_init_input):
    for i in xrange( nb_pt ):
        x, y = [ int( j ) for j in raw_input().split() ]
        x, y = x - 1, y - 1

        l_pt.append( Pt( x, y ) )

    update_grid( grid, l_pt, right_obs, up_obs, left_obs, down_obs )

    # print >> stderr, "0 l_pt", l_str( l_pt )
    # l_pt_origin = l_pt[ : ]

    # output = get_output( grid, l_pt )

    output, l_reachable_opponent = explore( grid, l_pt )

    if ( l_reachable_opponent != [] ) :
        # print >> stderr, "l_reachable_opponent", l_str( l_reachable_opponent )
        print >> stderr, "l_reachable_opponent\n", \
            ' | '.join( map( lambda c : str( c[ 0 ] ) + " " + str( c[ 1 ] ) + " " + str( c[ 2 ] ),
                             l_reachable_opponent ) )
        output = run_away( grid, l_pt, l_reachable_opponent )
        
    print_grid( grid, l_pt )

    print output
    
