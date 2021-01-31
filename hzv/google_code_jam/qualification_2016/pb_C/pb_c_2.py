import sys
import copy as cp
import math
import numpy as np

# all_prime_nb = set()

all_not_prime_nb = {}

def is_prime( n ) :
    res = True
    div = None

    # if ( n in all_prime_nb ) :
    #     return res, div

    # print n
    if ( crible_table[ n ] ) :
        return res, div

    if ( n in all_not_prime_nb.keys() ) :
        return False, all_not_prime_nb[ n ]
    
    limit = int( math.ceil( math.sqrt( n ) ) )
    for i in xrange( 3, limit + 1, 2 ):
        # print "n", n, "i", i
        # print "( n % i )", ( n % i )
        # print "( ( n % i ) == 0 )", ( ( n % i ) == 0 )
        if ( ( n % i ) == 0 ) :
            res = False
            div = i

            all_not_prime_nb[ n ] = div

            break

    # if ( res ):
    #     all_prime_nb.add( n )

    return res, div


def next_jc( jc ) :
    to_evol = jc[ 1 : -1 ]
    
    res  = int( to_evol, 2 )
    # print "res1", res
    res += 1
    # print "res2", res
    res  = bin( res )[ 2 : ]
    # print "res3", res
    res = "0" * ( len( to_evol ) - len( res ) ) + res
    # print "res4", res
    res = "1" + res + "1"
    
    return res
        

filename = sys.argv[ 1 ]
crible_filename = sys.argv[ 2 ]

crible_table = np.load( crible_filename )

# print crible_table

# exit()

fp = open( filename )
l_file = fp.readlines( )

nb_cases = int( l_file.pop( 0 ) )

# print nb_cases

line_n_j = l_file.pop( 0 )
n, j = tuple( map( lambda x : int( x ), line_n_j.split() ) )

# print n, j

origin_jc = "1" + "0" * ( n-2 ) + "1"

cur_jc = cp.deepcopy( origin_jc )

nb_good_jc = 0

d_res_jc = {}
i_dbg = 0

while ( nb_good_jc < j ) :
    # print cur_jc
    
    right = True
    l_div = []
    
    for i in xrange( 2, 11 ) :
        # print int( cur_nb, i )
        jc_in_base_i = int( cur_jc, i )

        # print "   BASE ", i
        res, div = is_prime( jc_in_base_i )
        if ( res ) :
            # print "base", i
            # print "jc_in_base_i", jc_in_base_i
            # print "prime", res
            right = False
            break

        else :
            l_div.append( div )

    if ( right ) :
        # print "GET A RIGHT"
        d_res_jc[ cur_jc ] = l_div[ : ]
        nb_good_jc += 1

        if ( ( nb_good_jc % 10 ) == 0 ) :
            print >> sys.stderr, "nb_good_jc", nb_good_jc
        # print d_res_jc
        
    cur_jc = next_jc( cur_jc )
    # else :
    #     cur_jc = next_jc( cur_jc )
    #     # print "cur_jc", cur_jc

    i_dbg += 1

    # if ( i_dbg > 2 ) :
    #     exit()

# print d_res_jc
print "Case #1:"
for k, l_div in d_res_jc.items() :
    print k,
    for div in l_div :
        print div,
    print ""
