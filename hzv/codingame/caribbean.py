# from __future__ import print_function

import math
from sys import stderr

from types import SimpleNamespace as sns

import time
import copy as cp

from operator import itemgetter

my_ship_count = None
entity_count  = None

SHIP_ID       = 1
BARREL_ID     = 2
CANNONBALL_ID = 3
MINE_ID       = 4

X_MAX = 22
Y_MAX = 20

iteration = 0

target_iteration = -1

def perr( s, verbose = True ) :
    if ( verbose ) :
        print( str( s ), file = stderr )


class Hex( object ) :
    
    def __init__( self, x, y ) :
        self.x = x
        self.y = y


    def __str__( self ) :
        return "hex: " + " ".join( [ str( self.x ), 
                                     str( self.y ) ] )


    def __add__( self, h ) :
        return Hex( self.x + h.x, self.y + h.y )


    def __sub__( self, h ) :
        return Hex( self.x - h.x, self.y - h.y )


    def __eq__( self, h ) :
        return ( ( self.x == h.x ) and ( self.y == h.y ) )


    def dist( self, h ) :
        self_c = hex_to_cube( self )
        oppo_c = hex_to_cube( h )
        return self_c.dist( oppo_c )


    def is_valid( self ) :
        return ( ( 0 <= self.x ) and ( self.x <= X_MAX ) and
                 ( 0 <= self.y ) and ( self.y <= Y_MAX ) )


class Cube( object ) :

    def __init__( self, x, y, z ) :
        self.x = x
        self.y = y
        self.z = z


    def __str__( self ) :
        return "cube: " + " ".join( [ str( self.x ),
                                      str( self.y ),
                                      str( self.z ) ] )


    def __imul__( self, n ) :
        return Cube( n * self.x, n * self.y, n * self.z )


    def __add__( self, c ) :
        return Cube( self.x + c.x, self.y + c.y, self.z + c.z )


    def dist( self, c ) :
        return max( abs( self.x - c.x ), abs( self.y - c.y ), abs( self.z - c.z ) )


def hex_to_cube( h ) :
    col = h.x
    row = h.y
    x   = int( col - ( row - ( row & 1 ) ) / 2 )
    z   = row
    y   = - x - z
    return Cube( x, y, z )


def cube_to_hex( c ) :
    col = int( c.x + ( c.z - ( c.z & 1 ) ) / 2 )
    row = c.z
    return Hex( col, row )


inc_hex_orientation      = [ None, None ]
inc_hex_orientation[ 0 ] = [ Hex( x =  1, y =  0 ),
                             Hex( x =  0, y = -1 ),
                             Hex( x = -1, y = -1 ),
                             Hex( x = -1, y =  0 ),
                             Hex( x = -1, y =  1 ),
                             Hex( x =  0, y =  1 ) ]
inc_hex_orientation[ 1 ] = [ Hex( x =  1, y =  0 ),
                             Hex( x =  1, y = -1 ),
                             Hex( x =  0, y = -1 ),
                             Hex( x = -1, y =  0 ),
                             Hex( x =  0, y =  1 ),
                             Hex( x =  1, y =  1 ) ]

inc_cube_orientation = [ Cube(  1, -1,  0 ),
                         Cube(  1,  0, -1 ),
                         Cube(  0,  1, -1 ),
                         Cube( -1,  1,  0 ),
                         Cube( -1,  0,  1 ),
                         Cube(  0, -1,  1 ) ]


def type_id_to_type_str( type_id ) :

    res = None
    if ( type_id == SHIP_ID ) :
        res = "SHIP"
    elif ( type_id == BARREL_ID ) :
        res = "BARREL"
    elif ( type_id == CANNONBALL_ID ) :
        res = "CANNONBALL"
    elif ( type_id == MINE_ID ) :
        res = "MINE"
    return res


