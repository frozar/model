import matplotlib.pyplot as plt
from operator import itemgetter

# Read file and get x, y
import urllib2
from bs4 import BeautifulSoup

f = open( "tmp/web_site.html" )
to_parse = f.read()

soup = BeautifulSoup( to_parse )

l_div = soup.find_all("div")
l_div = filter( lambda tag : tag.has_attr('class'), l_div )
l_div = filter( lambda tag : "outputLine" in tag["class"], l_div )
l_div = map( lambda tag : tag.contents, l_div )
l_div = sum( l_div, [] )

# print l_div[ : 10 ]
# print l_div[ 4 ][ 0 ][ : 2 ]

l_pt = filter( lambda s : s[ : 2 ] == "pt", l_div )
l_pt = map( lambda pt : ( int( pt.split()[ 1 ] ), int( pt.split()[ 2 ] ) ), l_pt )

# answer = "guess_3"

# f = open( 'to_plot_' + answer + '.txt' )

# l_f = f.readlines()
# l_f = map( lambda s : s.split(), l_f )

# x = map( itemgetter( 0 ), l_f )
# y = map( itemgetter( 1 ), l_f )

# x = map( int, x )
# y = map( int, y )

x = map( itemgetter( 0 ), l_pt )
y = map( itemgetter( 1 ), l_pt )

fig, ax = plt.subplots()

eps = 0.5

for i in xrange( len( x ) / 5 ) :
    ax.plot( x[ 5 * i : 5 * ( i + 1 ) ], y[ 5 * i : 5 * ( i + 1 ) ], 'ro' )
    ax.plot( x[ 5 * ( i + 1 ) - 1 : 5 * ( i + 1 ) ], y[ 5 * ( i + 1 ) - 1 : 5 * ( i + 1 ) ], 'go' )
    ax.set_xlim( ( min( x ) - eps, max( x ) + eps ) )
    ax.set_ylim( ( min( y ) - eps, max( y ) + eps ) )
    # plt.savefig( "/tmp/CG_" + answer + "/CG_%04d.png" % ( i ) )
    plt.savefig( "fig/CG_%04d.png" % ( i ) )
    ax.clear()
    
