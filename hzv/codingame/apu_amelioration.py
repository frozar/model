import sys
import math

# The machines are gaining ground. Time to show them what we're really made of...

err_channel = sys.stderr
err_channel = open('/dev/null', 'w')

def parse( line, y, nodes ):
    for x, c in enumerate( line ):
        if ( c != "." ):
            c_int = int( c )
            nodes[ (x, y) ] = c_int


def voisin( n, nodes ):
    res = []
    
    l_nodes = nodes.keys()
    l_nodes.remove( n )
    
    # get right node
    right_n = None
    for n_loop in l_nodes:
        if ( n_loop[1] != n[1] ):
            continue
        if ( n[0] < n_loop[0] ):
            if ( not right_n ):
                right_n = n_loop
            else:
                if ( n_loop[0] < right_n[0] ):
                    right_n = n_loop

    if ( right_n ):
        res.append( right_n )

    # get left node
    left_n = None
    for n_loop in l_nodes:
        if ( n_loop[1] != n[1] ):
            continue
        if ( n_loop[0] < n[0] ):
            if ( not left_n ):
                left_n = n_loop
            else:
                if ( left_n[0] < n_loop[0]  ):
                    left_n = n_loop

    if ( left_n ):
        res.append( left_n )

    # get top node
    top_n = None
    for n_loop in l_nodes:
        if ( n_loop[0] != n[0] ):
            continue
        if ( n[1] < n_loop[1] ):
            if ( not top_n ):
                top_n = n_loop
            else:
                if ( n_loop[1] < top_n[1] ):
                    top_n = n_loop

    if ( top_n ):
        res.append( top_n )

    # get bottom node
    bottom_n = None
    for n_loop in l_nodes:
        if ( n_loop[0] != n[0] ):
            continue
        if ( n_loop[1] < n[1] ):
            if ( not bottom_n ):
                bottom_n = n_loop
            else:
                if ( bottom_n[1] < n_loop[1] ):
                    bottom_n = n_loop

    if ( bottom_n ):
        res.append( bottom_n )

    return res


def possible_voisin( n, nodes, connexions ):
    l_res = []
    all_nodes = voisin( n, nodes )

    for n_loop in all_nodes:
        if ( nodes[ n_loop ] > 0 ):
            if( possible_connexion( n, n_loop, connexions ) ):
                l_res.append( n_loop )

    return l_res


def possible_vertical_connexion( n1, n2, connexions ):
    j_min = min( n1[1], n2[1] )
    j_max = max( n1[1], n2[1] )

    for cnx in connexions :
        if ( n1[0] == cnx[0] ):
            if ( j_min < cnx[1] ) and ( cnx[1] < j_max ):
                return False
        elif ( n1[0] == cnx[2] ):
            if ( j_min < cnx[3] ) and ( cnx[3] < j_max ):
                return False
        elif ( cnx[0] < n1[0] ) and ( n1[0] < cnx[2] ):
            if ( ( j_min <= cnx[1] ) and ( cnx[1] <= j_max ) ) or \
                    ( ( j_min <= cnx[3] ) and ( cnx[3] <= j_max ) ):
                return False

    return True


def possible_horizontal_connexion( n1, n2, connexions ):
    i_min = min( n1[0], n2[0] )
    i_max = max( n1[0], n2[0] )

    for cnx in connexions :
        if ( n1[1] == cnx[1] ) :
            if ( i_min < cnx[0] ) and ( cnx[0] < i_max ):
                return False
        elif ( n1[1] == cnx[3] ) :
            if ( i_min < cnx[2] ) and ( cnx[2] < i_max ):
                return False
        elif ( cnx[1] < n1[1] ) and ( n1[1] < cnx[3] ):
            if ( ( i_min <= cnx[0] ) and ( cnx[0] <= i_max ) ) or \
                    ( ( i_min <= cnx[2] ) and ( cnx[2] <= i_max ) ):
                return False

    return True


def count_connexion( n1, n2, connexions ):
    count = 0

    for cnx in connexions:
        c_n1 = cnx[0], cnx[1]
        c_n2 = cnx[2], cnx[3]
        if ( ( n1, n2 ) == ( c_n1, c_n2 ) ) or \
                ( ( n1, n2 ) == ( c_n2, c_n1 ) ) :
            count += cnx[4]

    return count


def possible_connexion( n1, n2, connexions ):
    if ( n1[0] == n2[0] ):
        if ( possible_vertical_connexion( n1, n2, connexions ) ):
            if( count_connexion( n1, n2, connexions ) < 2 ):
                return True
    if ( n1[1] == n2[1] ):
        if ( possible_horizontal_connexion( n1, n2, connexions ) ):
            if( count_connexion( n1, n2, connexions ) < 2 ):
                return True

    return False


