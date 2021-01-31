import sys
import math

def get_landing_site( surface ) :
    res = set()

    # print >> sys.stderr, zip( surface[ : -1 ], surface[ 1 : ] )
    # exit()
    for p1, p2 in zip( surface[ : -1 ], surface[ 1 : ] ) :
        if ( p1[ 1 ] == p2[ 1 ] ) :
            res.add( p1 ) ; res.add( p2 )

    res = sorted( list( res ) )
    # print >> sys.stderr, "res", res
    
    # return res[ 0 ][ 0 ], res[ -1 ][ 0 ]
    return res[ 0 ], res[ -1 ]

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

surface = []

surface_n = int(raw_input()) # the number of points used to draw the surface of Mars.
for i in xrange(surface_n):
     # land_x: X coordinate of a surface point. (0 to 6999)
     # land_y: Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
    land_x, land_y = [int(j) for j in raw_input().split()]
    surface.append( ( land_x, land_y ) ) 

landing_site_coord = get_landing_site( surface )
landing_site = ( landing_site_coord[ 0 ][ 0 ], landing_site_coord[ 1 ][ 0 ] )
landing_site_altitude = landing_site_coord[ 0 ][ 1 ]
# print >> sys.stderr, "landing_site", landing_site

h_speed_max = -1.0
h_speed_threshold = 50
h_speed_landing = 1
rot_action_std = 30

# game loop
while 1:
    # h_speed: the horizontal speed (in m/s), can be negative.
    # v_speed: the vertical speed (in m/s), can be negative.
    # fuel: the quantity of remaining fuel in liters.
    # rotate: the rotation angle in degrees (-90 to 90).
    # power: the thrust power (0 to 4).
    x, y, h_speed, v_speed, fuel, rotate, power = [int(i) for i in raw_input().split()]

    h_speed_max = max( h_speed_max, abs( h_speed ) )
    
    # Write an action using print
    # To debug: print >> sys.stderr, "Debug messages..."
    # print >> sys.stderr, "v_speed", v_speed
    # print >> sys.stderr, "h_speed", h_speed
    
    # rotate power. rotate is the desired rotation angle. power is the desired thrust power.
    rotate_action = 0
    power_action  = 4

    #print rotate_action, power_action
    # print "0 4"

    if ( ( landing_site[ 0 ] <= x ) and ( x <= landing_site[ 1 ] ) ) :
        print >> sys.stderr, "test 1"

        v_thres_1 = -10
        v_thres_2 = -20
            
        if ( v_thres_1 < v_speed ) :
            power_action = 3
        elif ( ( v_thres_2 < v_speed ) and ( v_speed < v_thres_1 ) ) :
            print >> sys.stderr, "high speed1"
            theta = float( v_speed + ( - v_thres_1 ) ) / ( v_thres_2 - v_thres_1 )
            power_action = 4 * (     theta ) + \
                           3 * ( 1 - theta )
            power_action = int( round( power_action ) )
        else :
            print >> sys.stderr, "high speed2"
            power_action = 4

        if ( h_speed < -h_speed_landing ) :
            rotate_action = max( -90, -90 * abs( h_speed ) / h_speed_max - 1 )
            
        elif ( h_speed_landing < h_speed ) :
            rotate_action = min( 90, 90 * abs( h_speed ) / h_speed_max + 1 )

        else :
            rotate_action = 0

            if ( v_speed < -35 ) :
                power_action = 4
            else :
                power_action = 3

                
    elif ( x < landing_site[ 0 ] ) :
        if ( h_speed_threshold < h_speed ) :
            print >> sys.stderr, "test 2.2"
            rotate_action = rot_action_std
            power_action  = 4
        
        else :
            print >> sys.stderr, "test 2.1"
            rotate_action = -rot_action_std
            power_action  = 4
        
        if ( v_speed < -40 ) :
            power_action = 4

        if ( 0 < v_speed ) :
            power_action = 0            
            
                
    elif ( landing_site[ 1 ] < x ) :
        if ( h_speed < -h_speed_threshold ) :
            print >> sys.stderr, "test 3.2"
            rotate_action = -rot_action_std
            power_action  = 4

        else :
            print >> sys.stderr, "test 3.1"
            rotate_action = rot_action_std
            power_action  = 4

        if ( v_speed < -40 ) :
            power_action = 4

        if ( 0 < v_speed ) :
            power_action = 0            

    if ( abs( y - landing_site_altitude ) < 50 ) :
        rotate_action = 0

    print rotate_action, power_action
    # print "-20 3"
