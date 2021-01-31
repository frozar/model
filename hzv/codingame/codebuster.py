import sys
import math

from operator import itemgetter, attrgetter

# Send your busters out into the fog to trap ghosts and bring them home!

busters_per_player = int(raw_input())  # the amount of busters you control
ghost_count = int(raw_input())  # the amount of ghosts on the map
my_team_id = int(raw_input())  # if this is 0, your base is on the top left of the map, if it is one, on the bottom right

# print >> sys.stderr, "busters_per_player", busters_per_player
# print >> sys.stderr, "ghost_count", ghost_count
# print >> sys.stderr, "my_team_id", my_team_id

opponent_team_id = ( my_team_id + 1 ) % 2

class Entity( object ):

    def __init__( self, entity_id, x, y, entity_type, state, value, 
                  possible_position = None, discovery = "NEW" ): 
        self.entity_id         = entity_id
        self.x                 = x
        self.y                 = y
        self.entity_type       = entity_type
        self.state             = state
        self.value             = value
        self.possible_position = possible_position
        # 5 values for the discovery:
        #   "NEW"
        #   "UPDATED"
        #   "DEDUCED"
        #   "NOT_FOUND"
        #   "DROP"
        self.discovery         = discovery

    def __str__( self ):
        return " ".join( [ str( self.entity_id ),
                           str( self.x ),
                           str( self.y ),
                           str( self.entity_type),
                           str( self.state ),
                           str( self.value ), 
                           self.discovery ] )

    def __eq__( self, other ):
        return ( ( self.entity_id == other.entity_id ) and \
                 ( self.x == other.x ) and \
                 ( self.y == other.y ) and \
                 ( self.entity_type == other.entity_type ) and \
                 ( self.state == other.state ) and \
                 ( self.value == other.value ) )


def polar_to_cart( alpha, len_move ) :
    delta_x = len_move * math.cos( alpha )
    delta_y = len_move * math.sin( alpha )
    return delta_x, delta_y


def l_str_entity( l ):
    return [ str( b ) for b in l ]


def get_action_ind( buster ):
    return buster.entity_id % busters_per_player


def get_my_buster( l_entity ):
    res = []
    for entity in l_entity :
        if ( entity.entity_type == my_team_id ) :
            res.append( entity )

    return res


def get_opponent_buster( l_entity ):
    res = []
    for entity in l_entity :
        if ( entity.entity_type == opponent_team_id ) :
            res.append( entity )

    return res


def get_ghost( l_entity ):
    res = []
    for entity in l_entity :
        if ( entity.entity_type == -1 ) :
            res.append( entity )

    return res


def get_busy_buster( l_entity ):

    l_buster = get_my_buster( l_entity )
    
    res = []
    for buster in l_buster :
        if ( buster.state == 1 ) :
            res.append( buster )

    return res


def get_buster_by_id( id_buster, l_buster ):
    res = None
    for buster in l_buster :
        if ( ( buster.entity_id % busters_per_player ) == id_buster ):
            res = buster
            break
    return res


def get_available_buster( l_buster ):
    return filter( lambda b : b.state == 0, l_buster )


def get_base_position( ):
    if ( my_team_id == 0 ):
        return ( 0, 0 )        
    else :
        return ( 16000, 9000 )


def get_opponent_base_position( ):
    if ( my_team_id == 0 ):
        return ( 16000, 9000 )
    else :
        return ( 0, 0 )        


base_position = get_base_position( )
opponent_base_position = get_opponent_base_position( )


# def get_next_target_position( position ):
def get_next_target_position( ind_action, ind_position, possible_position ):

    # ind_position = possible_position.index( position )
    # ind_position = ( ind_position + 1 ) % len( possible_position )

    # return possible_position[ ind_position ]

    # ind_position = possible_position.index( position )
    ind_position                       = ( ind_position + 1 ) % len( possible_position )
    current_ind_position[ ind_action ] = ind_position

    return possible_position[ ind_position ]


