import sys
import numpy as np

exponent = int( sys.argv[ 1 ] )

greatest_int = 2 ** exponent

crible_table = np.ones( greatest_int, np.bool8 )

crible_table[ 0 ] = np.False_
crible_table[ 1 ] = np.False_

crible_table[ 2 * 2 : greatest_int : 2 ] = np.False_

for i in xrange( 3, greatest_int, 2 ) : # 2**33 - 1

    if ( not crible_table[ i ] ) :
        continue
    else :
        crible_table[ i * 2 : greatest_int : i ] = np.False_

np.save( "crible_table_" + str( exponent ), crible_table )
