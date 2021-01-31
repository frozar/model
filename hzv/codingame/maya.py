import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

def get_new_nb( nb, s ):
    new_nb = []
    tmp_nb = None

    if ( h == 1 ) :
        new_nb = nb[ : ]

    else :
        for i in range( s ) :
            if ( i % h == 0 ) :
                tmp_nb = nb[ i ][ : ]

            elif ( i % h == ( h - 1 ) ) :
                tmp_nb += nb[ i ][ : ]
                new_nb.append( tmp_nb )
        
            else :
                tmp_nb += nb[ i ][ : ]
            
    new_nb.reverse()
    return new_nb


def numberToBase(n, b) :
    if n == 0:
        return [0]
    digits = []
    while n:
        digits.append(int(n % b))
        n /= b
    return digits[::-1]


all_maya_nb = []

d_maya_to_number = {}
d_number_to_maya = {}

nb_1 = []
nb_2 = []

l, h = [int(i) for i in raw_input().split()]

# print >> sys.stderr, "l", l
# print >> sys.stderr, "h", h

for i in xrange(h):
    numeral = raw_input()
    all_maya_nb.append( numeral )

# print >> sys.stderr, "all_maya_nb", all_maya_nb

s1 = int(raw_input())
for i in xrange(s1):
    num_1line = raw_input()
    nb_1.append( num_1line )

# print >> sys.stderr, "nb_1", nb_1

s2 = int(raw_input())
for i in xrange(s2):
    num_2line = raw_input()
    nb_2.append( num_2line )
    
# print >> sys.stderr, "nb_2", nb_2

operation = raw_input()

# print >> sys.stderr, "s1", s1
# print >> sys.stderr, "s2", s2

for l_all_maya_nb in all_maya_nb :
    for i in range( 20 ) :
        d_number_to_maya[ i ] = d_number_to_maya.get( i, '' ) + l_all_maya_nb[ i * l : ( i + 1 ) * l ]

for nb, maya_str in d_number_to_maya.items() :
    d_maya_to_number[ maya_str ] = nb

# print >> sys.stderr, "d_number_to_maya", d_number_to_maya
# print >> sys.stderr, "d_maya_to_number", d_maya_to_number

nb_1 = get_new_nb( nb_1, s1 )
nb_2 = get_new_nb( nb_2, s2 )

l_nb_1 = map( lambda maya : d_maya_to_number[ maya ], nb_1 )
l_nb_2 = map( lambda maya : d_maya_to_number[ maya ], nb_2 )

# print >> sys.stderr, "l_nb_1", l_nb_1
# print >> sys.stderr, "l_nb_2", l_nb_2

dec_nb_1 = 0
for i, maya_digit in enumerate( l_nb_1 ) :
    dec_nb_1 += maya_digit * ( 20 ** i )

dec_nb_2 = 0
for i, maya_digit in enumerate( l_nb_2 ) :
    dec_nb_2 += maya_digit * ( 20 ** i )

# print >> sys.stderr, "dec_nb_1", dec_nb_1
# print >> sys.stderr, "dec_nb_2", dec_nb_2

res = None

if ( operation == "+" ) :
    res = dec_nb_1 + dec_nb_2
    
elif ( operation == "-" ) :
    res = dec_nb_1 - dec_nb_2

elif ( operation == "*" ) :
    res = dec_nb_1 * dec_nb_2

elif ( operation == "/" ) :
    res = dec_nb_1 / dec_nb_2

# print >> sys.stderr, "res", res
# print >> sys.stderr, "numberToBase( res, 20 )", numberToBase( res, 20 )

l_maya_res = numberToBase( res, 20 )
# print >> sys.stderr, "l_maya_res", l_maya_res

# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

for maya_digit in l_maya_res :
    maya_str = d_number_to_maya[ maya_digit ]
    # print >> sys.stderr, "maya_str", maya_str
    for i in range( len( maya_str ) / l ) :
        print maya_str[ i * l : ( i + 1 ) * l ]

# print "result"
# print 3
