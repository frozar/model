import sys
import math

import copy as cp

def is_a_link( link, links ):

    res = False
    
    for ln in links :
        if ( ( ln == link ) or ( ln == link[ :: -1 ] ) ):
            res = True

    return res

def cmp_path( elt1, elt2 ):
    if ( elt1[ 0 ] < elt2[ 0 ] ):
        return -1
    elif ( elt1[ 0 ] > elt2[ 0 ] ):
        return 1
    elif ( elt1[ 1 ] < elt2[ 1 ] ):
        return 1
    elif ( elt1[ 1 ] > elt2[ 1 ] ):
        return -1
    else:
        return 0

def get_l_c_path( si, nodes, links, gateways ):

    l_c_path = []
    
    for node_g in gateways :
        dist, path = dijkstra_si( si, nodes, links, node_g )
        # print >> sys.stderr, "dist, path", dist, path
        if ( dist ):
            l_c_path.append( ( dist, path ) )

    l_c_path.sort()

    # print >> sys.stderr, "l_c_path", l_c_path
    score_min = l_c_path[ 0 ][ 0 ]
    filted_path = filter( lambda x : x[ 0 ] == score_min, l_c_path )
    l_c_path = filted_path


    links_score = {}
    
    for elt in l_c_path:
        paths = elt[ 1 ]
        # print >> sys.stderr, "paths", paths
        for path in paths:
            tmp_links = zip( path[ : -1 ], path[ 1 : ] )
            # print >> sys.stderr, "tmp_links", tmp_links
            for i, link in enumerate( tmp_links ) :
                links_score[ link ] = links_score.get( link, 0 ) + \
                                      float( 1 ) / ( i + 1 )

    # print >> sys.stderr, "links_score", links_score

    res = []
    
    for elt in l_c_path:
        score, paths = elt

        sum_link_score = 0
        for path in paths:
            tmp_links = zip( path[ : -1 ], path[ 1 : ] )
            # print >> sys.stderr, "tmp_links", tmp_links
            for link in tmp_links :
                sum_link_score += links_score[ link ]

        res.append( ( score, sum_link_score, paths ) )
    
    res.sort( cmp=cmp_path )

    # print >> sys.stderr, "res", res
    
    # print >> sys.stderr, "l_c_path END"
    return res


def get_critical_nodes( nodes, links, gateways ):

    l_nodes = []
    
    node_next_gateways = get_node_next_gateway( nodes, links, gateways )

    if ( verbose ):
        print >> sys.stderr, "node_next_gateways", node_next_gateways
    
    for n_n_g in node_next_gateways :
        cpt = 0
        for n_g in gateways :
            if ( is_a_link( ( n_n_g, n_g ), links ) ):
                cpt += 1

        if ( cpt > 1 ):
            # if ( verbose ):
            #     print >> sys.stderr, "n_n_g", n_n_g
            l_nodes.append( n_n_g )
    
    return l_nodes


def get_next_critical_node( si_next, tmp_links,
                            tmp_gateways, tmp_node_next_gateways ):

    # i_dbg = 0

    in_case_critical = []
    count = 0

    # verbose = True

    while( not is_critical_node( si_next, tmp_gateways, tmp_links ) ) :

        count += 1

        if ( 4 < count ):
            return in_case_critical[ 0 ]

        # i_dbg += 1
        
        # if ( i_dbg >= 6 ):
        #     exit()
        
        # Look for the potential path
        l_c_path = get_l_c_path( si_next, nodes, tmp_links, tmp_gateways )
                
        if ( verbose ):
            print >> sys.stderr, "FROZAR l_c_path", l_c_path
    
        dist, link_score, path = l_c_path[ 0 ]
        
        if ( verbose ):
            print >> sys.stderr, "dist, path", dist, path
    
        one_path = path[ 0 ]
        link_to_delete = one_path[ -2 ], one_path[ -1 ]
        link_to_delete = get_right_link( link_to_delete, tmp_links )
        in_case_critical.append( one_path[ -1 ] )    

        if ( verbose ):
            print >> sys.stderr, "link_to_delete", link_to_delete
    
        # Delete the link
        tmp_links.remove( link_to_delete )
    
        update_after_delete( tmp_links, tmp_gateways, tmp_node_next_gateways )
                
        elected_node = get_next_agent_node( si_next, nodes, tmp_links, tmp_gateways )
        
        # if ( verbose ):
        #     print >> sys.stderr, "elected_node", elected_node
    
        if ( verbose ):
            print >> sys.stderr, "FROZAR PASS"

        si_next = elected_node
        
        if ( verbose ):
            print >> sys.stderr, "si_next", si_next
            print >> sys.stderr, "is_critical_node", is_critical_node( si_next, tmp_gateways, tmp_links )


    if ( verbose ):
        print >> sys.stderr, "get_next_critical_node", get_next_critical_node
    # print >> sys.stderr, "si_next", si_next
    return si_next