def stun( l_entity, reload_stun, l_action ):
# def stun( l_entity, l_action ):

    l_buster = get_my_buster( l_entity )

    # print >> sys.stderr, "AVANT l_buster", "\n", l_str_entity( l_buster )
    # Delete STUNed buster
    l_buster = filter( lambda b : b.state != 2, l_buster )

    print >> sys.stderr, "reload_stun", reload_stun

    # print >> sys.stderr, "APRE l_buster", "\n", l_str_entity( l_buster )

    # # Consider only buster which can stun someone
    # l_buster = filter( lambda b : reload_stun[ get_action_ind( b ) ] == 0, 
    #                    l_buster )
    # # l_buster = filter( lambda b : b.reload_stun == 0, 
    # #                    l_buster )

    l_opponent_buster = get_opponent_buster( l_entity )

    # l_opponent_buster.sort( key = lambda buster : buster.state, reverse = True )

    # print >> sys.stderr, "l_opponent_buster", l_str_entity( l_opponent_buster )

    if ( l_opponent_buster == [] ) :
        return

    # If the opponent is not stuned
    # Sort by hand
    l_opponent_buster_not_stuned = []
    for i in [ 1, 3, 0 ]:
        for buster in l_opponent_buster:
            if ( buster.state == i ) :
                l_opponent_buster_not_stuned.append( buster )
    
    # print >> sys.stderr, "l_opponent_buster_not_stuned", "\n", \
    #     "\n".join( l_str_entity( l_opponent_buster_not_stuned ) )

    if ( l_opponent_buster_not_stuned == [] ) :
        return

    # wk_l_buster = l_buster[ : ]

    # # Don't use useless buster
    # for buster in wk_l_buster :
    #     action_ind = get_action_ind( buster )
    #     if ( 9 < reload_stun[ action_ind ] ) :
    #         wk_l_buster.remove( buster )

    # TODO: compute of direction to follow when a opponent buster carried a ghost
    for op_buster in l_opponent_buster_not_stuned :

        wk_l_buster = l_buster[ : ]

        # For 'op_buster', compute the number of step to reach his base
        dist_buster_to_his_base = dist( ( op_buster.x, op_buster.y ), opponent_base_position )
        dist_buster_to_release  = dist_buster_to_his_base - 1600
        nb_step_to_release      = int( dist_buster_to_release / 800 )

        # Don't use useless buster
        for buster in wk_l_buster :
            action_ind = get_action_ind( buster )
            if ( nb_step_to_release < reload_stun[ action_ind ] ) :
                wk_l_buster.remove( buster )

        # print >> sys.stderr, "wk_l_buster", "\n", l_str_entity( wk_l_buster )

        # Available: catching a ghost or idle
        l_available_buster = []
        # Busy     : carrying ghost
        l_busy_buster      = []

        # for buster in l_buster_near_opponent :
        for buster in wk_l_buster :
            # if ( ( buster.state == 1 ) or ( buster.state == 3 ) ) :
            if ( buster.state == 1 ) :
                l_busy_buster.append( buster )
            # elif ( buster.state == 0 ) :
            elif ( ( buster.state == 0 ) or ( buster.state == 3 ) ):
                l_available_buster.append( buster )

        if ( ( l_available_buster == [] ) and ( l_busy_buster == [] ) ):
            # continue
            return

        p_op_buster = ( op_buster.x, op_buster.y )

        l_available_buster.sort( key = lambda b : dist( ( b.x, b.y ), p_op_buster ) )
        l_busy_buster.sort( key = lambda b : dist( ( b.x, b.y ), p_op_buster ) )

        print >> sys.stderr, "l_busy_buster", "\n", l_str_entity( l_busy_buster )
        print >> sys.stderr, "l_available_buster", "\n", l_str_entity( l_available_buster )

        buster_is_available = False
        buster_is_busy      = False
        first_buster        = None
        action_ind          = None

        # print >> sys.stderr, "dist( p_op_buster, base_position ) < 1600 * 2", dist( p_op_buster, base_position ) < 1600 * 2
        # print >> sys.stderr, "filter( lambda b : dist( ( b.x, b.y ), p_op_buster ) < 1760, l_busy_buster )", l_str_entity( filter( lambda b : dist( ( b.x, b.y ), p_op_buster ) < 1760, l_busy_buster ) )

        if ( ( dist( p_op_buster, base_position ) < 1600 * 2 ) and \
             ( filter( lambda b : dist( ( b.x, b.y ), p_op_buster ) < 1760, l_busy_buster ) ) ) :
            buster_is_busy = False
            first_buster   = filter( lambda b : dist( ( b.x, b.y ), p_op_buster ) < 1760, 
                                     l_busy_buster )[ 0 ]
            action_ind = get_action_ind( first_buster )
            # print >> sys.stderr, "action_ind", action_ind

        else :

            for buster in l_available_buster :
                action_ind = get_action_ind( buster )
                if ( l_action[ action_ind ] is not None ):
                    continue
                buster_is_available = True
                first_buster        = l_available_buster[ 0 ]

            if ( not buster_is_available ):
                for buster in l_busy_buster :
                    action_ind = get_action_ind( buster )
                    if ( l_action[ action_ind ] is not None ):
                        continue
                    buster_is_busy = True
                    first_buster   = l_busy_buster[ 0 ]

        if ( first_buster is None ) :
            continue

        print >> sys.stderr, "first_buster", str( first_buster )
        
        action_ind = get_action_ind( first_buster )

        p_first_buster = ( first_buster.x, first_buster.y )

        # print >> sys.stderr, "dist( p_first_buster, p_op_buster ) < 1760", dist( p_first_buster, p_op_buster ) < 1760
        # print >> sys.stderr, "reload_stun[ action_ind ] == 0", reload_stun[ action_ind ] == 0

        # if ( ( dist( p_first_buster, p_op_buster ) < 1760 ) and ( reload_stun[ action_ind ] == 0 ) ) :
        #     # print >> sys.stderr, " FROZAR OK OK"
        #     l_action[ action_ind ] = "STUN " + str( op_buster.entity_id ) + \
        #                              " STUN IT"
        #     reload_stun[ action_ind ] = 20
        #     # print >> sys.stderr, "STUN l_action", l_action

        # else :

        #     # print >> sys.stderr, "( buster_is_available ) and ( reload_stun[ action_ind ] == 0 )", ( buster_is_available ) and ( reload_stun[ action_ind ] == 0 )
        #     # print >> sys.stderr, "( op_buster.state == 1 )", ( op_buster.state == 1 )

        #     # if ( buster_is_available ) :
        #     if ( ( buster_is_available ) and ( reload_stun[ action_ind ] == 0 ) ) :
        #         l_action[ action_ind ] = "MOVE " + \
        #                                  str( op_buster.x ) + " " + str( op_buster.y ) + \
        #                                  " STUN " + str( op_buster.entity_id )

        #     # elif ( ( buster_is_available ) and ( op_buster.state == 1 ) ) :
        #     elif ( op_buster.state == 1 ) :
        #         l_action[ action_ind ] = "MOVE " + \
        #                                  str( op_buster.x ) + " " + str( op_buster.y ) + \
        #                                  " STUN " + str( op_buster.entity_id )
        #         # print >> sys.stderr, "GOOD l_action", l_action

        if ( op_buster.state == 1 ) :
            if ( dist( p_first_buster, p_op_buster ) < 1760 ) :
                l_action[ action_ind ] = "STUN " + str( op_buster.entity_id ) + \
                                         " STUN IT"
                reload_stun[ action_ind ] = 20
            else :
                l_action[ action_ind ] = "MOVE " + \
                                         str( op_buster.x ) + " " + str( op_buster.y ) + \
                                         " STUN " + str( op_buster.entity_id )

        elif ( ( buster_is_available ) and ( reload_stun[ action_ind ] == 0 ) ) :
            if ( dist( p_first_buster, p_op_buster ) < 1760 ) :
                l_action[ action_ind ] = "STUN " + str( op_buster.entity_id ) + \
                                         " STUN IT"
                reload_stun[ action_ind ] = 20
            else :
                l_action[ action_ind ] = "MOVE " + \
                                         str( op_buster.x ) + " " + str( op_buster.y ) + \
                                         " STUN " + str( op_buster.entity_id )

        elif ( ( dist( p_first_buster, p_op_buster ) < 1760 ) and ( reload_stun[ action_ind ] == 0 ) ) :
            # print >> sys.stderr, " FROZAR OK OK"
            l_action[ action_ind ] = "STUN " + str( op_buster.entity_id ) + \
                                     " STUN IT"
            reload_stun[ action_ind ] = 20
            # print >> sys.stderr, "STUN l_action", l_action

        # elif ( ( buster_is_available ) and ( op_buster.state == 1 ) ) :
            # print >> sys.stderr, "GOOD l_action", l_action

        # print >> sys.stderr, "( buster_is_available ) and ( reload_stun[ action_ind ] == 0 )", ( buster_is_available ) and ( reload_stun[ action_ind ] == 0 )
        # print >> sys.stderr, "( op_buster.state == 1 )", ( op_buster.state == 1 )

        print >> sys.stderr, "l_action", l_action

        l_buster.remove( first_buster )


