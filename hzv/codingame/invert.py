
import numpy as np
import copy as cp
import atexit
import sys

diffusion=[
0xf26cb481,0x16a5dc92,0x3c5ba924,0x79b65248,0x2fc64b18,0x615acd29,0xc3b59a42,0x976b2584,
0x6cf281b4,0xa51692dc,0x5b3c24a9,0xb6794852,0xc62f184b,0x5a6129cd,0xb5c3429a,0x6b978425,
0xb481f26c,0xdc9216a5,0xa9243c5b,0x524879b6,0x4b182fc6,0xcd29615a,0x9a42c3b5,0x2584976b,
0x81b46cf2,0x92dca516,0x24a95b3c,0x4852b679,0x184bc62f,0x29cd5a61,0x429ab5c3,0x84256b97
]

confusion= [
0xac,0xd1,0x25,0x94,0x1f,0xb3,0x33,0x28,0x7c,0x2b,0x17,0xbc,0xf6,0xb0,0x55,0x5d,
0x8f,0xd2,0x48,0xd4,0xd3,0x78,0x62,0x1a,0x02,0xf2,0x01,0xc9,0xaa,0xf0,0x83,0x71,
0x72,0x4b,0x6a,0xe8,0xe9,0x42,0xc0,0x53,0x63,0x66,0x13,0x4a,0xc1,0x85,0xcf,0x0c,
0x24,0x76,0xa5,0x6e,0xd7,0xa1,0xec,0xc6,0x04,0xc2,0xa2,0x5c,0x81,0x92,0x6c,0xda,
0xc6,0x86,0xba,0x4d,0x39,0xa0,0x0e,0x8c,0x8a,0xd0,0xfe,0x59,0x96,0x49,0xe6,0xea,
0x69,0x30,0x52,0x1c,0xe0,0xb2,0x05,0x9b,0x10,0x03,0xa8,0x64,0x51,0x97,0x02,0x09,
0x8e,0xad,0xf7,0x36,0x47,0xab,0xce,0x7f,0x56,0xca,0x00,0xe3,0xed,0xf1,0x38,0xd8,
0x26,0x1c,0xdc,0x35,0x91,0x43,0x2c,0x74,0xb4,0x61,0x9d,0x5e,0xe9,0x4c,0xbf,0x77,
0x16,0x1e,0x21,0x1d,0x2d,0xa9,0x95,0xb8,0xc3,0x8d,0xf8,0xdb,0x34,0xe1,0x84,0xd6,
0x0b,0x23,0x4e,0xff,0x3c,0x54,0xa7,0x78,0xa4,0x89,0x33,0x6d,0xfb,0x79,0x27,0xc4,
0xf9,0x40,0x41,0xdf,0xc5,0x82,0x93,0xdd,0xa6,0xef,0xcd,0x8d,0xa3,0xae,0x7a,0xb6,
0x2f,0xfd,0xbd,0xe5,0x98,0x66,0xf3,0x4f,0x57,0x88,0x90,0x9c,0x0a,0x50,0xe7,0x15,
0x7b,0x58,0xbc,0x07,0x68,0x3a,0x5f,0xee,0x32,0x9f,0xeb,0xcc,0x18,0x8b,0xe2,0x57,
0xb7,0x49,0x37,0xde,0xf5,0x99,0x67,0x5b,0x3b,0xbb,0x3d,0xb5,0x2d,0x19,0x2e,0x0d,
0x93,0xfc,0x7e,0x06,0x08,0xbe,0x3f,0xd9,0x2a,0x70,0x9a,0xc8,0x7d,0xd8,0x46,0x65,
0x22,0xf4,0xb9,0xa2,0x6f,0x12,0x1b,0x14,0x45,0xc7,0x87,0x31,0x60,0x29,0xf7,0x73,
0x2c,0x97,0x72,0xcd,0x89,0xa6,0x88,0x4c,0xe8,0x83,0xeb,0x59,0xca,0x50,0x3f,0x27,
0x4e,0xae,0x43,0xd5,0x6e,0xd0,0x99,0x7b,0x7c,0x40,0x0c,0x52,0x86,0xc1,0x46,0x12,
0x5a,0x28,0xa8,0xbb,0xcb,0xf0,0x11,0x95,0x26,0x0d,0x34,0x66,0x22,0x18,0x6f,0x51,
0x9b,0x3b,0xda,0xec,0x5e,0x00,0x2a,0xf5,0x8f,0x61,0xba,0x96,0xb3,0xd1,0x30,0xdc,
0x33,0x75,0xe9,0x6d,0xc8,0xa1,0x3a,0x3e,0x5f,0x9d,0xfd,0xa9,0x31,0x9f,0xaa,0x85,
0x2f,0x92,0xaf,0x67,0x78,0xa5,0xab,0x03,0x21,0x4f,0xb9,0xad,0xfe,0xf3,0x42,0xfc,
0x17,0xd7,0xee,0xa3,0xd8,0x80,0x14,0x2e,0xa0,0x47,0x55,0xc4,0xff,0xe5,0x13,0x3f,
0x81,0xb6,0x7a,0x94,0xd0,0xb5,0x54,0xbf,0x91,0xa7,0x37,0xf1,0x6b,0xc9,0x1b,0xb1,
0x3c,0xb6,0xd9,0x32,0x24,0x8d,0xf2,0x82,0xb4,0xf9,0xdb,0x7d,0x44,0xfb,0x1e,0xd4,
0xea,0x5d,0x35,0x69,0x23,0x71,0x57,0x01,0x06,0xe4,0x55,0x9a,0xa4,0x58,0x56,0xc7,
0x4a,0x8c,0x8a,0xd6,0x6a,0x49,0x70,0xc5,0x8e,0x0a,0x62,0xdc,0x29,0x4b,0x42,0x41,
0xcb,0x2b,0xb7,0xce,0x08,0xa1,0x76,0x1d,0x1a,0xb8,0xe3,0xcc,0x7e,0x48,0x20,0xe6,
0xf8,0x45,0x93,0xde,0xc3,0x63,0x0f,0xb0,0xac,0x5c,0xba,0xdf,0x07,0x77,0xe7,0x4e,
0x1f,0x28,0x10,0x6c,0x59,0xd3,0xdd,0x2d,0x65,0x39,0xb2,0x74,0x84,0x3d,0xf4,0xbd,
0xc7,0x79,0x60,0x0b,0x4d,0x33,0x36,0x25,0xbc,0xe0,0x09,0xcf,0x5b,0xe2,0x38,0x9e,
0xc0,0xef,0xd2,0x16,0x05,0xbe,0x53,0xf7,0xc2,0xc6,0xa2,0x24,0x98,0x1c,0xad,0x04
]


