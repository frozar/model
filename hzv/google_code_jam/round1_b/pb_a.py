import sys

def get_nb_2_aux( s_phone_nb, cp_s, i_start, res ):
    cp_s = s_phone_nb[ : ]

    # print "s_phone_nb", s_phone_nb
    i = i_start
    while( i < 10 ):
        wk_cp_s = cp_s[ : ]
        
        l_c = d_nb[ i ]

        b_res = True
        for c in l_c :
            if ( c in cp_s ):
                cp_s.remove( c )
            else :
                b_res = False
                break

        if b_res :
            res.append( i )
            wk_cp_s = cp_s[ : ]

        else :
            cp_s = wk_cp_s[ : ]
            i += 1
    

def get_nb_2( s_phone_nb, cp_s, i_start, res ):
    # cp_s = s_phone_nb[ : ]

    # # print "s_phone_nb", s_phone_nb
    # i = i_start
    # while( i < 10 ):
    #     wk_cp_s = cp_s[ : ]
        
    #     l_c = d_nb[ i ]

    #     b_res = True
    #     for c in l_c :
    #         if ( c in cp_s ):
    #             cp_s.remove( c )
    #         else :
    #             b_res = False
    #             break

    #     if b_res :
    #         res.append( i )
    #         wk_cp_s = cp_s[ : ]

    #     else :
    #         cp_s = wk_cp_s[ : ]
    #         i += 1

    get_nb_2_aux( s_phone_nb, cp_s, i_start, res )
    
    # if ( 0 < len( cp_s ) ):
    
    while ( 0 < len( cp_s ) ):
        if ( res ):
            nb = res.pop( -1 )
            i_start = nb + 1
            cp_s += list( d_nb[ nb ] )
            
        else:
            i_start += 1
            
        get_nb_2_aux( cp_s[ : ], cp_s, i_start, res )

    return res, cp_s

    
def get_nb( s_phone_nb, cp_s, i_start, res ):
    cp_s = s_phone_nb[ : ]

    # print "s_phone_nb", s_phone_nb
    i = i_start
    while( i < 10 ):
        wk_cp_s = cp_s[ : ]
        
        l_c = d_nb[ i ]

        b_res = True
        for c in l_c :
            if ( c in cp_s ):
                cp_s.remove( c )
            else :
                b_res = False
                break

        if b_res :
            res.append( i )
            wk_cp_s = cp_s[ : ]

        else :
            cp_s = wk_cp_s[ : ]
            i += 1

        # print 'i', i, "res", res
        # print "cp_s", cp_s

    # res.sort()

    if ( 0 < len( cp_s ) ):
        if ( res ):
            nb = res.pop( -1 )
            i_start = nb + 1
            cp_s += list( d_nb[ nb ] )
            
            # print i_start
            # print s_phone_nb
            # print cp_s
            # print res
            # print ""

            # if ( res == [ 2, 4, 7 ] ):
            #     exit()
            
        else:
            i_start += 1

            # if ( 10 < i_start ):
            #     print s_phone_nb
            #     print cp_s
            #     print res
            #     exit()
            
        # return get_nb( s_phone_nb, cp_s, i_start, res )
        return get_nb( cp_s[ : ], cp_s, i_start, res )

    else :
        return res, cp_s


filename = sys.argv[ 1 ]

fp = open( filename )
l_file = fp.readlines( )

nb_cases = int( l_file.pop( 0 ) )
l_file = map( lambda x : x.strip(), l_file )

l_nb = "ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE"

d_nb = {}

for i, s_nb in enumerate( l_nb ) :
    d_nb[ i ] = s_nb

# print d_nb

l_res = []
for s_phone_nb in l_file :
    # print s_phone_nb
    origin_phone_nb = list( s_phone_nb )
    s_phone_nb = origin_phone_nb[ : ]
    cp_s = s_phone_nb[ : ]

    i_start = 0
    res = []
    while( ( 0 < len( cp_s ) ) and ( i_start < 10 ) ):
        res, cp_s = get_nb_2( origin_phone_nb, cp_s, i_start, res )
        i_start += 1
    
    # print 'cp_s', cp_s
    
    l_res.append( res )
    # print res

# print l_res

for i, phone_nb in enumerate( l_res ):
    print "Case #%d:" % ( i + 1),
    res = ''
    print ''.join( map( lambda x : str( x ), phone_nb ) )