class Entity( Hex ) :

    def __init__( self, entity_id, entity_type, x, y,
                  arg_1, arg_2, arg_3, arg_4 ) :

        self.id    = entity_id
        self.x     = x
        self.y     = y
        self.arg_1 = arg_1
        self.arg_2 = arg_2
        self.arg_3 = arg_3
        self.arg_4 = arg_4

        if( entity_type == "SHIP" ) :
            self.type           = SHIP_ID
            self.orientation    = arg_1
            self.speed          = arg_2
            self.rhum           = arg_3
            self.owner          = arg_4
            self.fire_countdown = None
            self.mine_countdown = None

        elif ( entity_type == "BARREL" ) :
            self.type = BARREL_ID
            self.rhum = arg_1

        elif ( entity_type == "CANNONBALL" ) :
            self.type      = CANNONBALL_ID
            self.expeditor = arg_1
            self.countdown = arg_2

        elif ( entity_type == "MINE" ) :
            self.type      = MINE_ID


    def __str__( self ):
        ent_type = None
        if ( self.type == SHIP_ID ) :
            ent_type = "SHIP"
        elif ( self.type == BARREL_ID ) :
            ent_type = "BARREL"
        elif ( self.type == CANNONBALL_ID ) :
            ent_type = "CANNONBALL"
        elif ( self.type == MINE_ID ) :
            ent_type = "MINE"

        return " ".join( [ str( self.id ),
                           ent_type,
                           " x: " + str( self.x ),
                           " y: " + str( self.y ),
                           "| " + str( self.arg_1 ),
                           str( self.arg_2 ),
                           str( self.arg_3 ),
                           str( self.arg_4 ) ] )


    def front_ship( self ) :
        row           = self.y
        parity        = row & 1
        neighbour_inc = inc_hex_orientation[ parity ][ self.orientation ]
        x = self.x + neighbour_inc.x
        y = self.y + neighbour_inc.y

        if ( ( x < 0 ) or ( X_MAX < x ) or ( y < 0 ) or ( Y_MAX < y ) ) :
            x = self.x
            y = self.y
 
        return Hex( x, y )


    def front_ship_dist( self, s ) :
        h_front_self = self.front_ship( )
        # perr( "h_front_self" )
        # perr( h_front_self )
        return h_front_self.dist( s )


    def countdown_before_hit( self, s ) :
        # perr( "COUNTDOWN" )
        dist = self.front_ship_dist( s )
        # perr( "dist" )
        # perr( dist )
        return int( round( 1 + dist / 3.0 ) )


    def get_next_pos( self ) :
        # perr( "GET_NEXT_POS" )
        # perr( "self" )
        # perr( self )
        self_cube      = hex_to_cube( self )
        inc_cube       = inc_cube_orientation[ self.orientation ]
        inc_cube      *= self.speed
        self_next_cube = self_cube + inc_cube
        self_next_hex  = cube_to_hex( self_next_cube )
        # perr( "self_next_hex" )
        # perr( self_next_hex )

        self_next = None
        if ( self_next_hex.is_valid( ) ) :
            # perr( "type" )
            # perr( self.type )
            self_next = Entity( self.id, type_id_to_type_str( self.type ), 
                                self_next_hex.x, self_next_hex.y, 
                                self.arg_1, self.arg_2, self.arg_3, self.arg_4 )

        return self_next


def dist_ent( my_ship ) :
    c_my_ship = hex_to_cube( my_ship )
    return lambda e : c_my_ship.dist( hex_to_cube( e ) )


class ListEntity( list ) :

    def __init__( self, l ) :
        for i in range( len( l ) ) :
            self.append( l[ i ] )

        # if ( iteration == target_iteration ) :
        #     perr( "__init__ listentity" )
        #     if ( l != [ ] ) :
        #         perr( "l" )
        #         perr( l )
        #     if ( self != [ ] ) :
        #         perr( "self" )
        #         perr( self )


    def __str__( self ) :
        return "\n".join( map( lambda e : str( e ), self ) )


    def get_ship( self, owner = None ) :
        res = ListEntity( [ ] )

        if ( owner is None ) :
            for e in self :
                if ( e.type == SHIP_ID ) :
                    res.append( e )

        else :
            for e in self :
                if ( ( e.type == SHIP_ID ) and
                     ( e.owner == owner ) ) :
                    res.append( e )

        return res


    def get_rhum( self ) :
        res = ListEntity( [ ] )

        for e in self :
            if ( e.type == BARREL_ID ) :
                res.append( e )

        return res


    def get_mine( self ) :
        res = ListEntity( [ ] )

        for e in self :
            if ( e.type == MINE_ID ) :
                res.append( e )

        return res


    def get_cannonball( self ) :
        res = ListEntity( [ ] )

        for e in self :
            if ( e.type == CANNONBALL_ID ) :
                res.append( e )

        return res


