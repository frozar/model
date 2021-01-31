import sys

def possible_line_i( item, i, columns ):
    res = True
    # print "i", i
    # print item
    # print columns
    for ii in range( i ):
        if ( ( columns[ ii ] is not None ) and
             ( item[ ii ] != columns[ ii ][ i ] ) ):
            res = False
            break

    return res


def possible_column_i( item, i, lines ):
    res = True
    for ii in range( i ):
        if ( ( lines[ ii ] is not None ) and
             ( item[ ii ] != lines[ ii ][ i ] ) ):
            res = False
            break

    return res


def solve_2 ( i_t, lines_n_columns ):
    # print lines_n_columns
    l_item = sorted( lines_n_columns )
    print l_item

    N = ( len( l_item ) + 1 ) / 2

    lines   = [ None ] * N
    columns = [ None ] * N

    compte = 0
    
    for i, item in enumerate( l_item ) :
        print "item", item
        done = False

        while ( not done ) :
            rank = compte / 2

            print "compte", compte
            
            print "BEGIN: lines", lines
            print "BEGIN: columns", columns
            
            if ( ( lines[ rank ] is None ) and
                 possible_line_i( item, rank, columns ) ):
                lines[ rank ] = item
                done = True
                print "in lines"
                
            elif ( ( columns[ rank ] is None ) and
                   possible_column_i( item, rank, lines ) ):
                columns[ rank ] = item
                done = True
                print "in columns"

            print "END: lines", lines
            print "END: columns", columns

            compte += 1
        
    # print lines
    # print zip(lines[0], lines[1], lines[2])
    # print zip(*lines)
    # print columns

    item_res = None
    if ( None in lines ):
        good_rank = lines.index( None )
        item_res = zip( *columns )
        item_res = item_res[ good_rank ]
        
    else :
        good_rank = columns.index( None )
        item_res = zip( *lines )
        item_res = item_res[ good_rank ]
        
    print "Case #%d:" % ( i_t + 1),
    for i in item_res :
        print i,
    print ""

    return

def solve ( i_t, lines_n_columns ):
    d_match = {}
    
    for item_1 in lines_n_columns :
        tmp_lines_n_columns = lines_n_columns[ : ]
        tmp_lines_n_columns.remove( item_1 )
    
        len_liste = len( tmp_lines_n_columns )
        
        # print "toto", lines_n_columns
        # print "titi", tmp_lines_n_columns
    
        for item_2 in tmp_lines_n_columns :
    
            if ( ( ( item_1, item_2 ) in d_match.keys() ) or \
                 ( ( item_2, item_1 ) in d_match.keys() ) ):
                break
    
            tmp_res = []
            
            for ind in xrange( len( item_1 ) ):
    
                # print "ind", ind
                # print "item_1", item_1
                # print "item_2", item_2
                
                if ( item_1[ ind ] == item_2[ ind ] ) :
                    tmp_res.append( ind )
                
                # print "tmp_res", tmp_res
                
                # if ( item_1[ ind ] == item_2[ ind ] ) :
                #     d_match[ item_1 ] = ind
                #     d_match[ item_2 ] = ind
                #     break
    
            if ( tmp_res ) :
                d_match[ item_1, item_2 ] = tmp_res[ : ]
                
    
    # for k, v in d_mathc.items():
    #     if ( v == [] ):
    # print d_match

    l_keys = d_match.keys()
    l_keys = map( lambda x : list( x ), l_keys )
    l_keys = reduce( lambda x, y : x + y, l_keys )
    l_keys = set( l_keys )
    
    # print l_keys
    res_l = None
    print "lines_n_columns", lines_n_columns
    for item in lines_n_columns :
        if ( item not in l_keys ):
            res_l = item
            break

    print "res_l", res_l
        
    l_sort = map( lambda x : x[ 0 ], sorted( d_match.keys() ) )
    # print l_sort

    l_sort.append( res_l )
    l_sort.sort( )
    
    my_rang = l_sort.index( res_l )

    # print "my_rang", my_rang
    
    l_keys = sorted( d_match.keys() )
    print l_keys

    real_l_res = None
    for i, k in enumerate( l_keys ):
        item_1, item_2 = k
        print item_1, item_2
        if ( item_1[ my_rang ] != res_l[ i ] ):
            # print "item_1[ my_rang ]", item_1[ my_rang ]
            real_l_res = list( res_l[ : i ] ) + [ item_1[ my_rang ] ] + list( res_l[ i + 1 : ] )
            break
        if ( item_2[ my_rang ] != res_l[ i ] ):
            # print "item_2[ my_rang ]", item_2[ my_rang ]
            real_l_res = list( res_l[ : i ] ) + [ item_2[ my_rang ] ] + list( res_l[ i + 1 : ] )
            # print real_l_res
            break
    
    # for item in lines_n_columns :
    #     if ( item not in l_keys ):
    print "Case #%d:" % ( i_t ),
    for i in real_l_res :
        print i,
    print ""


filename = sys.argv[ 1 ]

fp = open( filename )
l_file = fp.readlines( )

nb_cases = int( l_file.pop( 0 ) )
# l_file = map( lambda x : x.strip(), l_file )

all_case = {}

for i_t in xrange( nb_cases ) :
    N = int( l_file.pop( 0 ) )

    l_lines_n_columns = []
    for i in xrange( 2*N - 1 ):
        i_lines_n_columns = l_file.pop( 0 ).split()
        i_lines_n_columns = tuple( map( lambda y : int( y ),  i_lines_n_columns ) )

        l_lines_n_columns.append( i_lines_n_columns )
    
    all_case[ i_t ] = l_lines_n_columns

# lines_n_columns = map( lambda x : x.strip(), l_file )
# lines_n_columns = map( lambda x : tuple( map( lambda y : int( y ),  x.split() ) ), l_file )
# print l_lines_n_columns
# print all_case[ 1 ]

for k, v in all_case.items() :
    solve_2 ( k, v )
    exit()

