import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

d_alphabet = {}

l = int(raw_input())
h = int(raw_input())
t = raw_input()
for i in xrange(h):
    row = raw_input()
    l_i_char = [ None ] * 27 
#    print >> sys.stderr, "l_i_char", l_i_char
    
    for j, c in enumerate( row ):
        ind = j / l
#        print >> sys.stderr, "ind", ind
        l_char = l_i_char[ ind ]
        if ( not l_char ):
            l_char = []
        l_char.append( c )
        l_i_char[ ind ] = l_char
#        print >> sys.stderr, "l_char", l_char
#        print >> sys.stderr, "l_i_char", l_i_char

    for ind in range( 27 ):
        l_res = d_alphabet.get( ind + ord( 'A' ), [] )
        l_res.append( l_i_char[ ind ] )
        d_alphabet[ ind + ord( 'A' ) ] = l_res
#        print >> sys.stderr, "l_res", l_res

#print >> sys.stderr, "d_alphabet", d_alphabet

t = t.upper()

to_print = map( lambda x : ord( x ), t )

# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

for i in xrange(h):
    for order in to_print:
        if not ( ( ord('A') <= order ) and ( order <= ord('Z') ) ):
            for c in d_alphabet[ ord('A') + 27 - 1 ][ i ]:
                sys.stdout.write(c)
        else:
            for c in d_alphabet[ order ][ i ]:
                sys.stdout.write(c)
    print ""

