# By Armin Moser
# By frozar

from mpl_toolkits.mplot3d import Axes3D
import matplotlib
from matplotlib import cm

import numpy as np

from matplotlib import pyplot as plt

import numpy as np

from scipy.special import jv, yv
from scipy.special import jvp, yvp
from scipy.special import jn_zeros

def read_roots():
    fp    = open( "zeros_bessel.txt", 'r' )
    lines = fp.readlines()
    lines = lines[ 1 : ]

    for l in lines :
        # key   = ( m, k ) 
        # value = root_value
        k = tuple( map( lambda x : int(x), l.split()[ : 2 ] ) )
        v = float( l.split()[ 2 ] )

        roots[ k ] = v


def f( m, k, r, theta ):
    root = roots[ ( m, k ) ]

    return ( jv( m, complex( root , 0 ) ) * yv( m, complex( root * ( r / r_max ), 0 ) ) - \
                 yv( m, complex( root , 0 ) ) * jv( m, complex( root * ( r / r_max ), 0 ) ) ) * \
                 np.exp( complex(0, 1) * m * theta )



step = 0.04
maxval = 1.0
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

r_min = 0.1
r_max = 0.9

roots = {}

# create supporting points in polar coordinates
nb_pt = 21
r = np.linspace( r_min, r_max, nb_pt )
p = np.linspace( 0, 2*np.pi, nb_pt )

R,P = np.meshgrid(r,p)

# transform them to cartesian system
X,Y = R*np.cos(P),R*np.sin(P)


Z = np.zeros( ( len( X ), len( Y ) ) )

m = 1
k = 1

read_roots()

for i_x in range( len (X) ):
    r = R[0][ i_x ]
    for j_y in range( len (Y) ):
        theta = P[ j_y ][0]
        Z[ j_y ][ i_x ] = f( m, k, r, theta ).real

ymin, ymax = min( p ), max( p )

zmin = Z.min()

ax.plot_surface( X, Y, Z, rstride = 1, cstride = 1, linewidth = 0.4,
                 alpha=0.4, cmap=cm.RdYlBu_r )

# cset = ax.contourf(X, Y, Z, zdir='y', offset = ymax, cmap=cm.RdYlBu_r )
# cset = ax.contourf(X, Y, Z, zdir='y', offset = 1, cmap=cm.RdYlBu_r )
cset = ax.contourf(X, Y, Z, zdir='z', offset = zmin, cmap=cm.RdYlBu_r )

# cset = ax.contourf(X, Y, Z, zdir='z', offset = 0, cmap=cm.RdYlBu_r )

# ax.set_zlim3d(0, 1)

f_s = 20

ax.set_xlabel( r'$x$', fontsize = f_s )
ax.set_ylabel( r'$y$', fontsize = f_s )
ax.set_zlabel( r'$f_{m,k}( r, \theta )$', fontsize = f_s )

ax.view_init( elev = 34, azim = -56 )

plt.savefig( 'f_2_m1_k1.png' )
# plt.show()

# pngtopnm f_2_m1_k1.png > f_2_m1_k1_t.pnm && pnmcrop -left -right -top -bottom f_2_m1_k1_t.pnm > f_2_m1_k1.pnm && convert -density 300x300 f_2_m1_k1.pnm f_2_m1_k1.png && rm f_2_m1_k1.pnm f_2_m1_k1_t.pnm