def get_critical_link_to_delete( si_next, tmp_gateways, tmp_links ):

    l_c_path = get_l_c_path( si_next, nodes, tmp_links, tmp_gateways )

    # l_c_path.sort( cmp = cmp_path )
    score_min = l_c_path[ 0 ][ 0 ]
    filted_path = filter( lambda x : x[ 0 ] == score_min, l_c_path )

    # print >> sys.stderr, "FILTED_PATH", filted_path

    all_path = filted_path[0][2]
    one_path = all_path[0]
    # print >> sys.stderr, "one_path", one_path
    link_to_delete = one_path[ 0 ], one_path[ 1 ]
    
    return get_right_link( link_to_delete, tmp_links )


def is_critical_node( si_next, tmp_gateways, tmp_links ):

    l_c_path = []
    
    for node_g in tmp_gateways :
        dist, path = dijkstra_si( si_next, nodes, tmp_links, node_g )
        # print >> sys.stderr, "dist, path", dist, path
        if ( dist ):
            l_c_path.append( ( dist, path ) )

    filted_path = filter( lambda x : x[ 0 ] == 1, l_c_path )

    if ( verbose ):
        print >> sys.stderr, "FILTED_PATH", filted_path
    
    return len( filted_path ) >= 2


def get_neighbourhood( cur_n, links ) :
    l_neighbour=[]
    for ln in links:
        if ( cur_n == ln[ 0 ] ) :
            l_neighbour.append( ln[ 1 ] )

        if ( cur_n == ln[ 1 ] ) :
            l_neighbour.append( ln[ 0 ] )

    return l_neighbour


def dijkstra_si( n_s, nodes, links, n_g ) :
    # print >> sys.stderr, "n_s", n_s
    # print >> sys.stderr, "n_g", n_g

    if ( n_s == n_g ) :
        return None, []
    
    dist_nodes = {}
    # print >> sys.stderr, "nodes", nodes
    # for n in nodes :
    #     if ( n != n_s ) :
    #         dist_nodes[ n ] = None
    #     else :
    #         dist_nodes[ n ] = 0

    for n in nodes :
        dist_nodes[ n ] = None

    dist_nodes[ n_s ] = 0
    
    # print >> sys.stderr, "dist_nodes[ n_g ]", dist_nodes[ n_g ]
    nodes_to_visit = [ n_s ]

    # print >> sys.stderr, "nodes_to_visit", nodes_to_visit
    paths        = {}
    # paths[ n_s ] = [ [ n_s ] ]
    paths[ n_s ] = set( )
    paths[ n_s ].add( tuple( [ n_s ] ) )
    paths[ n_g ] = None

    # print >> sys.stderr, "0 paths", paths

    while ( nodes_to_visit ):
        
        cur_n          = nodes_to_visit.pop( 0 )
        # l_n_v_ng       = get_not_visited_neighbourhood( cur_n, links, dist_nodes )
        l_n_v_ng       = get_neighbourhood( cur_n, links )

        # print >> sys.stderr, "l_n_v_ng", l_n_v_ng
        # print >> sys.stderr, "cur_n", cur_n
        # print >> sys.stderr, "paths[ cur_n ]", paths[ cur_n ]

        for n in l_n_v_ng :

            # print >> sys.stderr, "n", n
            # print >> sys.stderr, "dist_nodes[ n ]", dist_nodes[ n ]
            # print >> sys.stderr, "not dist_nodes[ n ]", not dist_nodes[ n ]

            if ( dist_nodes[ n ] is None ):

                dist_nodes[ n ] = dist_nodes[ cur_n ] + 1

                # print >> sys.stderr, "cur_n", cur_n
                # print >> sys.stderr, "paths", paths
                res = set()
                new_path = cp.deepcopy( paths[ cur_n ] )
                # print >> sys.stderr, "new_path", type( new_path ), new_path
                for l in new_path :
                    # print >> sys.stderr, "l", l
                    # l = tuple( list( l ) + [ n ] )
                    l =  l + ( n, )
                    res.add( l )
                    # l.append( n )

                # paths[ n ] = new_path
                paths[ n ] = res

                nodes_to_visit.append( n )

            elif ( dist_nodes[ cur_n ] + 1 < dist_nodes[ n ] ):
                    
                res = set()
                dist_nodes[ n ] = dist_nodes[ cur_n ] + 1

                new_path = cp.deepcopy( paths[ cur_n ] )
                for l in new_path :
                    # l = tuple( list( l ) + [ n ] ) 
                    l =  l + ( n,  )
                    res.add( l )
                    # l.append( n )

                # paths[ n ] = new_path
                paths[ n ] = res

            # elif ( dist_nodes[ cur_n ] + 1 == dist_nodes[ n ] ):

            #     new_path = cp.deepcopy( paths[ cur_n ] )
            #     for l in new_path :
            #         l.append( n )

            #     paths[ n ] = paths[ n ] + new_path
        
        if ( dist_nodes[ n_g ] ) :
            for n in nodes_to_visit :
                if ( dist_nodes[ n_g ] <= dist_nodes[ n ] ):
                    nodes_to_visit.remove( n )
                    
    return dist_nodes[ n_g ], list( paths[ n_g ] )


