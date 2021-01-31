# -*- coding: utf-8 -*-

import sys
import math

def dist_pt( lon1, lat1, lon2, lat2 ):
    x = ( lon2 - lon1 ) * math.cos( ( lat1 + lat2 ) / 2 )
    y = ( lat2 - lat1 )
    return math.sqrt( x*x + y*y ) * 6371

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

d_defibs = {}

lon = raw_input()
lat = raw_input()

lon = float( lon.replace(",", ".") )
lat = float( lat.replace(",", ".") )

n = int(raw_input())

# Numéro identifiant le défibrillateur ; Nom ; Adresse ; 
# Numéro de téléphone à joindre ; Longitude (en degrés) ; Latitude (en degrés)
for i in xrange(n):
    defib   = raw_input()
    l_defib = defib.split(";")
    loop_dist = dist_pt( lon, lat, 
                         float( l_defib[ -2 ].replace(",", ".") ), 
                         float( l_defib[ -1 ].replace(",", ".") ) )
    d_defibs[ int( l_defib[ 0 ] ) ] = [ loop_dist, l_defib[ 1 ] ]

l_res = d_defibs.values()
l_res.sort()

# print >> sys.stderr, l_res[ : 5 ]

print l_res[0][1]
