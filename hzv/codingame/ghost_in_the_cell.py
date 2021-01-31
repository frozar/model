import math

from sys import stderr
from operator import itemgetter

from types import SimpleNamespace as sns

def perr( s, verbose = True ) :
    if ( verbose ) :
        print( s, file = stderr )

FACTORY_ID = 1
TROOP_ID   = 2
BOMB_ID    = 3

DIST_MAX   = 20

d_dist        = { }
l_entities    = [ ]
factory_count = None
link_count    = None
entity_count  = None

my_nb_bomb = 2

def get_dist( e1, e2 ) :
    i, j = e1.id, e2.id
    if ( j < i ) :
        i, j = j, i
        
    return d_dist[ ( i, j ) ]

def get_dist_int( i, j ) :
    if ( j < i ) :
        i, j = j, i
        
    return d_dist[ ( i, j ) ]

def print_dist( ) :
    for i in range( factory_count - 1 ) :
        print( "%2d :" % ( i ), end = '', file = stderr )
        for j in range( 1, factory_count ) :
            if ( j < ( i + 1 ) ) : 
                print( "   ", end = '', file = stderr )
            else :
                print( "%2d " % ( get_dist_int( i, j ) ), end = '', file = stderr )
        print( "", file = stderr )
        

class Entity( object ) :

    def __init__( self, entity_id, entity_type, 
                  arg_1, arg_2, arg_3, arg_4, arg_5 ) :

        self.id     = entity_id
        self.arg_1  = arg_1
        self.arg_2  = arg_2
        self.arg_3  = arg_3
        self.arg_4  = arg_4
        self.arg_5  = arg_5
        self.credit = 0

        if( entity_type == "FACTORY" ) :
            self.type    = FACTORY_ID
            self.owner   = arg_1
            self.nb_cyb  = arg_2
            self.prod    = arg_3
            self.nb_tour = arg_4

        elif ( entity_type == "TROOP" ) :
            self.type    = TROOP_ID
            self.owner   = arg_1
            self.src     = arg_2
            self.dest    = arg_3
            self.nb_cyb  = arg_4
            self.nb_tour = arg_5

        elif ( entity_type == "BOMB" ) :
            self.type    = BOMB_ID        
            self.owner   = arg_1
            self.src     = arg_2
            self.dest    = arg_3
            self.nb_tour = arg_4


    def __str__( self ):
        ent_type = None
        if ( self.type == FACTORY_ID ) :
            ent_type = "FACTORY"
        elif ( self.type == TROOP_ID ) :
            ent_type = "TROOP"
        elif ( self.type == BOMB_ID ) :
            ent_type = "BOMB"

        return " ".join( [ str( self.id ),
                           ent_type,
                           str( self.arg_1 ),
                           str( self.arg_2 ),
                           str( self.arg_3 ),
                           str( self.arg_4 ),
                           str( self.arg_5 ) ] )

        def __eq__( self, ent ) :
            return self.id == ent.id


class ListEntity( list ) :
    
    def get_factory( self, owner = None, parity = None ) :
        res = ListEntity( )

        if ( owner is None ) :
            if ( parity is None ) :
                for e in self :
                    if ( e.type  == FACTORY_ID ) :
                        res.append( e )
            else :
                for e in self :
                    if ( ( e.type  == FACTORY_ID ) and
                         ( ( e.id % 2 ) == ( parity % 2 ) ) ) :
                        res.append( e )

        else :
            for e in self :
                if ( ( e.type  == FACTORY_ID ) and
                     ( e.owner == owner ) ) :
                    if ( parity is None ) :
                        res.append( e )
                    elif ( ( e.id % 2 ) == ( parity % 2 ) ) :
                        res.append( e )

        return res

    def get_troop( self, owner = None ) :
        res = ListEntity( )

        if ( owner is None ) :
            for e in self :
                if ( e.type  == TROOP_ID ) :
                    res.append( e )

        else :
            for e in self :
                if ( ( e.type  == TROOP_ID ) and
                     ( e.owner == owner ) ) :
                    res.append( e )

        return res

    def get_bomb( self, owner = None ) :
        res = ListEntity( )

        if ( owner is None ) :
            for e in self :
                if ( e.type  == BOMB_ID ) :
                    res.append( e )

        else :
            for e in self :
                if ( ( e.type  == BOMB_ID ) and
                     ( e.owner == owner ) ) :
                    res.append( e )

        return res


