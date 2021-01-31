import sys
import math

def get_l_somme( c, budgets ) :

    l_somme = []
    
    for i, b in enumerate( budgets ) :
        nb_b         = len( budgets[ i : ] )

        equi_somme   = float( c ) / nb_b
        b_is_integer = equi_somme.is_integer()
        equi_somme   = int( equi_somme )
    
        if ( budgets[ i ] < equi_somme ) :
            c -= budgets[ i ]
            l_somme.append( budgets[ i ] )

        else :
            if ( b_is_integer ) :
                l_somme += [ equi_somme ] * nb_b
            else : 
                l_somme += [ equi_somme ] * nb_b
                c -= equi_somme * nb_b

                while ( 0 < c ) :
                    for ii in range( len( l_somme ) - 1, i - 1, -1 ) :
                        if( l_somme[ ii ] < budgets[ ii ] ) :
                            l_somme[ ii ] += 1
                            c -= 1
                        else :
                            break

                        if ( c <= 0 ) :
                            break

            break

    return l_somme
        
# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

budgets = []

n = int(raw_input())
c = int(raw_input())
for i in xrange(n):
    b = int(raw_input())
    budgets.append( b )

# print >> sys.stderr, "n", n
# print >> sys.stderr, "c", c
# print >> sys.stderr, "budgets", budgets

budgets.sort()

# print >> sys.stderr, "sorted budgets", budgets

# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

if ( sum ( budgets ) < c ) :
    print "IMPOSSIBLE"

else :
    l_somme = get_l_somme( c, budgets )

    for s in l_somme :
        print s
