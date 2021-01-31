import sys
import math

import copy as cp


def get_neighbourhood( cur_n, links ) :
    l_neighbour=[]
    for ln in links:
        if ( cur_n == ln[ 0 ] ) :
            l_neighbour.append( ln[ 1 ] )

        if ( cur_n == ln[ 1 ] ) :
            l_neighbour.append( ln[ 0 ] )

    return l_neighbour


def get_not_visited_neighbourhood( cur_n, links, c_nodes ) :
    l_not_visited_neighbour=[]

    l_neighbour = get_neighbourhood( cur_n, links )
    # print >> sys.stderr, "l_neighbour", l_neighbour
    
    # print >> sys.stderr, "c_nodes", c_nodes

    for n in l_neighbour:
        if ( not c_nodes[ n ] ):
            l_not_visited_neighbour.append( n )

    # print >> sys.stderr, "l_not_visited_neighbour", l_not_visited_neighbour

    return l_not_visited_neighbour


def dijkstra( n_s, nodes, links, n_g ) :
    # print >> sys.stderr, "n_s", n_s
    # print >> sys.stderr, "n_g", n_g

    if ( n_s == n_g ) :
        return 0, []

    
    c_nodes = {}
    # print >> sys.stderr, "nodes", nodes
    for n in nodes :
        if ( n != n_s ) :
            c_nodes[ n ] = None
        else :
            c_nodes[ n ] = 0

    # print >> sys.stderr, "c_nodes[ n_g ]", c_nodes[ n_g ]
    nodes_to_visit = [ n_s ]

    # print >> sys.stderr, "nodes_to_visit", nodes_to_visit

    paths        = {}
    paths[ n_s ] = [ n_s ]
    paths[ n_g ] = None

    while ( nodes_to_visit ):
        
        # print >> sys.stderr, "nodes_to_visit", nodes_to_visit

        cur_n          = nodes_to_visit.pop( 0 )
        l_n_v_ng       = get_not_visited_neighbourhood( cur_n, links, c_nodes )

        # print >> sys.stderr, "l_n_v_ng", l_n_v_ng
    
        # print >> sys.stderr, "cur_n", cur_n
        # print >> sys.stderr, "paths[ cur_n ]", paths[ cur_n ]

        for n in l_n_v_ng :
            # print >> sys.stderr, "n", n
            # print >> sys.stderr, "c_nodes[ n ]", c_nodes[ n ]
            # print >> sys.stderr, "not c_nodes[ n ]", not c_nodes[ n ]

            if ( not c_nodes[ n ] ):
                c_nodes[ n ] = c_nodes[ cur_n ] + 1
                paths[ n ]   = paths[ cur_n ] + [ n ]
                nodes_to_visit.append( n )
            else :
                if ( c_nodes[ cur_n ] + 1 < c_nodes[ n ] ):
                    c_nodes[ n ] = c_nodes[ cur_n ] + 1
                    paths[ n ]   = paths[ cur_n ] + [ n ]

            # print >> sys.stderr, "nodes_to_visit", nodes_to_visit
    
        # nodes_to_visit.remove( cur_n )
        
        if ( c_nodes[ n_g ] ) :
            for n in nodes_to_visit :
                if ( c_nodes[ n_g ] <= c_nodes[ n ] ):
                    nodes_to_visit.remove( n )

    return c_nodes[ n_g ], paths[ n_g ]


def get_accessible_nodes( links ):
    tmp_links = map( lambda x : list( x ), links )
    return list ( set( reduce( lambda x, y : x + y, tmp_links ) ) )


def is_a_link( link, links ):

    # print >> sys.stderr, "IN is_a_link( link, links )"

    res    = False
    res_ln = None
    
    for ln in links:
        if ( ( ( ln[ 0 ] == link[ 0 ] ) and  ( ln[ 1 ] == link[ 1 ] ) ) or \
             ( ( ln[ 0 ] == link[ 1 ] ) and  ( ln[ 1 ] == link[ 0 ] ) ) ) :
            res    = True
            res_ln = ln
            # link   = ln
            break

    # print >> sys.stderr, "OUT is_a_link( link, links )"
    # print >> sys.stderr, "res_ln", res_ln

    # link   = res_ln
    # print >> sys.stderr, "IN id( link )", id( link )
    return res, res_ln