def dijkstra_si_save( n_s, nodes, links, n_g ) :
    # print >> sys.stderr, "n_s", n_s
    # print >> sys.stderr, "n_g", n_g

    if ( n_s == n_g ) :
        return None, []
    
    dist_nodes = {}
    # print >> sys.stderr, "nodes", nodes
    for n in nodes :
        if ( n != n_s ) :
            dist_nodes[ n ] = None
        else :
            dist_nodes[ n ] = 0

    # print >> sys.stderr, "dist_nodes[ n_g ]", dist_nodes[ n_g ]
    nodes_to_visit = [ n_s ]

    # print >> sys.stderr, "nodes_to_visit", nodes_to_visit
    paths        = {}
    paths[ n_s ] = [ [ n_s ] ]
    paths[ n_g ] = None

    while ( nodes_to_visit ):
        
        cur_n          = nodes_to_visit.pop( 0 )
        # l_n_v_ng       = get_not_visited_neighbourhood( cur_n, links, dist_nodes )
        l_n_v_ng       = get_neighbourhood( cur_n, links )

        # print >> sys.stderr, "l_n_v_ng", l_n_v_ng
        # print >> sys.stderr, "cur_n", cur_n
        # print >> sys.stderr, "paths[ cur_n ]", paths[ cur_n ]

        for n in l_n_v_ng :

            # print >> sys.stderr, "n", n
            # print >> sys.stderr, "dist_nodes[ n ]", dist_nodes[ n ]
            # print >> sys.stderr, "not dist_nodes[ n ]", not dist_nodes[ n ]

            if ( dist_nodes[ n ] is None ):

                dist_nodes[ n ] = dist_nodes[ cur_n ] + 1

                new_path = cp.deepcopy( paths[ cur_n ] )
                for l in new_path :
                    l.append( n )

                paths[ n ] = new_path

                nodes_to_visit.append( n )

            elif ( dist_nodes[ cur_n ] + 1 < dist_nodes[ n ] ):
                    
                dist_nodes[ n ] = dist_nodes[ cur_n ] + 1

                new_path = cp.deepcopy( paths[ cur_n ] )
                for l in new_path :
                    l.append( n )

                paths[ n ] = new_path

            elif ( dist_nodes[ cur_n ] + 1 == dist_nodes[ n ] ):

                new_path = cp.deepcopy( paths[ cur_n ] )
                for l in new_path :
                    l.append( n )

                paths[ n ] = paths[ n ] + new_path
        
        if ( dist_nodes[ n_g ] ) :
            for n in nodes_to_visit :
                if ( dist_nodes[ n_g ] <= dist_nodes[ n ] ):
                    nodes_to_visit.remove( n )
                    
    return dist_nodes[ n_g ], paths[ n_g ]


def get_accessible_nodes( links ):
    tmp_links = map( lambda x : list( x ), links )
    return list ( set( reduce( lambda x, y : x + y, tmp_links ) ) )


