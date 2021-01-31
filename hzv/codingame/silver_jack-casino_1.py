import math
rounds = int( raw_input() )
cash   = int( raw_input() )
plays  = []
for i in xrange( rounds ) :
    plays.append( raw_input().split() )
for play in plays :
    bet = int( math.ceil( cash / 4.0 ) )
    ball, call, nb = ( int( play[ 0 ] ), play[ 1 ], int( play[ 2 ] ) ) if ( len( play ) == 3 ) else ( int( play[ 0 ] ), play[ 1 ], None )
    if ( ( ball != 0 ) and ( ( call == "EVEN" and ( ball % 2 ) == 0 ) or ( call == "ODD" and ( ball % 2 ) == 1 ) ) ) :
        cash += bet
    elif ( call == "PLAIN" and ball == nb ) :
        cash += 35 * bet
    else :
        cash -= bet
print cash