def go_to_base( l_entity, l_action, d_known_ghost, l_ghost_already_released ):
    
    # print >> sys.stderr, "l_entity", l_str_entity( l_entity )
    l_busy_buster = get_busy_buster( l_entity )
    # print >> sys.stderr, "l_busy_buster", l_str_entity( l_busy_buster )

    if ( l_busy_buster == [] ):
        # print >> sys.stderr, "   NO BUSY BUSTER!"
        return

    for buster in l_busy_buster :
        action_ind = get_action_ind( buster )

        if ( l_action[ action_ind ] is not None ):
            continue

        dist_to_base = dist( ( buster.x, buster.y ), base_position )

        # print >> sys.stderr, "buster.entity_id", buster.entity_id
        # print >> sys.stderr, "dist_to_base", dist_to_base

        if ( dist_to_base < 1600 ):
            l_action[ action_ind ] = "RELEASE IN go_to_base"

            # ghost_id = buster.value
            # ghost = d_known_ghost.pop( ghost_id, None )

            # l_ghost_already_released.append( ghost )

            # print >> sys.stderr, "l_ghost_already_released", "\n", \
            #     "\n".join( l_str_entity( l_ghost_already_released ) )
            # print >> sys.stderr, "d_known_ghost", "\n", \
            #     "\n".join( l_str_entity( d_known_ghost.values() ) )
            # print >> sys.stderr, " END go_to_base"
            # print >> sys.stderr, ""

        else :

            # Compute the right position, far of 1598 units from the base
            dist_x = base_position[ 0 ] - buster.x
            dist_y = base_position[ 1 ] - buster.y

            alpha = math.atan2( dist_y, dist_x )

            len_move = dist_to_base - 1598

            delta_x, delta_y = polar_to_cart( alpha, len_move )

            move_x = int( buster.x + delta_x )
            move_y = int( buster.y + delta_y )

            l_action[ action_ind ] = "MOVE " + str( move_x ) + " " + str( move_y ) + \
                                     " GO BASE"


def dist( p0, p1 ):
    return math.sqrt( ( p0[ 0 ] - p1[ 0 ] ) * ( p0[ 0 ] - p1[ 0 ] ) +
                      ( p0[ 1 ] - p1[ 1 ] ) * ( p0[ 1 ] - p1[ 1 ] ) )


# def nearest_available_buster( ghost, l_buster ):

#     l_available_buster = get_available_buster( l_buster )

#     # print >> sys.stderr, "l_available_buster", l_str_entity( l_available_buster )
    
#     l_dist = [ dist( ( ghost.x, ghost.y ), ( buster.x, buster.y ) )
#                for buster in l_available_buster ]

#     dist_min = None
#     if ( l_dist ):
#         dist_min = min( l_dist )
#     else :
#         return [], -1

#     res = []

#     for id_buster, d in enumerate( l_dist ):
#         if ( d == dist_min ):
#             res.append( l_available_buster[ id_buster ] )