def read_input( l_entities ) :
    entity_count = int(input())  # the number of entities (e.g. factories and troops)
    for i in range(entity_count):
        entity_id, entity_type, arg_1, arg_2, arg_3, arg_4, arg_5 = input().split()
        entity_id = int(entity_id)
        arg_1 = int(arg_1)
        arg_2 = int(arg_2)
        arg_3 = int(arg_3)
        arg_4 = int(arg_4)
        arg_5 = int(arg_5)

        l_entities.append( Entity( entity_id, entity_type, 
                                 arg_1, arg_2, arg_3, arg_4, arg_5 ) )


def get_credit( my_fact, verbose = False ) :
    
    # perr( my_fact )
    prod = my_fact.prod
    # if the current factory have no production, give it up
    if ( prod == 0 ) :
        return my_fact.nb_cyb

    ## Consider every opponent factories as attacker
    attacks_n_helps = { }
    for i in range( DIST_MAX + 1 ) :
        attacks_n_helps[ i ] = 0

    # # perr( "PASS 2" )
    # # for my_fact in l_entities.get_factory( 1 ) :
    # for oppo_fact in l_entities.get_factory( -1 ) :
    #     dist = get_dist( my_fact, oppo_fact )
    #     # perr ( "nb_oppo_cyb: %d" % ( oppo_fact.nb_cyb ) )
    #     attacks_n_helps[ dist ] -= oppo_fact.nb_cyb
    
    # perr( "ATTACKS AND HELPS( 0 )", verbose )
    # perr( attacks_n_helps, verbose )

    ## Take into account the opponent troop
    perr( "OPPONENT TROOP", verbose )
    # for e in l_entities.get_troop( -1 ) :
    #     perr( e )
    # perr( "FILTER" )
    for e in filter( lambda x : x.dest == my_fact.id, \
                     l_entities.get_troop( -1 ) ) :
        perr( e, verbose )
        attacks_n_helps[ e.nb_tour ] -= e.nb_cyb

    # perr( "ATTACKS AND HELPS( 1 )", verbose )
    # perr( attacks_n_helps, verbose )

    perr( "MY TROOP", verbose )
    for e in filter( lambda x : x.dest == my_fact.id, \
                     l_entities.get_troop( 1 ) ) :
        perr( e, verbose )
        attacks_n_helps[ e.nb_tour ] += e.nb_cyb
    # for oppo_troop in l_entities.get_troop( -1 ) :
        

    perr( "ATTACKS AND HELPS( 2 )", verbose )
    perr( attacks_n_helps, verbose )

    # virtual_nb_cyb = l_entities.get_factory( 1 )[ 0 ].nb_cyb
    virtual_nb_cyb      = [ 0 ] * ( DIST_MAX + 1 )
    virtual_nb_cyb[ 0 ] = my_fact.nb_cyb
    for i in range( 1, DIST_MAX + 1 ) :
        if ( my_fact.nb_tour < i ) :
            virtual_nb_cyb[ i ]  = virtual_nb_cyb[ i - 1 ] + prod
        virtual_nb_cyb[ i ] += attacks_n_helps[ i ]
        # perr( virtual_nb_cyb[ i ] )

    perr( "virtual_nb_cyb", verbose )
    perr( virtual_nb_cyb[ : 8 ], verbose )

    left_nb_cyb = min( virtual_nb_cyb )
    perr( "left_nb_cyb: %d" % ( left_nb_cyb ), verbose )
    
    credit = None
    if ( left_nb_cyb < 1 ) :
        diff   = 1 - left_nb_cyb
        # credit = my_fact.nb_cyb - diff
        credit = -diff
    else :
        # credit = my_fact.nb_cyb
        diff   = left_nb_cyb - 1
        credit = diff

    perr( "CREDIT", verbose )
    perr( credit, verbose )

    return credit


factory_count = int(input())  # the number of factories
link_count    = int(input())  # the number of links between factories
for i in range( link_count ) :
    factory_1, factory_2, distance = [ int(j) for j in input().split() ]
    d_dist[ ( factory_1, factory_2 ) ] = distance


