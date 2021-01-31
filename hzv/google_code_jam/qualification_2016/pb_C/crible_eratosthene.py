import sys

exponent = int( sys.argv[ 1 ] )

crible_table = set()

greatest_int = 2 ** exponent

# Deal with the special case 2
tmp_n  = 2    
while ( tmp_n < greatest_int ) :        
    crible_table.add( tmp_n )
    tmp_n += 2


for i in xrange( 3, greatest_int, 2 ) : # 2**33 - 1

    if ( i in crible_table ) :
        continue
    
    tmp_n  = i
    while ( tmp_n < greatest_int ) :
        crible_table.add( tmp_n )
        tmp_n += i

# print crible_table