def get_node_next_gateway( nodes, links, gateways ):
    l_node_n_g = []
    
    for node_g in gateways :
        for link in links :
            if ( node_g in link ) :
                node_n_g = None
                if ( node_g == link[ 0 ] ) :
                    node_n_g = link[ 1 ]
                else :
                    node_n_g = link[ 0 ]

                l_node_n_g.append( node_n_g )

    l_node_n_g = list( set( l_node_n_g ) )
                
    return l_node_n_g


def get_right_link( link, links ):
    res = None

    for ln in links:
        if ( ( ln == link ) or ( ln == link[ :: -1 ] ) ) :
            res = ln
            break

    return res


def get_better_potential_next_node_aux( wk_pot_path, nodes,
                                        links, gateways ) :

    if ( len( wk_pot_path.values() ) == 0 ):
        # return wk_pot_path.keys()[ 0 ]
        return wk_pot_path.keys()[ 0 ][ 0 ]
    
    lead_origin_node = {}
    
    # remove the first node
    for k, v in wk_pot_path.items() :
        # v = v[ 1 : ]
        lead_origin_node[ v[ 0 ] ] = k
        # lead_origin_node[ v[ 0 ] ] = k[ 0 ]

    if ( verbose ):
        print >> sys.stderr, "lead_origin_node", lead_origin_node

    potential_nodes = map( lambda x : x[ 0 ], wk_pot_path.values() )

    d_dist = {}
    for pot_node in potential_nodes :
        for node_g in gateways :
            dist, path = dijkstra_si( pot_node, nodes, links, node_g )
            d_dist[ pot_node ] = d_dist.get( pot_node, 0 ) + dist

    dist_min = min ( d_dist.values() )

    if ( verbose ):
        print >> sys.stderr, "d_dist", d_dist

    l_elt_d_dist = [ ( k , v ) for ( k, v ) in d_dist.items() ]

    elected_nodes = filter( lambda t : t[ 1 ] == dist_min,
                            l_elt_d_dist )

    if ( len( elected_nodes ) == 1 ) :
        # return lead_origin_node[ elected_nodes[ 0 ][ 0 ] ]
        return lead_origin_node[ elected_nodes[ 0 ][ 0 ] ][ 0 ]

    else :
        elected_pot_path = {}

        for n, dist in elected_nodes :
            origin_n = lead_origin_node[ n ]
            elected_pot_path[ origin_n ] = wk_pot_path[ origin_n ]

        return get_better_potential_next_node_aux( elected_pot_path, nodes,
                                                   links, gateways )
    

def get_better_potential_next_node( potential_path, nodes,
                                    links, gateways ) :

    node_res = None

    potential_path = map( lambda x : x [ 1 : ], potential_path )
    
    wk_pot_path = {}
    for p in potential_path :
        wk_pot_path[ p[ 0 ], hash( tuple( p ) ) ] = p[ : ]

    if ( verbose ):
        print >> sys.stderr, "wk_pot_path", wk_pot_path
        
    return get_better_potential_next_node_aux( wk_pot_path, nodes,
                                               links, gateways )


def update_after_delete( links, gateways, node_next_gateways ):

    accessible_nodes = get_accessible_nodes( links )

    for n_g in gateways:
        if ( n_g not in accessible_nodes ):
            gateways.remove( n_g )

    for n_n_g in node_next_gateways:
        if( n_n_g not in accessible_nodes ):
            node_next_gateways.remove( n_n_g )


def get_next_agent_node( si_next, nodes, tmp_links, tmp_gateways ):

    l_c_path = get_l_c_path( si_next, nodes, tmp_links, tmp_gateways )

    if ( verbose ):
    # if ( True ):
        print >> sys.stderr, "l_c_path", l_c_path
        # print >> sys.stderr, "tmp_links", tmp_links
        # print >> sys.stderr, "tmp_gateways", tmp_gateways

    dist_min = l_c_path[0][0]

    potential_path = filter( lambda l : l[0] == dist_min,
                             l_c_path )

    potential_path = map( lambda x : x[2], potential_path )

    res = []
    for l in potential_path :
        res.append( l )

    res = reduce( lambda x, y : x + y, potential_path )

    if ( verbose ):
        print >> sys.stderr, "res", res
    
    potential_path = res

    if ( verbose ):
        print >> sys.stderr, "potential_path", potential_path

    elected_node = None
    if ( len( potential_path ) == 1 ):
        elected_node = potential_path[ 0 ][ 1 ]
    elif ( reduce( lambda x, y : x and y , 
                   map( lambda x : x[ 1 ] == potential_path[ 0 ][ 1 ],
                        potential_path ) ) ):
        if ( verbose ):
            print >> sys.stderr, "FROZAR PASS 2"
        elected_node = potential_path[ 0 ][ 1 ]        
    else:
        elected_node = get_better_potential_next_node( potential_path, nodes,
                                                   tmp_links, tmp_gateways )

    if ( verbose ):
        print >> sys.stderr, "elected_node", elected_node

    return elected_node
    

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