#     return res, dist_min


def go_to_catch_aux( ghost, buster, action_ind ):
    dist_to_ghost = dist( ( ghost.x, ghost.y ), ( buster.x, buster.y ) )
        
    # print >> sys.stderr, "dist_to_ghost", dist_to_ghost

    if ( 1760 < dist_to_ghost ) :
        l_action[ action_ind ] = "MOVE " + str( ghost.x ) + " " + str( ghost.y ) + \
                                 " TO " + str( ghost.entity_id )

    elif ( ( 900 < dist_to_ghost ) and ( dist_to_ghost <= 1760 ) ):
        l_action[ action_ind ] = "BUST " + str( ghost.entity_id ) + \
                                     " GET " + str( ghost.entity_id )

    elif ( dist_to_ghost <= 900 ):
        # Compute the right move to get away of 900 unit at least of the ghost
        dist_x = ghost.x - buster.x
        dist_y = ghost.y - buster.y

        alpha = math.atan2( dist_y, dist_x )
        alpha = alpha + math.pi              # Go in the opposite direction of the ghost

        len_move = 902.0 - dist_to_ghost

        delta_x, delta_y = polar_to_cart( alpha, len_move )

        move_x = int( buster.x + delta_x )
        move_y = int( buster.y + delta_y )

        # print >> sys.stderr, "buster.entity_id", buster.entity_id
        # print >> sys.stderr, "ghost.x, ghost.y", ghost.x, ghost.y
        # print >> sys.stderr, "alpha", math.degrees( alpha )
        # print >> sys.stderr, "dist_to_ghost", dist_to_ghost
        # print >> sys.stderr, "len_move", len_move
        # print >> sys.stderr, "delta_x, delta_y", delta_x, delta_y
        # print >> sys.stderr, "move_x, move_y", move_x, move_x

        l_action[ action_ind ] = "MOVE " + str( move_x ) + " " + str( move_y ) + \
                                 " DIST " + str( ghost.entity_id )


reconfig_stun = False
        
def go_to_catch( ghost, l_entity, l_action ):

    global reconfig_stun
    
    # l_nearest_buster, dist_to_ghost = nearest_available_buster( ghost, l_buster )

    # l_nearest_buster = l_buster
    # print >> sys.stderr, "l_nearest_buster", l_str_entity( l_nearest_buster )

    l_buster = get_my_buster( l_entity )
    l_buster = filter( lambda b : ( b.state != 1 ) and ( b.state != 2 ), l_buster )

    if ( l_buster == [] ):
        return

    if ( ghost.discovery == "DROP" ):
        l_buster.sort( key = lambda b : dist( ( b.x, b.y ), ( ghost.x, ghost.y ) ) )

        print >> sys.stderr, "NEAR l_buster", l_str_entity( l_buster )
        
        nearest_buster = l_buster[ 0 ]
        action_ind  = get_action_ind( nearest_buster )
        go_to_catch_aux( ghost, nearest_buster, action_ind )

        print >> sys.stderr, "DROP l_action", l_action

        reconfig_stun = True

    if ( reconfig_stun ):
        stun( l_entity, reload_stun, l_action )
    
    # print >> sys.stderr, "l_buster", l_str_entity( l_buster )

    ghost_id = ghost.entity_id
    # print >> sys.stderr, "ghost_id", ghost_id

    l_op_buster             = get_opponent_buster( l_entity )
    nb_op_buster_busting_it = len( filter( lambda b : ( b.state == 3 ) and ( b. value == ghost_id ),
                                           l_op_buster ) )

    # Limit the number of buster affected to a ghost depending on the LP of the ghost
    nb_buster_max = None

    if ( round_cpt <= exploration_limit_min ) :
        if ( d_life_ghost[ ghost_id ] <= 3 ):
            nb_buster_max = max( 1, nb_op_buster_busting_it )

        elif ( ( 3 < d_life_ghost[ ghost_id ] ) and ( d_life_ghost[ ghost_id ] <= 15 ) ):
            # nb_buster_max = max( 3, nb_op_buster_busting_it )
            nb_buster_max = max( 2, nb_op_buster_busting_it )

        elif ( ( 15 < d_life_ghost[ ghost_id ] ) and ( d_life_ghost[ ghost_id ] <= 40 ) ):
            # nb_buster_max = max( 4, nb_op_buster_busting_it )
            nb_buster_max = max( 3, nb_op_buster_busting_it )

    else :
        nb_buster_max = busters_per_player

    # print >> sys.stderr, "nb_buster_max", nb_buster_max

    nb_buster_for_current_ghost = 0

    # If some buster are already busting the current ghost
    # -> Keep them busting
    for buster in l_buster:
        if ( ( buster.state == 3 ) and ( buster.value == ghost_id ) ):
            action_ind = get_action_ind( buster )
            if ( l_action[ action_ind ] is None ) :
                l_action[ action_ind ] = "BUST " + str( ghost.entity_id ) + \
                                         " READY"
                nb_buster_for_current_ghost += 1

    # Sort the l_buster depending on the distance between a ghost and a buster
    l_buster.sort( key = lambda b : dist( ( b.x, b.y ), ( ghost.x, ghost.y ) ) )

    for buster in l_buster:

        if ( nb_buster_max <= nb_buster_for_current_ghost ) :
            break
        
        action_ind = get_action_ind( buster )

        # print >> sys.stderr, "BEFORE l_action", l_action
        
        if ( l_action[ action_ind ] is not None ):
            continue

        go_to_catch_aux( ghost, buster, action_ind )
        
        # dist_to_ghost = dist( ( ghost.x, ghost.y ), ( buster.x, buster.y ) )
        
        # # print >> sys.stderr, "dist_to_ghost", dist_to_ghost

        # if ( 1760 < dist_to_ghost ) :
        #     l_action[ action_ind ] = "MOVE " + str( ghost.x ) + " " + str( ghost.y ) + \
        #                              " TO " + str( ghost.entity_id )

        # elif ( ( 900 < dist_to_ghost ) and ( dist_to_ghost <= 1760 ) ):
        #     l_action[ action_ind ] = "BUST " + str( ghost.entity_id ) + \
        #                              " GET " + str( ghost.entity_id )

        # elif ( dist_to_ghost <= 900 ):
        #     # Compute the right move to get away of 900 unit at least of the ghost
        #     dist_x = ghost.x - buster.x
        #     dist_y = ghost.y - buster.y

        #     alpha = math.atan2( dist_y, dist_x )
        #     alpha = alpha + math.pi              # Go in the opposite direction of the ghost

        #     len_move = 902.0 - dist_to_ghost

        #     delta_x, delta_y = polar_to_cart( alpha, len_move )

        #     move_x = int( buster.x + delta_x )
        #     move_y = int( buster.y + delta_y )

        #     # print >> sys.stderr, "buster.entity_id", buster.entity_id
        #     # print >> sys.stderr, "ghost.x, ghost.y", ghost.x, ghost.y
        #     # print >> sys.stderr, "alpha", math.degrees( alpha )
        #     # print >> sys.stderr, "dist_to_ghost", dist_to_ghost
        #     # print >> sys.stderr, "len_move", len_move
        #     # print >> sys.stderr, "delta_x, delta_y", delta_x, delta_y
        #     # print >> sys.stderr, "move_x, move_y", move_x, move_x

        #     l_action[ action_ind ] = "MOVE " + str( move_x ) + " " + str( move_y ) + \
        #                              " DIST " + str( ghost.entity_id )

        nb_buster_for_current_ghost += 1
        # print >> sys.stderr, "nb_buster_for_current_ghost", nb_buster_for_current_ghost

        # print >> sys.stderr, "AFTER l_action", l_action