def print_vec( i, str_vec, vec ) :
    print " i - %3d:" %  ( i ), str_vec +" :",
    for elt in vec:
        print "%3d" % ( elt ),
    print ""


def forbidden( confusion ):
    amoung = range( 256 )
    res = []
    reverse_ind = 0
    for i in range( 256 ):
        try:
            reverse_ind = confusion.index( i )
        except:
            res.append( i )
    return res


def print_mat( P ):
    for i in range( len(P) ):
        print "%2d" % (i), ":",
        for j in range( len(P[i]) ):
            print "%1d" % (P[ i ][ j ]),
        print ""


def reverse_confusion( d ):
    global confusion

    res = [ -1 ] * len( d )
    for i in range( len( d ) ):
        if ( 255 < d[ i ] ):
            print "in reverse IMPOSSIBLE: d[",  i, "]", d[ i ], res[ i ]
            continue

        res[ i ] = confusion.index( d[ i ] )
        if ( 255 < res[ i ] ):
            print "in reverse PB: d[",  i, "]", d[ i ], res[ i ]

    return res


def check_init_target( target, target_ascii ):
    for i in range( len( target_ascii ) ):

        target_val = ( target_ascii[ i ] ^ output_state[ i ] ) % 256
        complement = output_state[ i ] % 256

        while( ( target_val in forbidden_1 ) or 
               ( complement in forbidden_2 ) or 
               ( 255 < target_val ) or 
               ( 255 < complement ) ):
    
            next_output_state( output_state, i )

            target_val = ( target_ascii[ i ] ^ output_state[ i ] ) % 256
            complement = output_state[ i ] % 256

    res = [ 0 ] * 32
    for i in range( 16 ):
        res[ 2 * i     ] = ( target_ascii[ i ] ^ output_state[ i ] ) % 256
        res[ 2 * i + 1 ] = output_state[ i ] % 256

    return res


output_state = [ 0 ] * 16

def next_output_state_aux( output_state, i ):
    if ( len( output_state ) <= i ):
        print " Every possible output have been tried"
        exit( 0 )

    if ( output_state[ i ] != 255 ):
        output_state[ i ] += 1
        return
    else:
        output_state[ i ] = 0
        next_output_state_aux( output_state, i + 1 )
        return


def next_output_state( output_state, i ):
    next_output_state_aux( output_state, i )