nodes    = []
links    = []
gateways = []

# n: the total number of nodes in the level, including the gateways
# l: the number of links
# e: the number of exit gateways
n, l, e = [int(i) for i in raw_input().split()]
for i in xrange(l):
    # n1: N1 and N2 defines a link between these nodes
    n1, n2 = [int(j) for j in raw_input().split()]
    links.append( (n1, n2) )
    nodes.append( n1 )
    nodes.append( n2 )

nodes = sorted( list( set( nodes ) ) )

for i in xrange(e):
    ei = int(raw_input())  # the index of a gateway node
    gateways.append( ei )

origin_links = cp.deepcopy( links )
# print >> sys.stderr, "origin_links", origin_links

node_next_gateways = get_node_next_gateway( nodes, links, gateways )
print >> sys.stderr, "node_next_gateways", node_next_gateways

# verbose = False
verbose = True

# game loop
while True:
    si = int( raw_input() )

    if ( verbose ):
        print >> sys.stderr, "si", si
    
    l_c_path = get_l_c_path( si, nodes, links, gateways )

    if ( verbose ):
        print >> sys.stderr, "l_c_path", l_c_path

    dist_t, link_score, all_path_t = l_c_path[ 0 ]
    one_path_t = all_path_t[ 0 ]

    if ( verbose ):
        print >> sys.stderr, "one_path_t", one_path_t
    
    link_to_delete = None

    if ( 2 <= dist_t ) :

        l_critical_nodes = get_critical_nodes( nodes, links, gateways )

        if ( verbose ):
            print >> sys.stderr, "l_critical_nodes", l_critical_nodes

        # case if there is a critical node (occurence >= 2) in the graph
        if ( 2 <= len( l_critical_nodes ) ) :
        
            tmp_links              = links[ : ]
            tmp_gateways           = gateways[ : ]
            tmp_node_next_gateways = node_next_gateways[ : ]
            
            # Simulate the next position of the agent
            si_next = one_path_t[ 1 ]
            
            if ( verbose ):
                print >> sys.stderr, "si_next", si_next
            
            # print >> sys.stderr, "FROZAR before critical_node"
            critical_node = get_next_critical_node( si_next,
                                                    tmp_links,
                                                    tmp_gateways,
                                                    tmp_node_next_gateways )
            # print >> sys.stderr, "FROZAR after critical_node", critical_node
            
            # link_to_delete = get_critical_link_to_delete( critical_node,
            #                                               tmp_gateways,
            #                                               tmp_links )
            link_to_delete = get_critical_link_to_delete( critical_node,
                                                          gateways,
                                                          links )
    
            
            if ( verbose ):
                print >> sys.stderr, "link_to_delete", link_to_delete
    
            print link_to_delete[ 0 ], link_to_delete[ 1 ]

            # verbose = False
            
        elif( len( l_critical_nodes ) == 1 ):

            critical_node = l_critical_nodes.pop()

            link_to_delete = get_critical_link_to_delete( critical_node,
                                                          gateways,
                                                          links )
            
            print link_to_delete[ 0 ], link_to_delete[ 1 ]

            
        else :
            
            link_to_delete = one_path_t[ -2 ], one_path_t[ -1 ]

            link_to_delete = get_right_link( link_to_delete, links )
            print link_to_delete[ 0 ], link_to_delete[ 1 ]

    else:

        link_to_delete = one_path_t[ -2 ], one_path_t[ -1 ]

        link_to_delete = get_right_link( link_to_delete, links )
        print link_to_delete[ 0 ], link_to_delete[ 1 ]

    
    if ( verbose ):
    # if ( True ):
        print >> sys.stderr, "link_to_delete", link_to_delete
        print >> sys.stderr, "links", links
    
    links.remove( link_to_delete )
    update_after_delete( links, gateways, node_next_gateways )

