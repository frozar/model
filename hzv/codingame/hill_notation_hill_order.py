
from sys import stderr


def parse_compound( compound ) :
    l_elt = []
    cur_elt = ""
    cur_occurence = ""
    for c in compound :
        if ( c.isupper() ) :

            if ( cur_occurence == "" ) :
                l_elt.append( [ cur_elt, 1 ] )
            else :
                l_elt.append( [ cur_elt, int( cur_occurence ) ] )
                
            cur_elt = c
            cur_occurence = ""

        elif( c.islower() ) :
            cur_elt += c

        elif( c.isdigit() ) :
            cur_occurence += c

    if ( cur_occurence == "" ) :
        l_elt.append( [ cur_elt, 1 ] )
    else :
        l_elt.append( [ cur_elt, int( cur_occurence ) ] )
        
    l_elt.pop( 0 )

    return l_elt


def hill_notation( compound ) :
    l_elt = []
    while ( "(" in compound ) :
        ind_p_open  = compound.index( "(" )
        ind_p_close = compound.index( ")" )
        
        l_elt += parse_compound( compound[ : ind_p_open ] )

        l_p_elt = parse_compound( compound[ ind_p_open + 1 : ind_p_close ] )
        factor  = int( compound[ ind_p_close + 1 ] )
        l_p_elt = map( lambda c : [ c[ 0 ], c[ 1 ] * factor ], l_p_elt )

        l_elt += l_p_elt

        compound = compound[ ind_p_close + 2 : ]
    
    l_elt += parse_compound( compound )
    
    # Check for "doublon"
    l_elt.sort()

    cur_elt = l_elt[ 0 ]
    l_res = []
    for elt in l_elt[ 1 : ] :
        if ( cur_elt[ 0 ] == elt[ 0 ] ) :
            cur_elt[ 1 ] += elt[ 1 ]
        else :
            l_res.append( cur_elt )
            cur_elt = elt
    l_res.append( cur_elt )

    l_elt = l_res

    # Sort the finally writting
    for elt in l_elt :
        if ( elt[ 0 ] == 'C' ) :
            l_elt.remove( elt )
            l_elt.insert( 0, elt )
            break
    
    for elt in l_elt :
        if ( elt[ 0 ] == 'H' ) :
            if ( l_elt[ 0 ][ 0 ] == 'C' ) :
                l_elt.remove( elt )
                l_elt.insert( 1, elt )
            break

    res = ""
    for elt in l_elt :
        if ( elt[ 1 ] == 1 ) :
            res += elt[ 0 ]
        else :
            res += elt[ 0 ] + str( elt[ 1 ] )

    return res


def cmp_compound( compound1, compound2 ) :

    l_elt1 = parse_compound( compound1 )
    l_elt2 = parse_compound( compound2 )

    if ( l_elt1 < l_elt2 ) :
        return -1
    elif( l_elt1 > l_elt2 ) :
        return 1
    else :
        if ( len( l_elt1 ) < len( l_elt2 ) ) :
            return -1
        elif ( len( l_elt1 ) > len( l_elt2 ) ) :
            return 1
        else :
            return 0


l_compound = []

num_compounds = int( raw_input() )
for i in xrange( num_compounds ):
    l_compound.append( raw_input() )


l_compound.sort()
l_compound = map( lambda x : hill_notation( x ), l_compound )

l_compound = list( set( l_compound ) )

l_compound.sort( cmp = cmp_compound )

print "\n".join( l_compound )