def manage_ghost( l_entity, l_action, d_known_ghost, l_ghost_already_released ):

    global previous_carried_n_stuned_opponent

    l_ghost  = get_ghost( l_entity )
    # print >> sys.stderr, "STD l_ghost", l_str_entity( l_ghost )

    # Update previous_carried_n_stuned_opponent
    # for ghost in l_ghost :
    #     id_ghost = ghost.entity_id
    #     previous_carried_n_stuned_opponent = filter( lambda b : b.value != id_ghost, 
    #                                                  previous_carried_n_stuned_opponent )

    # print >> sys.stderr, "BEFORE previous_carried_n_stuned_opponent", "\n", \
    #     l_str_entity( previous_carried_n_stuned_opponent )

    l_buster = get_my_buster( l_entity )
    tmp_previous_carried_n_stuned_opponent = previous_carried_n_stuned_opponent[ : ]
    for op_buster in previous_carried_n_stuned_opponent :
    # for i in range( len( previous_carried_n_stuned_opponent ) ) :
        # op_buster = previous_carried_n_stuned_opponent[ i ]
        for buster in l_buster:
            if( dist( ( op_buster.x, op_buster.y ), ( buster.x, buster.y ) ) < 400 ) :
                # print >> sys.stderr, "IN tmp_previous_carried_n_stuned_opponent", \
                #     l_str_entity( tmp_previous_carried_n_stuned_opponent )
                # print >> sys.stderr, "op_buster", op_buster

                if ( op_buster in tmp_previous_carried_n_stuned_opponent ) :
                    tmp_previous_carried_n_stuned_opponent.remove( op_buster )

    previous_carried_n_stuned_opponent = tmp_previous_carried_n_stuned_opponent

    # If there is something in previous_carried_n_stuned_opponent
    # go toward it
    for op_buster in previous_carried_n_stuned_opponent :
        p_op_buster = ( op_buster.x, op_buster.y )
        l_buster.sort( key = lambda b : dist( ( b.x, b.y ), p_op_buster ) )

        buster = l_buster[ 0 ]

        print >> sys.stderr, "buster", str( buster )

        action_ind = get_action_ind( buster )

        if ( l_action[ action_ind ] is None ) :
            l_action[ action_ind ] = "MOVE " + str( op_buster.x ) + " " + str( op_buster.y ) + \
                                     " PREV " + str( op_buster.entity_id )

    # Sort the l_ghost depending on the LP of ghost
    l_ghost.sort( key = lambda ghost : ghost.state )

    # print >> sys.stderr, "SORTED l_ghost", l_str_entity( l_ghost )

    for ghost in l_ghost :

        # print >> sys.stderr, "not ( ( round_nb < 8 ) and ( 15 <= ghost.value ) )", not ( ( round_nb < 8 ) and ( 15 <= ghost.value ) )
        if ( round_cpt <= exploration_limit_min ) :
            # if ( ghost.state < 40 ) :
            if ( ghost.state < 15 ) :
                go_to_catch( ghost, l_entity, l_action )
        else :
                go_to_catch( ghost, l_entity, l_action )