def get_target_ent( my_ship ) :
    target_ent = None    
    l_rhum = l_entities.get_rhum( )
    if ( l_rhum ) :
        l_rhum.sort( key = dist_ent( my_ship ) )
        target_ent = l_rhum[ 0 ]

    else :
        l_oppo_ship = l_entities.get_ship( owner = 0 )
        l_oppo_ship.sort( key = dist_ent( my_ship ) )
        oppo_ship = l_oppo_ship[ 0 ]

        target_ent = cp.deepcopy( oppo_ship )
        # target_ent.x = ( target_ent.x + 10 ) % 23
        # target_ent.y = ( target_ent.y - 9 ) % 21

    return target_ent


d_fire_countdown = { }
d_mine_countdown = { }
init_countdowns  = False

def init_fire_n_mine_countdown( l_entities ) :
    global init_countdowns

    l_my_ship = l_entities.get_ship( owner = 1 )

    if ( init_countdowns ) :
        for ship in l_my_ship :
            d_fire_countdown[ ship.id ] -= 1
            d_mine_countdown[ ship.id ] -= 1

    else :
        for ship in l_my_ship :
            d_fire_countdown[ ship.id ] = 0
            d_mine_countdown[ ship.id ] = 0

        init_countdowns = True

        
def update_fire_n_mine_entities( l_entities ) :
    l_my_ship = l_entities.get_ship( owner = 1 )
    for ship in l_my_ship :
        ship.fire_countdown = d_fire_countdown[ ship.id ]
        ship.mine_countdown = d_mine_countdown[ ship.id ]


def read_input( l_entities ) :
    global my_ship_count, entity_count
    my_ship_count = int( input( ) )  # the number of remaining ships
    entity_count  = int( input( ) )  # the number of entities (e.g. ships, mines or cannonballs)
    for i in range( entity_count ) :
        entity_id, entity_type, x, y, arg_1, arg_2, arg_3, arg_4 = input( ).split( )

        entity_id = int( entity_id )
        x         = int( x )
        y         = int( y )
        arg_1     = int( arg_1 )
        arg_2     = int( arg_2 )
        arg_3     = int( arg_3 )
        arg_4     = int( arg_4 )

        l_entities.append( Entity( entity_id, entity_type, x, y,
                                   arg_1, arg_2, arg_3, arg_4 ) )

    if ( iteration == target_iteration ) :
        perr( "READ_INPUT" )
        perr( str( l_entities ) )
        perr( "" )

    init_fire_n_mine_countdown( l_entities )
    update_fire_n_mine_entities( l_entities )


def get_nearest_oppo_ship( l_entities, my_ship ) :
    h_front_ship = my_ship.front_ship( )

    l_oppo_ship = l_entities.get_ship( owner = 0 )
    l_oppo_ship.sort( key = dist_ent( my_ship ) )
    return l_oppo_ship[ 0 ]


