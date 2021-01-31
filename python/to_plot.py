"""
Demo of a simple plot with a custom dashed line.

A Line object's ``set_dashes`` method allows you to specify dashes with
a series of on/off lengths (in points).
"""
import numpy as np
import matplotlib.pyplot as plt
import math

y = [ 42, 78, 108, 134, 156, 175, 191, 204, 215, 225, 233, 240, 246, 251, 255, 259, 262, 265, 267, 269, 271, 272, 273, 274, 275, 276, 277 ]
# x = np.linspace(0, len( y ) )
# x = range( 1, len( y ) + 1 )
x = range( len( y ) )

line, = plt.plot(x, y, '--', linewidth=2)
# line2, = plt.plot( map( lambda img : math.log( float( img ) ), x ), [ math.log( 1 ) ] + map( lambda img : math.log( img ), y[ 1 : ] ), '*', linewidth=2)
# line2, = plt.plot( x, map( lambda img : 277 * math.exp( - img / 2.0 ), x ), '*', linewidth=2)
y_exp = map( lambda img : 42 + ( 277 - 42 ) * ( 1 - math.exp( - img / 2.0 ) ), x )
line2, = plt.plot( x, y_exp, '*', linewidth=2)

print y
print y_exp

l_diff = []

for i in range( len( y ) ):
    l_diff.append( y_exp[ i ] - y[ i ] )

print l_diff

# line2, = plt.plot( x, map( lambda img : 277 * ( 1 - math.exp( - img / 2.0 ) ), x ), '*', linewidth=2)

plt.show()

