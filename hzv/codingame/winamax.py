import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

def get_card_value( card ):
    card = card[ :-1 ]
    card_val = None
    if ( card.isdigit() ) :
        card_val = int( card )
    elif ( card == "J" ) :
        card_val = 11
    elif ( card == "Q" ) :
        card_val = 12
    elif ( card == "K" ) :
        card_val = 13
    elif ( card == "A" ) :
        card_val = 14
        
    return card_val


deckp_1 = []
deckp_2 = []

n = int(raw_input())  # the number of cards for player 1
for i in xrange(n):
    cardp_1 = raw_input()  # the n cards of player 1

    cardp_1_val = get_card_value( cardp_1 )        
    deckp_1.append( cardp_1_val )

m = int(raw_input())  # the number of cards for player 2
for i in xrange(m):
    cardp_2 = raw_input()  # the m cards of player 2

    cardp_2_val = get_card_value( cardp_2 )
    deckp_2.append( cardp_2_val )

# print >> sys.stderr, "deckp_1", deckp_1
# print >> sys.stderr, "deckp_2", deckp_2
    
# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

def run_a_round( deckp_1, deckp_2,
                 save_cardp_1 = None, save_cardp_2 = None,
                 verbose = False ):

    card_1 = deckp_1.pop( 0 )
    card_2 = deckp_2.pop( 0 )

    if ( save_cardp_1 is None ):
        save_cardp_1 = []
    if ( save_cardp_2 is None ):
        save_cardp_2 = []

    if ( card_1 < card_2 ):
        deckp_2 += save_cardp_1 + [ card_1 ] + \
                   save_cardp_2 + [ card_2 ]
        if verbose :
            print >> sys.stderr, "P2 deckp_1", deckp_1
            print >> sys.stderr, "P2 deckp_2", deckp_2
            
        return False

    elif ( card_1 > card_2 ):
        deckp_1 += save_cardp_1 + [ card_1 ] + \
                   save_cardp_2 + [ card_2 ]
        if verbose :
            print >> sys.stderr, "P1 deckp_1", deckp_1
            print >> sys.stderr, "P1 deckp_2", deckp_2
            
        return False

    else:
        if ( ( len( deckp_1 ) < 4 ) or ( len( deckp_2 ) < 4 ) ):
            print "PAT"
            return True

        else :
            save_cardp_1.append( card_1 )
            for i in range( 3 ):
                save_cardp_1.append( deckp_1.pop( 0 ) )
    
            save_cardp_2.append( card_1 )
            for i in range( 3 ):
                save_cardp_2.append( deckp_2.pop( 0 ) )
    
            return run_a_round( deckp_1, deckp_2,
                                save_cardp_1, \
                                save_cardp_2, \
                                verbose )
    

nb_round  = 0
pat_found = False

while ( ( deckp_1 != [] ) and ( deckp_2 != [] ) ):

    # to_print = ( deckp_1[ 0 ] == deckp_2[ 0 ] )
    # if to_print :
    #     print >> sys.stderr, "BEF deckp_1", deckp_1
    #     print >> sys.stderr, "BEF deckp_2", deckp_2

    # run_a_round( deckp_1, deckp_2, verbose = to_print )

    # if to_print :
    #     print >> sys.stderr, "AFT deckp_1", deckp_1
    #     print >> sys.stderr, "AFT deckp_2", deckp_2

    pat_found = run_a_round( deckp_1, deckp_2 )
    
    if ( pat_found ):
        break
    
    nb_round += 1
        
# print >> sys.stderr, "OUT deckp_1", deckp_1
# print >> sys.stderr, "OUT deckp_2", deckp_2

if ( not pat_found ):
    if ( deckp_1 and ( deckp_2 == [] ) ):
        print 1, nb_round
    elif ( deckp_2 and ( deckp_1 == [] ) ):
        print 2, nb_round