def get_move_action( my_ship, target_ent, l_entities ) :
    if ( iteration == target_iteration ) :
        perr( "0 my_ship" )
        perr( str( my_ship ) )
        perr( "target_ent" )
        perr( target_ent )

    l_mine       = l_entities.get_mine( )
    l_cannonball = l_entities.get_cannonball( )
    # l_cannonball = list( filter( lambda c : 2 < c.countdown, l_cannonball ) )
    l_cannonball = list( filter( lambda c : 1 < c.countdown, l_cannonball ) )

    # l_obstacle   = l_mine + l_cannonball
    l_obstacle   = ListEntity( l_mine + l_cannonball )

    # if ( iteration == target_iteration ) :
    #     perr( "l_mine" )
    #     perr( l_mine )
    #     perr( "l_cannonball" )
    #     perr( l_cannonball )
    #     perr( "sum" )
    #     perr( l_mine + l_cannonball )
    #     perr( "0 l_obstacle" )
    #     perr( l_obstacle )
    #     perr( type( l_obstacle ) )
    #     perr( l_obstacle.__len__( ) )
    #     perr( l_obstacle[ 0 ] )
    #     perr( len( l_obstacle ) )
    #     perr( "1 l_obstacle" )

    carte = [ None ] * ( X_MAX + 1 )
    for i in range( X_MAX + 1 ) :
        carte[ i ] = [ None ] * ( Y_MAX + 1 )

    carte[ my_ship.x ][ my_ship.y ] = sns( dist = 0, parent = None )

    to_visit = [ Hex( my_ship.x, my_ship.y ) ]
    while( to_visit ) :
        # perr( "iteration" )
        # perr( iteration )
        # if ( iteration == target_iteration ) :
        #     perr( "to_visit" )
        #     perr( "\n".join( map( lambda h : str( h ), to_visit ) ) )

        cur_hex = to_visit.pop( 0 )
        # perr( "cur_hex" )
        # perr( str( cur_hex ) )
        parity  = cur_hex.y & 1
        for orientation in range( 6 ) :
            inc_hex       = inc_hex_orientation[ parity ][ orientation ]
            # perr( "inc_hex" )
            # perr( str( inc_hex ) )

            neighbour_hex = Hex( cur_hex.x + inc_hex.x, cur_hex.y + inc_hex.y )

            if ( not neighbour_hex.is_valid( ) ) :
                continue

            # perr( "neighbour_hex" )
            # perr( str( neighbour_hex ) )

            # perr( "carte" )
            # # perr( carte )
            # perr( carte[ 0 ][ 0 ] )
            # perr( str( neighbour_hex ) )
            # perr( carte[ neighbour_hex.x ][ neighbour_hex.y ] )
            if ( carte[ neighbour_hex.x ][ neighbour_hex.y ] is not None ) :
                continue

            is_obstacle = False
            for e in l_obstacle :
                if ( ( e.x == neighbour_hex.x ) and ( e.y == neighbour_hex.y ) ) :
                    is_obstacle = True
                    break

            if ( is_obstacle ) :
                carte[ neighbour_hex.x ][ neighbour_hex.y ] = sns( dist = None, parent = None )
                continue

            # perr( "AJOUT" )

            dist = carte[ cur_hex.x ][ cur_hex.y ].dist + 1
            carte[ neighbour_hex.x ][ neighbour_hex.y ] = sns( dist = dist, parent = cur_hex )
        
            to_visit.append( neighbour_hex )

        # perr( "" )

    # perr( "carte" )
    # perr( carte[ target_ent.x ][ target_ent.y ].dist )
    # perr( carte[ target_ent.x ][ target_ent.y ].parent )

    target_hex = Hex( target_ent.x, target_ent.y )
    path = [ target_hex ]

    # if ( iteration == target_iteration ) :
    #     perr( "target_hex" )
    #     perr( target_hex )
    #     perr( "carte[ target_hex.x ][ target_hex.y ]" )
    #     perr( carte[ target_hex.x ][ target_hex.y + 4 ] )
    #     perr( carte[ target_hex.x ][ target_hex.y + 3 ] )
    #     perr( carte[ target_hex.x ][ target_hex.y + 2 ] )
    #     perr( carte[ target_hex.x ][ target_hex.y + 1 ] )
    #     perr( carte[ target_hex.x ][ target_hex.y ] )
    #     perr( carte[ target_hex.x ][ target_hex.y - 1 ] )
    #     perr( carte[ target_hex.x ][ target_hex.y - 2 ] )
    #     perr( carte[ target_hex.x ][ target_hex.y - 3 ] )
    #     perr( carte[ target_hex.x ][ target_hex.y - 4 ] )

    parent = carte[ target_hex.x ][ target_hex.y ].parent
    while ( parent is not None ) :
        path.append( parent )
        parent = carte[ parent.x ][ parent.y ].parent

    path.reverse( )
    # perr( "my_ship" )
    # perr( str( my_ship ) )
    # perr( "target_ent" )
    # perr( str( target_ent ) )

    path.pop( 0 )

    # perr( "path" )
    # perr( " ".join( list( map( lambda h : str( h ), path ) ) ) )
    # perr( "" )

    origin_hex = Hex( my_ship.x, my_ship.y )

    # perr( "path" )
    # perr( "\n".join( map( lambda h : str( h ), path ) ) )
    target_hex = path[ 0 ]
    inc_hex    = target_hex - origin_hex
    # perr( "inc_hex" )
    # perr( inc_hex )
    parity = origin_hex.y & 1
    target_orientation = inc_hex_orientation[ parity ].index( inc_hex )

    move_action_str = ""
    if ( my_ship.orientation != target_orientation ) :
        if ( 0 < my_ship.speed ) :
            move_action_str = "SLOWER"
        else :
            complement             = ( 6 - my_ship.orientation ) % 6
            sub_target_orientation = ( target_orientation + complement ) % 6
            if ( sub_target_orientation <= 3 ) :
                move_action_str = "PORT"
            else :
                move_action_str = "STARBOARD"

    else :
        move_action_str = "FASTER"
        
    return move_action_str


