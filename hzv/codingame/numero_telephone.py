import sys
import math

def add_nb( d_chiffre, telephone, chiffre ) :

    if not ( chiffre in d_chiffre.keys() ) :
        d_chiffre[ chiffre ] = {}

    if not ( telephone ) :
        return
    
    new_d_chiffre = d_chiffre[ chiffre ]
    chiffre = telephone.pop( 0 )
    add_nb( new_d_chiffre, telephone, chiffre )


def get_nb_chiffre( d_chiffre ) :
    if ( d_chiffre.keys() == 0 ) :
        return 0
    
    res = len( d_chiffre.keys() )

    for tmp_d_chiffre in d_chiffre.values() :
        res += get_nb_chiffre( tmp_d_chiffre )

    return res
    

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

d_all_nb = {}

n = int(raw_input())

for i in xrange(n):
    telephone = raw_input()
    telephone = map( lambda x : int( x ), list( telephone ) )
    # print >> sys.stderr, "telephone", telephone

    chiffre = telephone.pop( 0 )
    add_nb( d_all_nb, telephone, chiffre )

    
# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

# print >> sys.stderr, "d_all_nb", d_all_nb

# The number of elements (referencing a number) stored in the structure.
print get_nb_chiffre( d_all_nb )
