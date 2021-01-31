import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

values = {}

n = int(raw_input())
time_i = 0
for i in raw_input().split():
    v = int(i)
    values[ v ] = values.get( v, [] ) + [ time_i ]
    time_i += 1
    
# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

# print >> sys.stderr, "values", values

max_perte = 0
min_value = min( values.keys() )

sorted_values = sorted( values.keys(), reverse = True )

for v in sorted_values :
    if ( max_perte < min_value - v ):
        break

    # print >> sys.stderr, "v", v
    # print >> sys.stderr, "max_perte", max_perte
    
    for tmp_v in sorted( sorted_values[ sorted_values.index( v ) + 1 : ] ) :
        t1 = min( values[ v ] )
        t2 = max( values[ tmp_v ] )
        # print >> sys.stderr, "t1", t1
        # print >> sys.stderr, "t2", t2
        
        if ( t1 < t2 ) :
            if ( tmp_v - v < max_perte ):
                max_perte = tmp_v - v
                break
        
print max_perte
