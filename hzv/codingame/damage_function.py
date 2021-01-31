

import math as m

A = 125000.0

def f( x ) :
    return int( round( A / ( m.sqrt( x ) ) ** 1.2 ) )

x = ( 16000 ** 2 + 9000 ** 2 ) / 2

for i in range( 1, 151 ) :

    while( f( x ) <= i ) :
        x -= 1

    print x #, f( x )


