import sys

filename = sys.argv[ 1 ]

fp = open( filename )
l_file = fp.readlines( )
l_file = map( lambda x : int( x ), l_file )

nb_cases = l_file.pop( 0 )

for i_case, N in enumerate( l_file ):
    if ( N == 0 ) :
        print "Case #%d: INSOMNIA" % ( i_case + 1 )
        continue

    s_seen_digit = set()

    factor = 1
    
    while( len( s_seen_digit ) < 10 ) :
        res_N = factor * N
        str_N = str( res_N )

        for c in str_N :
            s_seen_digit.add( c )

        factor += 1

    print "Case #%d: %d" % ( i_case + 1, res_N )