# print_dist( )

def dijkstra( i, j ) :

    if ( i == j ) :
        return 0, [ ]

    visited_links = { }
    for ii in range( factory_count - 1 ) :
        for jj in range( ii + 1, factory_count ) :
            visited_links[ ( ii, jj ) ] = False

    c_nodes = { }
    for ii in range( factory_count ) :
        c_nodes[ ii ] = None if ( ii != i ) else 0

    paths        = {}
    paths[ i ] = [ i ]
    paths[ j ] = None

    # already_view   = [ ]
    nodes_to_visit = [ i ]
    # i_dbg = 0
    while ( nodes_to_visit ) :
        # i_dbg += 1
        # perr( "i_dbg: %d" % ( i_dbg ) )
        # if ( 10 < i_dbg ) :
        #     break

        cur_node = nodes_to_visit.pop( 0 )
        # already_view.append( cur_node )

        links_not_visited = [ ]
        for link, state in visited_links.items( ) :
            if ( ( cur_node in link ) and ( not state ) ) :
                links_not_visited.append( link )

        # perr( "cur_node" )
        # perr( cur_node )
        l_neighboug_node_to_visit = [ ]
        for l in links_not_visited :
            # perr( "l" )
            # perr( l )
            if ( l[ 0 ] == cur_node ) :
                l_neighboug_node_to_visit.append( l[ 1 ] )
            else :
                l_neighboug_node_to_visit.append( l[ 0 ] )

        # perr( "l_neighboug_node_to_visit" )
        # perr( l_neighboug_node_to_visit )
        for dest_node in l_neighboug_node_to_visit :
            dist = get_dist_int( cur_node, dest_node )
            
            if ( c_nodes[ dest_node ] is None ) :
                c_nodes[ dest_node ] = c_nodes[ cur_node ] + dist
                paths[ dest_node ]   = paths[ cur_node ] + [ dest_node ]
                nodes_to_visit.append( dest_node )
            else :
                if ( c_nodes[ cur_node ] + dist < c_nodes[ dest_node ] ):
                    c_nodes[ dest_node ] = c_nodes[ cur_node ] + dist
                    paths[ dest_node ]   = paths[ cur_node ] + [ dest_node ]

            visited_links[ ( cur_node, dest_node ) ] = True
            visited_links[ ( dest_node, cur_node ) ] = True


        if ( c_nodes[ j ] ) :
            for n in nodes_to_visit :
                if ( c_nodes[ j ] <= c_nodes[ n ] ):
                    nodes_to_visit.remove( n )

    # perr( "c_nodes" )
    # perr( c_nodes[ 6 ] )
    # perr( "paths" )
    # perr( paths[ 6 ] )

    return c_nodes[ j ], paths[ j ]


# dijkstra( 2, 6 )

d_dijkstra = { }
for i in range( factory_count ) :
    for j in range( factory_count ) :
        path_len, path_nodes = dijkstra( i, j )
        # d_dijkstra[ ( i, j ) ] = ( path_len, path_nodes )
        path_struct = sns( len = path_len, nodes = path_nodes )
        d_dijkstra[ ( i, j ) ] = path_struct

# perr( d_dijkstra.items( ) )
# exit( )

time_step = 0