def next_output( target_ascii ):
    next_output_state( output_state, 0 )

    for i in range( len( target_ascii ) ):

        target_val = ( target_ascii[ i ] ^ output_state[ i ] ) % 256
        complement = output_state[ i ] % 256

        while( ( target_val in forbidden_1 ) or 
               ( complement in forbidden_2 ) or 
               ( 255 < target_val ) or 
               ( 255 < complement ) ):
    
            next_output_state( output_state, i )

            target_val = ( target_ascii[ i ] ^ output_state[ i ] ) % 256
            complement = output_state[ i ] % 256


    res = [ 0 ] * 32
    for i in range( 16 ):
        res[ 2 * i     ] = ( target_ascii[ i ] ^ output_state[ i ] ) % 256
        res[ 2 * i + 1 ] = output_state[ i ] % 256

    output = [ 0 ] * 32
    for i in range( 16 ):
        output[ i * 2 ]     = confusion.index( res[ 2 * i ] )
        output[ i * 2 + 1 ] = confusion[ 256 : ].index( res[ 2 * i + 1 ] )

    if( not check_output( output, target_ascii ) ):
        print "  Bad output", final_step( output )
        print "  target", target
        exit( 0 )

    return output


def final_step( output ):
    res = [0] * 16
    for i in range( 16 ):
        res[ i ] = confusion[ output[ i * 2 ] ] ^ \
            confusion[ output[ i * 2 + 1 ] + 256 ]
    return res


def check_output( output, target_ascii ):
    res = True
    for i in range( 16 ):
        if( ( confusion[ output[ i * 2 ] ] ^ 
              confusion[ output[ i * 2 + 1 ] + 256 ] ) != target_ascii[ i ] ):
            res = False
            break
    return res


def matrice_xor( A, X ):
    res = [ 0 ] * len( A )
    for i in range( len( A ) ):
        for j in range( len( A[ i ] ) ):
            res[ i ] ^= A[i][j] * X[ j ]

    for i in range( len( A ) ):
        res[ i ] = res[ i ] % 256

    return res


def fine_pop( dic, key ):
    if( key in dic ):
        dic.pop( key )


def analyse_c( c, ind, multiple_ante, dict_tack ):
    all_possibility = []
    for i, ante in enumerate( c ):
        for imag, antes in multiple_ante.items():
            if ( ante in antes ):
                alternative_ante = None
                if ( ante != antes[ 0 ] ):
                    alternative_ante = antes[ 0 ]
                else:
                    alternative_ante = antes[ 1 ]
                    
                possibility = [ i, ante, alternative_ante ]

                all_possibility.append( possibility )

    fine_pop( dict_track, ind )

    state_possibility = [ 0 ] * len( all_possibility )

    dict_track[ ind ] = state_possibility, all_possibility


def next_state_possibility_aux( state_possibility, i ):
    if ( len( state_possibility ) <= i ):
        return False

    if ( state_possibility[ i ] == 0 ):
        state_possibility[ i ] += 1
        return True
    else:
        state_possibility[ i ] = 0
        return next_state_possibility_aux( state_possibility, i + 1 )


def next_state_possibility( state_possibility ):
    return next_state_possibility_aux( state_possibility, 0 )


def next_c( c, ind, multiple_ante, dict_track ):
    find = False

    # test if there is a next 'c'
    state_possibility, all_possibility = dict_track[ ind ]

    if( next_state_possibility( state_possibility ) ):
        find = True

        for i, ind_ante in enumerate( state_possibility ):
            ind, ante_0, ante_1 = all_possibility[ i ]
            if ( ind_ante == 0 ):
                c[ ind ] = ante_0
            if ( ind_ante == 1 ):
                c[ ind ] = ante_1

    return find


def check_xor( c, P, d ):
    xor_res = matrice_xor( P, d )

    res = True
    for elt_0, elt_1 in zip( xor_res, c ):
        if( elt_0 != elt_1 ):
            res = False
            break

    return res


def confusion_func( c ):
    res = [ 0 ] * len( c )

    for i in range( len( res ) ):
        res[ i ] = confusion[ c[ i ] ]
    
    return res


def check_confusion( d, c ):
    confusion_res = confusion_func( c )

    res = True
    for elt_0, elt_1 in zip( confusion_res, d ):
        if( elt_0 != elt_1 ):
            res = False
            break
    return res


def possible_reverse_confusion( d ):
    res = True
    for elt in d:
        if( elt in forbidden_1 ):
            res = False
            break
    return res


# Global variable used in "find_input"
max_ind_ = 0
all_c = []
for i in range( 256 ):
    all_c.append( [] )