def get_l_action( l_entities ) :

    l_action = [ ]

    l_my_ship = l_entities.get_ship( owner = 1 )
    l_my_ship.sort( key = lambda e : e.id )

    for i, my_ship in enumerate( l_my_ship ) :
        if ( my_ship.fire_countdown <= 0 ) :
            h_front_ship = my_ship.front_ship( )

            oppo_ship = get_nearest_oppo_ship( l_entities, my_ship )
            c_oppo    = hex_to_cube( oppo_ship )

            # perr( "oppo_ship" )
            # perr( str( oppo_ship ) )

            dist = h_front_ship.dist( oppo_ship )

            # perr( "DISTANCE" )
            # perr( dist )
            # perr( "" )

            if ( dist <= 10 ) :

                # NEXT implementation
                countdown_max = int( round( 1 + 10 / 3.0 ) )

                tmp_ship = cp.deepcopy( oppo_ship )
                tmp_l    = [ ]
                # perr( "my_ship" )
                # perr( my_ship )
                for i in range( countdown_max + 1 ) :
                    # perr( "tmp_ship" )
                    # perr( tmp_ship )
                    # perr( my_ship.front_ship_dist( tmp_ship ) )
                    # perr( my_ship.countdown_before_hit( tmp_ship ) )

                    # tmp_l.append( ( tmp_ship, i, my_ship.countdown_before_hit( tmp_ship ) ) )
                    tmp_l.append( ( tmp_ship, abs( my_ship.countdown_before_hit( tmp_ship ) - i ) ) )

                    tmp_ship = tmp_ship.get_next_pos( )
                    if ( tmp_ship is None ) :
                        break

                # perr( "tmp_l" )
                # perr( "\n".join( map( lambda x : str( x ), tmp_l ) ) )
                # perr( "" )

                tmp_l.sort( key = itemgetter( 1 ) )
                target_hex = tmp_l[ 0 ][ 0 ]

                action_str = "FIRE %d %d" % ( target_hex.x, target_hex.y )
                l_action.append( action_str )
                d_fire_countdown[ my_ship.id ] = 2

                continue

        # target_ent = get_target_ent( my_ship )
        # action_str = "MOVE %d %d" % ( target_ent.x, target_ent.y )
        # l_action.append( action_str )

        target_ent = get_target_ent( my_ship )
        # if ( i == 0 ):
        #     move_action = get_move_action( my_ship, target_ent, l_entities )
        #     # perr( "move_action" )
        #     # perr( move_action )
        #     action_str = move_action
        #     l_action.append( action_str )

        # else :
        #     action_str = "MOVE %d %d" % ( target_ent.x, target_ent.y )
        #     l_action.append( action_str )

        move_action = get_move_action( my_ship, target_ent, l_entities )
        # perr( "move_action" )
        # perr( move_action )
        action_str = move_action
        l_action.append( action_str )


    return l_action


# GAME LOOP
while True:
    iteration += 1

    l_entities = ListEntity( [ ] )
    read_input( l_entities )
    perr( "iteration" )
    perr( iteration )


    l_action = get_l_action( l_entities )
    for action in l_action :
        print( action )