# def explore_map( l_buster, l_action ):
# def explore_map( l_entity, l_action, position_by_buster ):
# def explore_map( l_entity, l_action ):
def explore_map( l_entity, l_action, d_known_ghost ):

    # print >> sys.stderr, "BEFORE l_action", l_action

    l_buster = get_my_buster( l_entity )

    l_target_position = []

    for action in l_action :
        if ( ( action is not None ) and ( action.split()[ 0 ] == "MOVE" ) ) :
            move_x = int( action.split()[ 1 ] )
            move_y = int( action.split()[ 2 ] )
            l_target_position.append( ( move_x, move_y ) )
        
    id_ghost = None

    # Go to the opponent base position
    for ind_action in range( busters_per_player ):

        if ( l_action[ ind_action ] is not None ):
            continue
        
        buster  = get_buster_by_id( ind_action, l_buster )

        l_ghost = filter( lambda g : g.discovery != "NOT_FOUND",
                          d_known_ghost.values() )

        # Sort by multiple attribute (it's written in reverse order):
        #  1st: the number of Life Point
        #  2nd: the distance between a ghost and the current buster
        # doc: http://stackoverflow.com/questions/4233476/sort-a-list-by-multiple-attributes
        l_ghost = sorted( l_ghost, key = lambda g : dist( ( g.x, g.y ), ( buster.x, buster.y ) ) )
        l_ghost = sorted( l_ghost, key = attrgetter( 'state' ) )

        print >> sys.stderr, "l_ghost", "\n", "\n".join( l_str_entity( l_ghost ) )

        target_position = None

        if ( ( exploration_limit_min <= round_cpt ) and \
             ( round_cpt < exploration_limit_max ) ) :

            for ghost in l_ghost :
                # first_ghost     = l_ghost[ 0 ]
                # target_position = ( first_ghost.x, first_ghost.y )

                target_position = ( ghost.x, ghost.y )
                id_ghost = ghost.entity_id

                if ( target_position not in l_target_position ):
                    id_ghost = ghost.entity_id
                    l_target_position.append( target_position )
                    print >> sys.stderr, "elected ghost", str( ghost )
                    break

        # else :
        # elif ( target_position is None ) :
        if ( target_position is None ) :
            ind_position    = current_ind_position[ ind_action ]
            target_position = buster.possible_position[ ind_position ]

            
            # If the target position is reached
            if ( dist( ( buster.x, buster.y ), target_position ) < 2200 - 100 ):
                target_position = get_next_target_position( ind_action, 
                                                            ind_position, 
                                                            buster.possible_position )

        print >> sys.stderr, "target_position", target_position
                
        str_id_ghost = str( id_ghost ) if id_ghost is not None else ""
        l_action[ ind_action ] = "MOVE " + \
                                 str( target_position[ 0 ] ) + " " + \
                                 str( target_position[ 1 ] ) + " EX " + \
                                 str_id_ghost

    # print >> sys.stderr, "AFTER l_action", l_action