def add_connexion( n_src, n_target, connexions, forbidden ):
    max_nb_lien = min ( nodes[ n_src ], nodes[ n_target ] )

    c = 2 - count_connexion( n_src, n_target, connexions )
    max_nb_lien = min ( max_nb_lien, c )

    for nb_lien in range( max_nb_lien, 0, -1 ):
        cnx = ( n_src[0], n_src[1], n_target[0], n_target[1], nb_lien )
        # if ( not cnx in forbidden[ n_src ] ):
        #     connexions.append( cnx )
        #     dec_nodes( nodes, n_src, n_target, nb_lien )
        #     return True

        l_forbidden = forbidden.get( len( connexions ), [] )
        if ( not cnx in l_forbidden ):
            connexions.append( cnx )
            dec_nodes( nodes, n_src, n_target, nb_lien )
            return True

    return False


def dec_nodes( nodes, n1, n2, nb_lien ):
    nodes[ n1 ] -= nb_lien
    nodes[ n2 ] -= nb_lien


def inc_nodes( nodes, n1, n2, nb_lien ):
    nodes[ n1 ] += nb_lien
    nodes[ n2 ] += nb_lien


def discard_last_connexion( connexions, nodes, forbidden ):
    # forbidden[ len( connexions ) ] = []

    l_forbidden = forbidden.get( len( connexions ) -1, [] )
    l_forbidden.append( connexions[ -1 ] )
    forbidden[ len( connexions ) -1 ] = l_forbidden

    last_cnx = connexions.pop()
    
    n1 = ( last_cnx[ 0 ], last_cnx[ 1 ] )
    n2 = ( last_cnx[ 2 ], last_cnx[ 3 ] )
    nb_lien = last_cnx[ 4 ]

    inc_nodes( nodes, n1, n2, nb_lien )

    return last_cnx


def backtrack( n, connexions, nodes, n_path, forbidden ):
    print >> err_channel, connexions
    print >> err_channel, "n_path", n_path
    print >> err_channel, "forbidden", forbidden
    last_cnx = discard_last_connexion( connexions, nodes, forbidden )

    prev_n = n_path.pop()

    # if ( prev_n != n ):
    #     for i, l_forbidden in forbidden.items():
    #         good_forbidden = []
    #         for cnx in l_forbidden:
    #             if( ( cnx[0], cnx[1] ) != n ):
    #                 good_forbidden.append( cnx )
    #         forbidden[ i ] = good_forbidden

    # if ( prev_n == n ):
    #     forbidden[ n ].append( last_cnx )
    # else:
    #     forbidden[ prev_n ].append( last_cnx )
    #     forbidden[ n ] = []

    n = prev_n

    print >> err_channel, connexions
    print >> err_channel, "n_path", n_path
    print >> err_channel, "forbidden", forbidden

    return n


def compute_next_set( connexions, nodes ):
    known_node = set()

    for cnx in connexions:
        n1 = ( cnx[0], cnx[1] )
        n2 = ( cnx[2], cnx[3] )
        known_node.add( n1 )
        known_node.add( n2 )

    l_known_node = list( known_node )

    res = set()
    for n in l_known_node:
        if( nodes[ n ] > 0 ):
            res.add( n )

    return res


width  = int(raw_input()) # the number of cells on the X axis
height = int(raw_input()) # the number of cells on the Y axis

nodes = {}

# Read input
for i in xrange( height ):
    line = raw_input() # width characters, each either a number or a '.'

    parse( line, i, nodes )
    print >> err_channel, line


# Compute the connexions
connexions = []
forbidden  = {}
# n_next     = set()
n_path     = []

# for n in nodes.keys() :
#     forbidden [ n ] = []

print >> err_channel, "nodes", nodes

n = sorted( [ ( v, k ) for k, v in nodes.items() ], reverse = True )[0][1]

while ( sum( nodes.values() ) > 0 ):
    print >> err_channel, "node", n, nodes[ n ]

    if ( nodes[ n ] == 0 ):
        n_next = compute_next_set( connexions, nodes )

        if ( not n_next ):
            print >> err_channel, "no next node"
            n = backtrack( n, connexions, nodes, n_path, forbidden )

        else:
            n = n_next.pop()

        continue

    # Get the positif voisin
    l_possible = possible_voisin( n, nodes, connexions )
    print >> err_channel, "l_possible", n, l_possible

    if ( not l_possible ):
        print >> err_channel, "no l_possible"
        n = backtrack( n, connexions, nodes, n_path, forbidden )
        continue

    has_add = False
    for n_tmp in l_possible:
        # If a connexion have been achieved
        if ( add_connexion( n, n_tmp, connexions, forbidden ) ):
            has_add = True
            print >> err_channel, "add connexions", connexions
            n_path.append( n )

            for i in forbidden.keys():
                if ( i >= len( connexions ) ):
                    forbidden[ i ] = []

            break

    if( not has_add ):
        print >> err_channel, "no has_add"
        n = backtrack( n, connexions, nodes, n_path, forbidden )
        continue

print >> err_channel, "final connexions", connexions

# Two coordinates and one integer: a node, one of its neighbors, the number of links connecting them.
# print "0 0 2 0 1"

# Print the connexions
for cnx in connexions:
    for i in cnx:
        print i, 
    print ""