## GAME LOOP
while True :
    time_step += 1
    # l_entities = [ ]
    # l_entities = None
    l_entities = ListEntity( )
    read_input( l_entities )
    # perr( "len: %d" % ( len( l_entities ) ) )

    order = [ ]

    id_to_watch = -1

    d_credit_n_possibility = { }
    for my_fact in l_entities.get_factory( 1 ) :

        if ( my_fact.id == id_to_watch ) :
            perr( my_fact )
            credit = get_credit( my_fact, True )
        else :
            credit = get_credit( my_fact )

        my_fact.credit = credit

        # d_credit[ my_fact ] = credit

        l_neutral_factories = []
        # for e in l_entities.get_factory( 0 ) :
        for e in l_entities.get_factory( 0, my_fact.id ) :
            # perr( e )
            # perr( get_dist( my_fact, e ) )
            # l_neutral_factories.append( ( e.id, e.prod, get_dist( my_fact, e ) ) )
            if ( e.prod == 0 ) :
                continue

            # l_neutral_factories.append( ( e.id, get_dist( my_fact, e ) , 
            #                               e.prod, e.nb_cyb ) )
            # l_neutral_factories.append( ( e.id, d_dijkstra[ ( my_fact.id, e.id ) ][ 0 ], 
            #                               e.prod, e.nb_cyb ) )
            e_tmp = sns( id     = e.id,
                         # dist   = d_dijkstra[ ( my_fact.id, e.id ) ][ 0 ],
                         dist   = d_dijkstra[ ( my_fact.id, e.id ) ].len,
                         prod   = e.prod,
                         nb_cyb = e.nb_cyb )
            l_neutral_factories.append( e_tmp )

        # perr( l_neutral_factories )
        # Sort by multiple attribute (it's written in reverse order):
        #  1st: factory production
        #  2nd: distance to my factory
        #  3rd: nb of neutral cyborg
        # doc: http://stackoverflow.com/questions/4233476/sort-a-list-by-multiple-attributes
        # l_neutral_factories.sort( key = itemgetter( 3 ) )
        # # perr( l_neutral_factories )
        # l_neutral_factories.sort( key = itemgetter( 1 ) )
        # # perr( l_neutral_factories )
        # l_neutral_factories.sort( key = itemgetter( 2 ), reverse = True )
        
        l_neutral_factories.sort( key = lambda x : x.nb_cyb )
        # perr( l_neutral_factories )
        l_neutral_factories.sort( key = lambda x : x.dist )
        # perr( l_neutral_factories )
        l_neutral_factories.sort( key = lambda x : x.prod, reverse = True )
        
        if ( my_fact.id == id_to_watch ) :
            perr( "l_neutral_factories" )
            perr( l_neutral_factories )

        # d_credit_n_possibility[ my_fact.id ] = l_neutral_factories

        # for e in l_neutral_factories :
        #     d_credit_n_possibility[ my_fact.id ] = d_credit_n_possibility.get( my_fact.id, [ ] ) + \
        #                                            [ ( e[ 0 ], e[ 1 ] ) ]
    
        # d_credit_n_possibility[ my_fact ] = ( credit, l_neutral_factories )
        
        l_opponent_factories = []
        for e in l_entities.get_factory( -1 ) :
            # perr( e )
            # perr( get_dist( my_fact, e ) )
            # l_neutral_factories.append( ( e.id, e.prod, get_dist( my_fact, e ) ) )
            # if ( e.prod == 0 ) :
            #     continue

            # l_opponent_factories.append( ( e.id, get_dist( my_fact, e ) , 
            #                                e.prod, e.nb_cyb ) )
            # l_opponent_factories.append( ( e.id, d_dijkstra[ ( my_fact.id, e.id ) ][ 0 ], 
            #                               e.prod, e.nb_cyb ) )
            e_tmp = sns( id     = e.id,
                         # dist   = d_dijkstra[ ( my_fact.id, e.id ) ][ 0 ],
                         dist   = d_dijkstra[ ( my_fact.id, e.id ) ].len,
                         prod   = e.prod,
                         nb_cyb = e.nb_cyb )
            l_opponent_factories.append( e_tmp )

        # perr( l_neutral_factories )
        # Sort by multiple attribute (it's written in reverse order):
        #  1st: factory production
        #  2nd: distance to my factory
        #  3rd: nb of neutral cyborg
        # doc: http://stackoverflow.com/questions/4233476/sort-a-list-by-multiple-attributes
        # l_opponent_factories.sort( key = itemgetter( 3 ) )
        # # perr( l_neutral_factories )
        # l_opponent_factories.sort( key = itemgetter( 1 ) )
        # # perr( l_neutral_factories )
        # l_opponent_factories.sort( key = itemgetter( 2 ), reverse = True )

        l_opponent_factories.sort( key = lambda x : x.nb_cyb )
        # perr( l_neutral_factories )
        l_opponent_factories.sort( key = lambda x : x.dist )
        # perr( l_neutral_factories )
        l_opponent_factories.sort( key = lambda x : x.prod, reverse = True )

        if ( my_fact.id == id_to_watch ) :
            perr( "l_opponent_factories" )
            perr( l_opponent_factories )

        #d_credit_n_possibility.get( my_fact.id, [ ] ) + \
        d_credit_n_possibility[ my_fact.id ] = l_neutral_factories + \
                                               l_opponent_factories

        # for e in l_neutral_factories :
        #     d_credit_n_possibility[ my_fact.id ] = d_credit_n_possibility.get( my_fact.id, [ ] ) + \
        #                                            [ ( e[ 0 ], e[ 1 ] ) ]
    
        # d_credit_n_possibility[ my_fact ] = ( credit, l_neutral_factories )
        
    critical_fact = [ ]
    for my_fact in l_entities.get_factory( 1 ) :
        if ( my_fact.credit < 0 ) :
            critical_fact.append( my_fact )

    perr( "CRITICAL FACTORIES" )
    for e in critical_fact :
        perr( e )

    for e in critical_fact :
        # Get the other factories and sort them distance
        my_factories = [ ]
        for other_e in l_entities.get_factory( 1 ) :
            if ( e == other_e ) :
                continue
            if ( other_e.credit < 0 ) :
                continue
            # my_factories.append( ( other_e, get_dist( other_e, e ) ) )
            # my_factories.append( ( other_e, d_dijkstra[ ( other_e.id, e.id ) ][ 0 ] ) )
            my_factories.append( ( other_e, d_dijkstra[ ( other_e.id, e.id ) ].len ) )

            # l_opponent_factories.append( ( e.id, d_dijkstra( ( my_fact.id, e.id ) )[ 0 ], 
            #                               e.prod, e.nb_cyb ) )
        my_factories.sort( key = itemgetter( 1 ) )

        critical_credit = e.credit
        for fact, dist in my_factories :
            diff = fact.credit - ( 1 - critical_credit )
            if ( 0 < diff ) :
                # order.append( ( fact.id, e.id, 1 - critical_credit ) )
                # path      = d_dijkstra[ ( e.id, fact.id ) ][ 1 ]
                # path      = d_dijkstra[ ( fact.id, e.id ) ][ 1 ]
                path      = d_dijkstra[ ( fact.id, e.id ) ].nodes
                dest_fact = path[ 1 ]
                order.append( ( fact.id, dest_fact, 1 - critical_credit ) )
                critical_credit  = 1
                fact.credit     -= ( 1 - critical_credit )
                break 
            else :
                # order.append( ( fact.id, e.id, fact.credit ) )
                # path      = d_dijkstra[ ( e.id, fact.id ) ][ 1 ]
                # path      = d_dijkstra[ ( fact.id, e.id ) ][ 1 ]
                path      = d_dijkstra[ ( fact.id, e.id ) ].nodes
                dest_fact = path[ 1 ]
                order.append( ( fact.id, dest_fact, fact.credit ) )
                critical_credit -= fact.credit
                fact.credit      = 0

    # perr( "DICTIONARY" )
    # perr( d_credit_n_possibility.items( ) )

    for my_fact_id, l_factories in d_credit_n_possibility.items( ) :
        # perr( my_fact_id )
        perr( "\n".join( map( lambda x : str( x ), l_factories[ : 6 ] ) ) )
        for e in l_factories :
            if ( l_entities[ my_fact_id ].credit <= 0 ) :
                break
        
            # perr( t )
            # nb_cyb = t[ 3 ]
            nb_cyb = e.nb_cyb
            diff   = l_entities[ my_fact_id ].credit - ( nb_cyb + 1 )
            # perr( diff )
            if ( 0 < diff ) :
                # path      = d_dijkstra[ ( my_fact_id, t[ 0 ] ) ][ 1 ]
                # path      = d_dijkstra[ ( my_fact_id, e.id ) ][ 1 ]
                path      = d_dijkstra[ ( my_fact_id, e.id ) ].nodes
                dest_fact = path[ 1 ]
                # perr( "(0)src/dest  : %d %d" % ( my_fact_id, t[ 0 ] ) )
                perr( "(0)src/dest  : %d %d" % ( my_fact_id, e.id ) )
                perr( "(0)d_dijkstra: %d" % ( dest_fact ) )
                order.append( ( my_fact_id, dest_fact, nb_cyb + 1 ) )
                l_entities[ my_fact_id ].credit -= nb_cyb + 1
            else :
                # fact = l_entities[ t[ 0 ] ]
                # if ( fact.owner == -1 ) :
                #     path      = d_dijkstra[ ( my_fact_id, t[ 0 ] ) ][ 1 ]
                #     dest_fact = path[ 1 ]
                #     perr( "(oppo)src/dest  : %d %d" % ( my_fact_id, t[ 0 ] ) )
                #     perr( "(oppo)d_dijkstra: %d" % ( dest_fact ) )
                #     order.append( ( my_fact_id, dest_fact, l_entities[ my_fact_id ].credit ) )
                #     l_entities[ my_fact_id ].credit = 0

                # else :
                #     # continue
                #     path      = d_dijkstra[ ( my_fact_id, t[ 0 ] ) ][ 1 ]
                #     dest_fact = path[ 1 ]
                #     perr( "(neutre)src/dest  : %d %d" % ( my_fact_id, t[ 0 ] ) )
                #     perr( "(neutre)d_dijkstra: %d" % ( dest_fact ) )
                #     order.append( ( my_fact_id, dest_fact, l_entities[ my_fact_id ].credit ) )
                #     l_entities[ my_fact_id ].credit = 0

                # path      = d_dijkstra[ ( my_fact_id, t[ 0 ] ) ][ 1 ]
                # path      = d_dijkstra[ ( my_fact_id, e.id ) ][ 1 ]
                path      = d_dijkstra[ ( my_fact_id, e.id ) ].nodes
                dest_fact = path[ 1 ]
                # perr( "(1)src/dest  : %d %d" % ( my_fact_id, t[ 0 ] ) )
                perr( "(1)src/dest  : %d %d" % ( my_fact_id, e.id ) )
                perr( "(1)d_dijkstra: %d" % ( dest_fact ) )
                order.append( ( my_fact_id, dest_fact, l_entities[ my_fact_id ].credit ) )
                l_entities[ my_fact_id ].credit = 0



    inc_order = [ ]
    for my_fact in l_entities.get_factory( 1 ) :
        if ( ( 10 <= my_fact.credit ) and ( my_fact.prod < 3 ) ):
            inc_order.append( my_fact.id )
            my_fact.credit -= 10
        
    order_bomb = [ ]

    if ( ( 3 < time_step ) and ( 0 < my_nb_bomb ) ) :
        for oppo_fact in sorted( l_entities.get_factory( -1 ),
                                 key = lambda f : f.nb_cyb,
                                 reverse = True ) :
            # perr( "oppo_fact" )
            # perr( oppo_fact )
            my_factories = list( map( lambda f : ( f, get_dist( f, oppo_fact ) ),
                                      l_entities.get_factory( 1 ) ) )
            my_factories.sort( key = itemgetter( 1 ) )
            my_fact = my_factories[ 0 ][ 0 ]
            
            if ( 0 < my_nb_bomb ):
                order_bomb.append( ( my_fact.id, oppo_fact.id ) )
                my_nb_bomb -= 1
            else :
                break

    
    my_production_capacity  = sum( map( lambda x : x.prod, l_entities.get_factory( 1 ) ) )
    his_production_capacity = sum( map( lambda x : x.prod, l_entities.get_factory( -1 ) ) )

    # perr( my_production_capacity )
    # perr( his_production_capacity )

    # ACTION
    s_order = None
    if ( len( order ) == 0 ) :
        s_order = [ "WAIT" ]

    else :
        s_order = list( map( lambda o : "MOVE %d %d %d" % ( o[ 0 ], o[ 1 ], o[ 2 ] ), order ) )

    s_inc_order  = list( map( lambda fact_id : "INC %d" % ( fact_id ), inc_order ) )
    s_order_bomb = list( map( lambda o : "BOMB %d %d" % ( o[ 0 ], o[ 1 ] ), order_bomb ) )
    msg          = [ "MSG %d vs %d" %( my_production_capacity, his_production_capacity ) ]

    s_order += s_inc_order
    s_order += s_order_bomb
    s_order += msg
    
    print( ";".join( s_order ) )