def update_known_ghost( l_entity, d_known_ghost, l_ghost_already_released, d_life_ghost ):

    l_buster          = get_my_buster( l_entity )
    l_opponent_buster = get_opponent_buster( l_entity )
    l_every_buster    = l_buster + l_opponent_buster

    # Delete ghost from d_known_ghost if it is carrying by a buster
    for buster in l_every_buster :
        if ( buster.state == 1 ) :
            ghost_id = buster.value
            ghost = d_known_ghost.pop( ghost_id, None )

            # print >> sys.stderr, "ghost", str( ghost )

            if ( ghost is not None ) :
                l_ghost_already_released.append( ghost )

    l_ghost  = get_ghost( l_entity )

    for ghost in l_ghost :

        ghost_id = ghost.entity_id
        
        # Add a "NEW" ghost
        if ( ghost_id not in d_known_ghost.keys() ) :
            if ( ghost.state == 0 ):
                ghost.discovery = "DROP"
            d_known_ghost[ ghost_id ] = ghost
        # Update a previous known ghost ( "NEW", "DEDUCED", "UPDATED", "NOT_FOUND" )
        else :
            ghost.discovery = "UPDATED"
            d_known_ghost[ ghost_id ] = ghost

    # print >> sys.stderr, "d_known_ghost.items()", d_known_ghost.items()

    # Add the symmetrical/"DEDUCED" ghost
    for ghost_id, ghost in d_known_ghost.items() :
        # The ghost in the center of tha map doesn't have any symmetrical ghost
        if ( ghost_id == 0 ):
            continue

        # print >> sys.stderr, "ghost.entity_id", ghost.entity_id
        # print >> sys.stderr, "ghost.discovery", ghost.discovery

        # Only care about "NEW" ghost
        if ( ghost.discovery != "NEW" ):
            continue

        # print >> sys.stderr, " IN manage_ghost"
        # print >> sys.stderr, "l_ghost_already_released", "\n", \
        #     "\n".join( l_str_entity( l_ghost_already_released ) )
        # print >> sys.stderr, ""

        symmetric_ghost_id = ghost_id - 1 if ghost_id % 2 == 0 else ghost_id + 1

        maybe_ghost = d_known_ghost.get( symmetric_ghost_id, None )

        # if ( maybe_ghost is None ):
        #     continue

        # if ( maybe_ghost.discovery == "NOT_FOUND" ):
        #     continue

        if ( maybe_ghost is not None ):
            if ( maybe_ghost.discovery == "NOT_FOUND" ):
                continue

        entity_id   = symmetric_ghost_id
        x           = 16000 - ghost.x
        y           = 9000 - ghost.y
        entity_type = ghost.entity_type
        state       = ghost.state
        value       = ghost.value
        discovery   = "DEDUCED"

        symmetric_ghost = Entity( entity_id, x, y, entity_type, state, value, 
                                  discovery = discovery )

        # print >> sys.stderr, "symmetric_ghost", str( symmetric_ghost )
        # print >> sys.stderr, "l_ghost_already_released", l_str_entity( l_ghost_already_released )
        # print >> sys.stderr, "symmetric_ghost in l_ghost_already_released", symmetric_ghost in l_ghost_already_released

        if ( symmetric_ghost in l_ghost_already_released ):
            continue

        d_known_ghost[ symmetric_ghost_id ] = symmetric_ghost

    # print >> sys.stderr, ""


    # Update d_life_ghost
    for ghost_id, ghost in d_known_ghost.items() :
        if ( ghost.discovery in [ "NEW", "DEDUCED" ] ) :
            d_life_ghost[ ghost_id ] = ghost.state

    l_buster = get_my_buster( l_entity )

    # print >> sys.stderr, "d_known_ghost.values()", "\n", \
    #     "\n".join( l_str_entity( d_known_ghost.values() ) )

    # Check if the "DEDUCED" and "UPDATED" are found or "NOT_FOUND"
    for ghost_id, ghost in d_known_ghost.items() :

        # Only care about "NEW", "DROP", "DEDUCED" and "UPDATED" ghost
        if ( ( ghost.discovery != "DEDUCED" ) and \
             ( ghost.discovery != "UPDATED" ) and \
             ( ghost.discovery != "DROP" ) and \
             ( ghost.discovery != "NEW" ) ) :
            continue

        # print >> sys.stderr, "ghost", str( ghost )
        p_ghost = ( ghost.x, ghost.y )

        should_be_visible = False
        for buster in l_buster:
            p_buster = ( buster.x, buster.y )
            # if( dist( p_buster, p_ghost ) < 2200 ):
            if( dist( p_buster, p_ghost ) < 2200 - 800 ):
                should_be_visible = True
                break

        if ( not should_be_visible ):
            continue

        if ( ghost_id not in map( lambda g : g.entity_id, l_ghost ) ) :
            # In this case, if a ghost should be visible and is not, so it is "NOT_FOUND"
            ghost.discovery = "NOT_FOUND"
            d_known_ghost[ ghost_id ] = ghost


# position_by_buster = []
# possible_position = [ opponent_base_position, 
#                       ( opponent_base_position[ 0 ],          base_position[ 1 ] ),
#                       base_position, 
#                       (          base_position[ 0 ], opponent_base_position[ 1 ] ),
#                       base_position ]

possible_position = [ 
    # opponent_base_position, 
    (          base_position[ 0 ], opponent_base_position[ 1 ] ),
    # opponent_base_position, 
    ( opponent_base_position[ 0 ],          base_position[ 1 ] ),
    opponent_base_position, 
    base_position ]

# for i in xrange( busters_per_player ) :
#     position_by_buster.append( possible_position[ 0 ] )

reload_stun = [ 0 ] * busters_per_player

current_ind_position = [ 0 ] * busters_per_player

d_known_ghost = {}
l_ghost_already_released = []
d_life_ghost = {}

exploration_limit_min = None
exploration_limit_max = None

offset_limit_max = int( ghost_count / 9.0 ) * 10

if ( busters_per_player == 2 ) :
    # exploration_limit_min = 40
    exploration_limit_min = 30
    exploration_limit_max = 150
elif ( busters_per_player == 3 ) :
    # exploration_limit_min = 35
    exploration_limit_min = 30
    exploration_limit_max = 155
elif ( busters_per_player == 4 ) :
    exploration_limit_min = 30
    exploration_limit_max = 160

exploration_limit_max += offset_limit_max
    
previous_carried_n_stuned_opponent = []

first_possible_positions = []

angle_offset = None
if ( my_team_id == 0 ) :
    angle_offset = 0
else :
    angle_offset = math.pi

angles = None
offset_angle1 = math.radians( 8 )
if ( busters_per_player == 2 ) :
    angles = [ 0 + offset_angle1, math.pi / 2 - offset_angle1]
elif ( busters_per_player == 3 ) :
    angles = [ 0 + offset_angle1, math.atan2( 9000, 16000 ), math.pi / 2 - offset_angle1]
elif ( busters_per_player == 4 ) :
    angles = [ 0 + offset_angle1, math.atan2( 9000, 16000 ) - offset_angle1, math.atan2( 9000, 16000 ) + offset_angle1, math.pi / 2 - offset_angle1 ]

angles = map( lambda angle : angle + angle_offset, angles )

round_cpt = 0

