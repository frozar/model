import sys

filename = sys.argv[ 1 ]

fp = open( filename )
l_file = fp.readlines( )

nb_cases = int( l_file.pop( 0 ) )

l_file = map( lambda x : x.strip(), l_file )

for i_case, s_stack in enumerate( l_file ):
    # print i_case, s_stack

    nb_manip = 0
    
    while ( "-" in s_stack ) :
        first_c = s_stack[ 0 ]

        len_group = 0
        for c in s_stack :
            if ( c == first_c ) :
                len_group += 1
            else :
                break

        # print "beg ", s_stack
        
        to_reverse = reversed( s_stack[ : len_group ] )
        # print to_reverse
        # print "TOTO", s_stack[ : len_group ]
        to_reverse = map( lambda x : "+" if x == "-" else "-", to_reverse )
        # print to_reverse
        to_reverse = reduce( lambda x, y : x + y, to_reverse )
        # print to_reverse
        s_stack = to_reverse + s_stack[ len_group : ]

        # print "end ", s_stack
        
        nb_manip += 1

        # if ( nb_manip > 4 ) :
        #     exit()
        
    print "Case #%d: %d" % ( i_case + 1, nb_manip )

    # nb_manip = 0

    # cp_stack = s_stack.rstrip( "+" )

    # if ( cp_stack == "" ) :
    #     print "Case #%d: %d" % ( i_case + 1, nb_manip )
    #     continue
    
    # prev_c = cp_stack[ 0 ]
    # for c in cp_stack[ 1 : ] :
    #     if ( c != prev_c ) :
    #         if ( c == "+" ) :
    #             nb_manip += 1
    #         else :
    #             nb_manip += 2

    #     prev_c = c

    # if ( nb_manip == 0 ) :
    #     if ( s_stack[ 0 ] == "-" ):
    #         nb_manip += 1
                
    # print "Case #%d: %d" % ( i_case + 1, nb_manip )