def better_link( si, links, gateways ):
    res_links = []

    for n_g in gateways:
        accessible_nodes = get_accessible_nodes( links )

        # get all nodes at distance 1 of n_g
        tmp_nodes = []
        for n_a in accessible_nodes:
            # if is_a_link( ( n_a, n_g ), links )[0] :
            if is_a_link( ( n_a, n_g ), origin_links )[0] :
                tmp_nodes.append( n_a )

        # get links between nodes at distance 1 of n_g
        for n1 in tmp_nodes:
            for n2 in tmp_nodes[ tmp_nodes.index( n1 ) + 1 : ] :
                ln = ( n1, n2 )
                if is_a_link( ln, links )[ 0 ] :
                    res_links.append( is_a_link( ln, links )[ 1 ] )
                    # res_links.append( ln )

    if ( not res_links ):
        return None
    
    score_link = []
    # Find the nearest links of 'si'
    for ln in res_links :
        n1 = ln[ 0 ]
        n2 = ln[ 1 ]

        # print >> sys.stderr, "n1", n1
        # print >> sys.stderr, "n2", n2
        # print >> sys.stderr, "dijkstra( si, nodes, links, n1 )", dijkstra( si, nodes, links, n1 )
        # print >> sys.stderr, "dijkstra( si, nodes, links, n2 )", dijkstra( si, nodes, links, n2 )
        # tmp_score = 0
        score_1 = dijkstra( si, nodes, links, n1 )[ 0 ]
        score_2 = dijkstra( si, nodes, links, n2 )[ 0 ]

        # if ( ( ( n1, n2 ) == ( 4, 5 ) ) or \
        #      ( ( n1, n2 ) == ( 5, 4 ) ) ) :
        #     print >> sys.stderr, "score_1", score_1
        #     print >> sys.stderr, "score_2", score_2
        #     print >> sys.stderr, "score_1 and score_2", score_1 and score_2
            
        
        # if ( score_1 and score_2 ):
        if ( ( score_1 is not None ) and ( score_2 is not None ) ):
            score_link.append( ( score_1 + score_2 , ln ) )
        
    score_link.sort()
    print >> sys.stderr, "res_links", res_links
    print >> sys.stderr, "score_link", score_link

    if ( score_link ):
        return score_link[ 0 ][ 1 ] #res_links
    else :
        return None


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

# game loop
while True:
    si = int(raw_input())  # The index of the node on which the Skynet agent is positioned this turn

    # print >> sys.stderr, "origin_links", origin_links

    l_c_path = []
    for n_g in gateways:
        # print >> sys.stderr, "n_g", n_g
        # print >> sys.stderr, "dijkstra( si, nodes, links, n_g )", dijkstra( si, nodes, links, n_g )
        c, path = dijkstra( si, nodes, links, n_g )
        if ( c ):
            l_c_path.append( ( c, path ) )

    l_c_path.sort()
            
    # print >> sys.stderr, "beginning"
    # print >> sys.stderr, "l_c_path", l_c_path
    
    # Write an action using print
    # To debug: print >> sys.stderr, "Debug messages..."

    # Example: 0 1 are the indices of the nodes you wish to sever the link between
    # print "1 2"

    link = None
    if ( l_c_path[0][0] == 1 ) :
        # print >> sys.stderr, "case 1"
        link = l_c_path[ 0 ][ 1 ][ -2 ], l_c_path[ 0 ][ 1 ][ -1 ]

    else:
        if ( l_c_path[0][0] == 2 ) :
            # l_better_links = better_link( si, links, gateways )
            better_ln = better_link( si, links, gateways )
            
            # if ( l_better_links ) :
            #     # print >> sys.stderr, "case 2"
            #     link = l_better_links[ 0 ]
            
            if ( better_ln ) :
                # print >> sys.stderr, "case 2"
                link = better_ln

            else:
                # print >> sys.stderr, "case 3"
                link = l_c_path[ 0 ][ 1 ][ 0 ], l_c_path[ 0 ][ 1 ][ 1 ]

        else:
            # print >> sys.stderr, "case 3"
            link = l_c_path[ 0 ][ 1 ][ 0 ], l_c_path[ 0 ][ 1 ][ 1 ]
        
    print link[ 0 ], link[ 1 ]

    # print >> sys.stderr, "remove link and node"
    
    # print >> sys.stderr, "link before", link
    # print >> sys.stderr, "OUT id( link )", id( link )
    if is_a_link( link, links )[ 0 ] :
        # print >> sys.stderr, "link after", link
        # print >> sys.stderr, "is_a_link( link, links )[ 1 ]", is_a_link( link, links )[ 1 ]
        links.remove( is_a_link( link, links )[ 1 ] )
        # links.remove( link )
        # print >> sys.stderr, "B FOR break"

    # print >> sys.stderr, "accessibility"

    accessible_nodes = get_accessible_nodes( links )
    # print >> sys.stderr, "accessible_nodes", accessible_nodes

    # print >> sys.stderr, "accessibility next"
    for n_g in gateways:
        if ( n_g not in accessible_nodes ):
            gateways.remove( n_g )

    # print >> sys.stderr, "end"