# game loop
while True:
    round_cpt += 1

    entities = int(raw_input())  # the number of busters and ghosts visible to you

    l_entity          = []
    
    for i in xrange(entities):
        # entity_id  : buster id or ghost id
        # x, y       : position of this buster / ghost
        # entity_type: the team id if it is a buster, -1 if it is a ghost.
        # state      : For busters: 0 = idle, 
        #                           1 = carrying a ghost,
        #                           2 = stuned,
        #                           3 = busting a ghost.
        #              For ghosts : number of LP
        # value      : For busters: Ghost id being carried. 
        #              For ghosts : number of busters attempting to trap this ghost.
        entity_id, x, y, entity_type, state, value = [int(j) for j in raw_input().split()]

        print >> sys.stderr, "IN ", entity_id, x, y, entity_type, state, value

        entity = Entity( entity_id, x, y, entity_type, state, value )

        l_entity.append( entity )
                
    # Post-process the entity: fix the first possible position
    l_buster = get_my_buster( l_entity )
    l_buster.sort( key = lambda b : abs( base_position[ 1 ] - b.y ) )

    # Compute the first target position
    for norm_ind, buster in enumerate( l_buster ) :
        # print >> sys.stderr, "REMOVED buster", str( buster )
        l_entity.remove( buster )

        # quantum = 16000.0 / ( busters_per_player - 1 )
        # pos_x = int( quantum * norm_ind )
        # # print >> sys.stderr, "pos_x", pos_x
        # first_possible_position = ( pos_x, opponent_base_position[ 1 ] )

        if ( round_cpt == 1 ):
            dist_x = buster.x - base_position[ 0 ]
            dist_y = buster.y - base_position[ 1 ]

            # alpha = math.atan2( dist_y, dist_x )
            alpha = angles[ norm_ind ]

            print >> sys.stderr, "alpha", math.degrees( alpha )

            # len_move = 8000
            len_move = 16000

            delta_x, delta_y = polar_to_cart( alpha, len_move )

            move_x = int( buster.x + delta_x )
            move_y = int( buster.y + delta_y )

            move_x = min( move_x, 16000 - 1000 )
            move_x = max( move_x, 0 )
            move_y = min( move_y, 9000 - 1000 )
            move_y = max( move_y, 0 )

            first_possible_position = ( move_x, move_y )
            
            print >> sys.stderr, "buster.entity_id", buster.entity_id
            print >> sys.stderr, "first_possible_position", first_possible_position

            first_possible_positions.append( first_possible_position )
            # first_possible_position = first_possible_positions[ norm_ind ]

        # buster_possible_position = [ first_possible_position ] + possible_position
        buster_possible_position = [ first_possible_positions[ norm_ind ] ] + possible_position
        fixed_buster = Entity( buster.entity_id, buster.x, buster.y, 
                               buster.entity_type, buster.state, buster.value, 
                               possible_position = buster_possible_position )
    
        l_entity.append( fixed_buster )


    # print >> sys.stderr, "previous_carried_n_stuned_opponent", "\n", \
    #     l_str_entity( previous_carried_n_stuned_opponent )

    # print >> sys.stderr, "l_entity", "\n", "\n".join( l_str_entity( l_entity ) )

    l_action = [ None ] * busters_per_player

    # If it's possible to stun
    stun( l_entity, reload_stun, l_action )
    # print >> sys.stderr, "MAIN l_action", l_action

    # If there are some buster already carrying a ghost
    go_to_base( l_entity, l_action, d_known_ghost, l_ghost_already_released )
    # print >> sys.stderr, "MAIN 2 l_action", l_action

    update_known_ghost( l_entity, d_known_ghost, l_ghost_already_released, d_life_ghost )
    # print >> sys.stderr, "MAIN 3 l_action", l_action

    manage_ghost( l_entity, l_action, d_known_ghost, l_ghost_already_released )
    # print >> sys.stderr, "MAIN 4 l_action", l_action
    
    print >> sys.stderr, "d_known_ghost", "\n", \
        "\n".join( l_str_entity( d_known_ghost.values() ) )

    explore_map( l_entity, l_action, d_known_ghost )
    # print >> sys.stderr, "MAIN 5 l_action", l_action

    # MOVE x y | BUST id | RELEASE | STUN id
    for i in xrange(busters_per_player):
        print l_action[ i ]


    # Update reaload_stun
    for i in xrange( len( reload_stun ) ):
        if ( 0 < reload_stun[ i ] ):
            reload_stun[ i ] -= 1

    # print >> sys.stderr, "MAIN 6 l_action", l_action

    # Update previous_stuned_opponent
    for action in l_action :
        print >> sys.stderr, "action", action
        # print >> sys.stderr, "'STUN' == action.split()[ 0 ]", "STUN" == action.split()[ 0 ]
        if ( action.split()[ 0 ] == "STUN" ) :
            # print >> sys.stderr, "action.split()[ 1 ]", action.split()[ 1 ]
            id_op_buster = int( action.split()[ 1 ] )
            l_op_buster  = get_opponent_buster( l_entity )
            # print >> sys.stderr, "l_op_buster", l_str_entity( l_op_buster )
            op_buster    = filter( lambda b : b.entity_id == id_op_buster, l_op_buster )[ 0 ]

            # print >> sys.stderr, "op_buster", op_buster

            if ( op_buster.state == 1 ) :
                previous_carried_n_stuned_opponent.append( op_buster )

    # TODO: simulate the physic aspect of this game to predict the move of the ghost
    #       and update d_known_ghost


    # for buster in get_my_buster( l_entity ) :
    #     if ( 0 < buster.reload_stun ):
    #         buster.reload_stun -= 1
        
