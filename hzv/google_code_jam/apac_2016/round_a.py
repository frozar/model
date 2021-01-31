import sys

def get_set( w, set_0, set_1 ):
    if ( w in set_0 ):
        return set_0
    else:
        return set_1

    
def put_other_set( w, w_set, set_0, set_1 ) :
    if ( w_set == set_0 ):
        set_1.add( w )
    else :
        set_0.add( w )
        
    
# print sys.argv

filename = sys.argv[ 1 ]

fp = open( filename )
l_file = fp.readlines( )

# print l_file

nb_cases = int( l_file.pop( 0 ) )
# print nb_cases

# int( l_file.pop( 0 ) )
# print tuple( l_file.pop( 0 ).split() )
# exit()

d_cases = {}

for i_case in xrange( nb_cases ) :
    nb_pair = int( l_file.pop( 0 ) )

    l = []
    for i_pair in xrange( nb_pair ) :
        l.append( tuple( l_file.pop( 0 ).split() ) )

    d_cases[ i_case ] = l

# print d_cases[ 0 ]

for i_case, l_pair in d_cases.items():
    d_trouble = {}
    
    for pair in l_pair:
        w_0, w_1 = pair

        d_trouble[ w_0 ] = d_trouble.get( w_0, [] ) + [ w_1 ]
        d_trouble[ w_1 ] = d_trouble.get( w_1, [] ) + [ w_0 ]

    # print d_trouble

    set_0 = set()
    set_1 = set()

    res = True

    for w, l_w in d_trouble.items() :
        b_0 = False
        b_1 = False

        for tmp_w in l_w :
            if tmp_w in set_0 :
                b_0 = True
            elif tmp_w in set_1 :
                b_1 = True

        if ( b_0 and b_1 ) :
            res = False
        elif ( b_0 and not b_1 ) :
            set_1.add( w )
        elif ( not b_0 and b_1 ) :
            set_0.add( w )
        else :
            set_0.add( w )
            
    print "Case #%d: %s" % ( i_case, "Yes" if res else "No" )
    # print set_0
    # print set_1
    

    # if ( i_case == 2 ):
    #     exit()

    #     if ( ( w_0 not in set_0 ) and ( w_0 not in set_1 ) and \
    #          ( w_1 not in set_0 ) and ( w_1 not in set_1 ) ) :
    #         set_0.add( w_0 )
    #         set_1.add( w_1 )

    #     elif( ( ( w_0 in set_0 ) or ( w_0 in set_1 ) ) and \
    #           ( w_1 not in set_0 ) and ( w_1 not in set_1 ) ) :
    #         w_0_set = get_set( w_0, set_0, set_1 )
    #         put_other_set( w_1, w_0_set, set_0, set_1 )

    #     elif( ( ( w_1 in set_0 ) or ( w_1 in set_1 ) ) and \
    #           ( w_0 not in set_0 ) and ( w_0 not in set_1 ) ) :
    #         w_1_set = get_set( w_1, set_0, set_1 )
    #         put_other_set( w_0, w_1_set, set_0, set_1 )

    #     elif ( ( ( w_0 in set_0 ) and ( w_1 in set_0 ) ) or \
    #            ( ( w_0 in set_1 ) and ( w_1 in set_1 ) ) ) :
    #         res = False
    #         break
        
    # print "Case #%d: %s" % ( i_case, "Yes" if res else "No" )
    # print set_0
    # print set_1

    # if ( i_case == 2 ):
    #     exit()