def find_input( c, nb_loop, multiple_ante, dict_track, P ):
    ind = 0

    while ( ( 0 <= ind ) and ( ind < nb_loop ) ):
        global max_ind_

        if ( ind > max_ind_ ):
            max_ind_ = ind
            print "max_ind_ =", max_ind_

        # print_vec( ind, "c", c )
        d = matrice_xor( P, c )
        # print_vec( ind, "d", d )

        # if ( not check_xor( c, P, d ) ):
        #     print " Error check_xor"
        #     print "c  ", c
        #     print "XOR", matrice_xor( P, d )
        #     print ""
        #     print "output_state", output_state
        #     exit( 0 )

        # If 'd' is impossible to be obtained by reverse_confusion()
        if ( not possible_reverse_confusion( d ) ):

            # You don't have many chose on the first input
            if ( ind == 0 ):
                ind -= 1
                continue

            # Walk down through as many level as it is necessary to find
            # one which allow a next step exploration
            while( ( 0 <= ind ) and 
                   ( not next_c( c, ind, multiple_ante, dict_track ) ) ):

                # Clean the dictionary entry
                fine_pop( dict_track, ind )

                # Go the previous level
                ind -= 1

                # Restore the previous vector 'c'
                c = cp.deepcopy( all_c[ ind ] )

                # Special exit condition
                if ( ind <= 0 ):
                    return [ -1 ]

            continue


        # Save the current 'c' vector at level 'ind'
        all_c[ ind ] = cp.deepcopy( c )

        # Get 'c' by reverse_confusion()
        c = reverse_confusion( d )

        # if ( not check_confusion( d, c ) ):
        #     print " Error check_confusion"
        #     print "d", d
        #     print "confusion( c )", confusion_func( c )
        #     exit( 0 )

        # Go to the next step
        ind += 1

        if( ind <= 255 ):
            # Save the first 'c' vector at the new level 'ind'
            all_c[ ind ] = cp.deepcopy( c )

            # Analyse the different possiblity for the vector 'c'
            analyse_c ( c, ind, multiple_ante, dict_track )

    if ( ind == nb_loop ):
        return c
    else:
        return [ -1 ]


def init_output( target_ascii ):
    output = [0] * 32
    target = [0] * 32

    for i in range( 16 ):
        target[ 2*i ]     = target_ascii[ i ]
        target[ 2*i + 1 ] = 0

    target = check_init_target( target, target_ascii )

    for i in range( 16 ):
        output[ i * 2 ]     = confusion.index( target[ 2 * i ] )
        output[ i * 2 + 1 ] = confusion[ 256 : ].index( target[ 2 * i + 1 ] )

    if( not check_output( output, target_ascii ) ):
        print "Bad output", output
        print "target", target
        exit( 0 )
    
    return output




P = [ [ diffusion[i] >> k & 1 for k in range( 0 , 32 )] for i in range( 0, 32 ) ]

# target = "Hire me!!!!!!!!"
# target = "Reverse me fast"
target = "fabien.rozar   "

target_ascii = map( lambda x : ord( x ) , target )
target_ascii.append( 0 )

# print "target_ascii"
# print " ".join( map( lambda x : str( x ) , target_ascii ) )

forbidden_1 = forbidden( confusion[ : 256 ] )
forbidden_2 = forbidden( confusion[ 256 : ] )
forbidden   = forbidden_1 + forbidden_2

# print "forbidden", sorted( forbidden )
# print "forbidden_1", sorted( forbidden_1 )
# print "forbidden_2", sorted( forbidden_2 )

output = init_output( target_ascii )
# print "output", output

multiple_ante = {}

image_confusion = [ 0 ] * 256
image_occurence = {}
for i in range( 256 ):
    image_confusion[ i ] = confusion[ i ]
    multiple_ante[ image_confusion[ i ] ] = \
        multiple_ante.get( image_confusion[ i ], [] ) + [ i ]
    image_occurence[ image_confusion[ i ] ] = image_occurence.get( image_confusion[ i ], 0 ) + 1

for k, v in multiple_ante.items() :
    if ( len( v ) < 2 ):
        multiple_ante.pop( k )

dict_track = {}
c = cp.deepcopy( output )
nb_loop = 256

sol = find_input( c, nb_loop, multiple_ante, dict_track, P )

while( sol[ 0 ] == -1 ):
    c   = next_output( target_ascii )
    sol = find_input( c, nb_loop, multiple_ante, dict_track, P )

print "Final input_vec"
sol_str = map( lambda x : format( x, "#04x") , sol )
final_s  = ",".join( sol_str[ : 16 ] )
final_s += ","
print final_s

final_s  = ",".join( sol_str[ 16 : ] )
print final_s

