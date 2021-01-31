import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

correct_v_size = 4 * 400
correct_speed  = 200
max_speed      = 200
min_speed      = 20


def dist( pt1, pt2 ) :
    vect = ( pt1[ 0 ] - pt2[ 0 ], pt1[ 1 ] - pt2[ 1 ] )
    # print >> sys.stderr, "pt1", pt1
    # print >> sys.stderr, "vect", vect
    return math.ceil( math.sqrt( vect[ 0 ] * vect[ 0 ] + vect[ 1 ] * vect[ 1 ] ) )


def dist_angle( angle1, angle2 ) :
    angle_min = min( [ angle1, angle2 ] )
    angle_max = max( [ angle1, angle2 ] )

    res = angle_max - angle_min
    
    if ( 180 < res ) :
        res = angle_min + 360 - angle_max

    return res


def nearest_int( x ):
    return int( round( x ) )


def resize_vect( v, guess_size ) :
    guess_size = float( guess_size )
    cur_size = norme( v )
    k = guess_size / cur_size
    return ( v[ 0 ] * k, v[ 1 ] * k )


def norme( vector ) :
    return math.sqrt( vector[ 0 ] * vector[ 0 ] + vector[ 1 ] * vector[ 1 ] )


def rot_vect( vect, theta ) :
    r_th = math.radians( theta )
    on_x = vect[ 0 ] * math.cos( r_th ) - vect[ 1 ] * math.sin( r_th )
    on_y = vect[ 0 ] * math.sin( r_th ) + vect[ 1 ] * math.cos( r_th )

    return ( on_x, on_y )


def vec_in_r2( vect, a, b, c, d ) :
    det = d*a - c*b
    return ( ( vect[ 0 ] * d + vect[ 1 ] * ( -c ) ) / det,
             ( vect[ 0 ] * ( -b ) + vect[ 1 ] * a ) / det )

            
def col_factor( vect_v, pt1, pt2 ) :
    vect_direction = ( pt2[ 0 ] - pt1[ 0 ], pt2[ 1 ] - pt1[ 1 ] )

    # print >> sys.stderr, "vect_v", vect_v
    # print >> sys.stderr, "vect_direction", vect_direction

    # angle in ] -Pi, Pi ]
    angle_1 = math.atan2( vect_v[ 1 ], vect_v[ 0 ] )
    angle_2 = math.atan2( vect_direction[ 1 ], vect_direction[ 0 ] )

    # angle in ] 0, 2 * Pi ]
    angle_1 += math.pi
    angle_2 += math.pi

    # print >> sys.stderr, "angle_1", angle_1
    # print >> sys.stderr, "angle_2", angle_2
    
    # delta_a in [ 0, 1 ]
    delta_a = None
    if ( angle_1 < angle_2 ) :
        delta_a = min( abs( angle_2 - angle_1 ), abs( ( 2 * math.pi + angle_1 ) - angle_2 ) )
    else :
        delta_a = min( abs( angle_1 - angle_2 ), abs( ( 2 * math.pi + angle_2 ) - angle_1 ) )

    # print >> sys.stderr, "1 delta_a", delta_a
    delta_a = delta_a / math.pi
    # print >> sys.stderr, "2 delta_a", delta_a
    
    return delta_a


def f1( vect_v, pt1, pt2, verbose = False ) :
    norm_delta = col_factor( vect_v, pt1, pt2 )
    save_col_factor = norm_delta
    norm_delta = 1 - norm_delta
    norm_delta = math.pow( norm_delta, 5 )

    # if( verbose ) :
    #     print >> sys.stderr, "col_factor( vect_v, pt1, pt2 )", save_col_factor
            
    power_min = 50
    power_max = 200
    return int ( power_min + norm_delta * ( power_max - power_min ) )
    

def velocity_enough( x, y, vx, vy, direction_angle, vitesse_angle, nextCheckPointId ) :
    res = False

    v_factor = 3
    
    virtual_pt = ( x + vx * v_factor, y + vy * v_factor )
    # if( dist( virtual_pt, checkpoints[ nextCheckPointId ] ) < 600 ) :
    if( dist( ( x, y ), checkpoints[ nextCheckPointId ] ) - 600 <= dist( virtual_pt, ( x, y ) ) ) :
        if ( dist_angle( direction_angle, vitesse_angle ) <= 3 ) :
            res = True
        

    return res


def get_target_pt_n_power_2( x, y, prev_x, prev_y, vx, vy, angle, nextCheckPointId ) :

    prevCheckPointId = ( nextCheckPointId - 1 ) % len( checkpoints )
    prev_ck_pt_x = checkpoints[ prevCheckPointId ][ 0 ]
    prev_ck_pt_y = checkpoints[ prevCheckPointId ][ 1 ]
    
    # If the check point is not accessible now because of the angle
    trg_x = checkpoints[ nextCheckPointId ][ 0 ]
    trg_y = checkpoints[ nextCheckPointId ][ 1 ]

    direction_vector = ( trg_x - x, trg_y - y )
    direction_angle  = math.atan2( direction_vector[ 1 ], direction_vector[ 0 ] )
    direction_angle  = math.degrees( direction_angle )
    if ( direction_angle < 0 ) :
        direction_angle += 360
    direction_angle = int( round( direction_angle ) )

    vitesse_vector = ( vx, vy )
    vitesse_angle  = math.atan2( vitesse_vector[ 1 ], vitesse_vector[ 0 ] )
    vitesse_angle  = math.degrees( vitesse_angle )
    if ( vitesse_angle < 0 ) :
        vitesse_angle += 360
    vitesse_angle = int( round( vitesse_angle ) )

    if ( angle == -1 ) :
        return trg_x, trg_y, max_speed
    
    if( velocity_enough( x, y, vx, vy, direction_angle, vitesse_angle, nextCheckPointId ) ) :
        print >> sys.stderr, "pass 0"
        nextnextCheckPointId = ( nextCheckPointId + 1 ) % len( checkpoints )
        # return checkpoints[ nextnextCheckPointId ][ 0 ], \
        #     checkpoints[ nextnextCheckPointId ][ 1 ], \
        #     min_speed
        return checkpoints[ nextnextCheckPointId ][ 0 ], \
            checkpoints[ nextnextCheckPointId ][ 1 ], \
            0

    if ( ( 3 * 18 <= dist_angle( vitesse_angle, direction_angle ) ) and
         ( 3 * 18 <= dist_angle( angle, direction_angle ) ) ):
        print >> sys.stderr, "pass 1.0"
        return trg_x, trg_y, \
            max( min_speed, 
            min( nearest_int( ( 2 * dist_angle( vitesse_angle, direction_angle ) / 180.0 -
                                dist( ( x, y ), ( trg_x, trg_y ) ) /
                                dist( ( prev_ck_pt_x, prev_ck_pt_y ), ( trg_x, trg_y ) ) )
                              * max_speed ),
                 max_speed )
            )

    elif ( ( 3 * 18 <= dist_angle( vitesse_angle, direction_angle ) ) and
           ( dist_angle( angle, direction_angle ) < 3 * 18 ) ):
        print >> sys.stderr, "pass 1.1"
        # return trg_x, trg_y, \
        #     max( min_speed, 
        #     min( nearest_int( ( dist( ( x, y ), ( trg_x, trg_y ) ) /
        #                         dist( ( prev_ck_pt_x, prev_ck_pt_y ), ( trg_x, trg_y ) ) )
        #                       * 50 ),
        #          max_speed )
        #     )

        return trg_x, trg_y, 80
    
    
    if( 2 <= dist_angle( vitesse_angle, direction_angle ) ) :

        # if ( dist_angle( angle, direction_angle ) < 2 * 18 ) :
        #     print >> sys.stderr, "pass 2.0"
        
        print >> sys.stderr, "pass 2"
        # If we don't move
        if ( norme( ( vx, vy ) ) < 5 ) :
            return trg_x, trg_y, max_speed

        if ( dist_angle( angle, direction_angle ) <= 1 ) :
            return trg_x, trg_y, correct_speed
        
        dist_a = dist_angle( vitesse_angle, direction_angle )
        # Kind of normalisation of angles
        if ( 180 < abs( vitesse_angle - direction_angle ) ) :
            if ( vitesse_angle < direction_angle ):
                vitesse_angle += 360
            else :
                direction_angle += 360

        # Choice a target point and an a power depending on the distance between angles
        v_next_pt = ( None, None )

        fine_factor_angle = 1.8

        angle_rotation = None
        if ( vitesse_angle < direction_angle ) :
            angle_rotation = min( fine_factor_angle * dist_a, angle + 18 - vitesse_angle )
            # angle_rotation = max( fine_factor_angle * dist_a, angle + 18 - vitesse_angle )
            v_next_pt = rot_vect( ( vx, vy ), angle_rotation )
        else :
            angle_rotation = max( angle - 18 - vitesse_angle, - fine_factor_angle * dist_a )
            # angle_rotation = min( angle - 18 - vitesse_angle, - fine_factor_angle * dist_a )
            v_next_pt = rot_vect( ( vx, vy ), angle_rotation )

        # print >> sys.stderr, "angle_rotation", angle_rotation
        v_next_pt = resize_vect( v_next_pt, correct_v_size )
        
        next_pt_x, next_pt_y = ( x + v_next_pt[ 0 ], y + v_next_pt[ 1 ] )
        # print >> sys.stderr, "next_pt_x, next_pt_y", next_pt_x, next_pt_y

        # if ( 2.4 * 18 < dist_angle( angle, direction_angle ) ) :
        #     return nearest_int( next_pt_x ), nearest_int( next_pt_y ), min_speed
        # else :
        #     return nearest_int( next_pt_x ), nearest_int( next_pt_y ), correct_speed

        return nearest_int( next_pt_x ), nearest_int( next_pt_y ), correct_speed
    
    else :
        print >> sys.stderr, "pass 3"
        v_next_pt = resize_vect( ( vx, vy ), correct_v_size )

        next_pt_x, next_pt_y = ( x + v_next_pt[ 0 ], y + v_next_pt[ 1 ] )
        
        return nearest_int( next_pt_x ), nearest_int( next_pt_y ), correct_speed


        
def get_target_pt_n_power( x, y, prev_x, prev_y, vx, vy, angle, nextCheckPointId ) :

    # If the check point is not accessible now because of the angle
    trg_x = checkpoints[ nextCheckPointId ][ 0 ]
    trg_y = checkpoints[ nextCheckPointId ][ 1 ]

    direction_vector = ( trg_x - x, trg_y - y )
    direction_angle  = math.atan2( direction_vector[ 1 ], direction_vector[ 0 ] )
    direction_angle  = math.degrees( direction_angle )
    if ( direction_angle < 0 ) :
        direction_angle += 360
    direction_angle = int( round( direction_angle ) )

    if( velocity_enough( x, y, vx, vy, nextCheckPointId ) ) :
        nextnextCheckPointId = ( nextCheckPointId + 1 ) % len( checkpoints )
        return checkpoints[ nextnextCheckPointId ][ 0 ], \
            checkpoints[ nextnextCheckPointId ][ 1 ], \
            0
    
    elif ( ( 18 * 4 < dist_angle( direction_angle - angle ) ) and ( angle != -1 ) ) :
        return trg_x, trg_y, "SHIELD"

    elif ( ( 18 * 2 < dist_angle( direction_angle - angle )  ) and
           ( abs( direction_angle - angle ) <= 18 * 4 ) and
           ( angle != -1 )  ) :
        return trg_x, trg_y, 0

    else :
        v_normal = ( direction_vector[ 0 ] / norme( direction_vector ),
                     direction_vector[ 1 ] / norme( direction_vector ) )
        v_tang   = rot_vect( v_normal, -90 )

        # Is it possible to reach the line between the current and the next checkpoint
        # thanks to a given target point and a power
        trg_v_1 = rot_vect( direction_vector, -18 )
        trg_pt_1 = ( x + trg_v_1[ 0 ], y + trg_v_1[ 1 ] )
        # trg_v_1 = rot_vect( v_normal, -18 )
        # print >> sys.stderr, "trg_v_1", trg_v_1
        # print >> sys.stderr, "trg_pt_1", trg_pt_1
        
        # next_pt_1_x, next_pt_1_y, angle_1 = \
        #     predict_next_pt_n_angle( x, y, prev_x, prev_y,
        #                              vx, vy, angle, trg_v_1[ 0 ], trg_v_1[ 1 ],
        #                              200 )
        next_pt_1_x, next_pt_1_y, angle_1 = \
            predict_next_pt_n_angle( x, y, prev_x, prev_y,
                                     vx, vy, angle, trg_pt_1[ 0 ], trg_pt_1[ 1 ],
                                     200 )

        # print >> sys.stderr, "next_pt_1_x, next_pt_1_y", next_pt_1_x, next_pt_1_y
        
        trg_v_2 = rot_vect( direction_vector, 18 )
        trg_pt_2 = ( x + trg_v_2[ 0 ], y + trg_v_2[ 1 ] )
        # trg_v_2 = rot_vect( v_normal, 18 )
        # print >> sys.stderr, "trg_v_2", trg_v_2
        # print >> sys.stderr, "trg_pt_2", trg_pt_2
        
        # next_pt_2_x, next_pt_2_y, angle_2 = \
        #     predict_next_pt_n_angle( x, y, prev_x, prev_y,
        #                              vx, vy, angle, trg_v_2[ 0 ], trg_v_2[ 1 ],
        #                              200 )
        next_pt_2_x, next_pt_2_y, angle_2 = \
            predict_next_pt_n_angle( x, y, prev_x, prev_y,
                                     vx, vy, angle, trg_pt_2[ 0 ], trg_pt_2[ 1 ],
                                     200 )

        # print >> sys.stderr, "next_pt_2_x, next_pt_2_y", next_pt_2_x, next_pt_2_y
        
        a = v_normal[ 0 ]
        b = v_normal[ 1 ]
        c = v_tang[ 0 ]
        d = v_tang[ 1 ]

        v_o2_next_pt_1 = ( next_pt_1_x - x, next_pt_1_y - y )
        v_o2_next_pt_2 = ( next_pt_2_x - x, next_pt_2_y - y )

        v_o2_next_pt_1_in_r2 = vec_in_r2( v_o2_next_pt_1, a, b, c, d )
        v_o2_next_pt_2_in_r2 = vec_in_r2( v_o2_next_pt_2, a, b, c, d )

        # print >> sys.stderr, "v_o2_next_pt_1_in_r2", v_o2_next_pt_1_in_r2
        # print >> sys.stderr, "v_o2_next_pt_2_in_r2", v_o2_next_pt_2_in_r2
        
        dist_min = 16000 * 16000
        elected_x, elected_y = None, None

        for tmp_angle in range( -18, 18 + 1 ) :
            # tmp_trg_v = rot_vect( v_normal, tmp_angle )
            # tmp_trg_v = ( x + 100 * tmp_trg_v[ 0 ], y + 100 * tmp_trg_v[ 0 ] )

            tmp_trg_v = rot_vect( direction_vector, tmp_angle )
            tmp_trg_v = ( x + tmp_trg_v[ 0 ], y + tmp_trg_v[ 1 ] )
            # print >> sys.stderr, "tmp_trg_v", tmp_trg_v
            
            tmp_next_pt_x, tmp_next_pt_y, predicted_angle = \
                predict_next_pt_n_angle( x, y, prev_x, prev_y,
                                         vx, vy, angle, tmp_trg_v[ 0 ], tmp_trg_v[ 1 ],
                                         200 )
            # print >> sys.stderr, "tmp_next_pt_x, tmp_next_pt_y", tmp_next_pt_x, tmp_next_pt_y
            
            tmp_vect = ( trg_x - tmp_next_pt_x, trg_y - tmp_next_pt_y )
            tmp_dist_min = norme( tmp_vect )
            # print >> sys.stderr, "tmp_dist_min", tmp_dist_min
            
            if ( tmp_dist_min < dist_min ) :
                dist_min = tmp_dist_min
                elected_x, elected_y = tmp_trg_v
                # print >> sys.stderr, "elected_x, elected_y", elected_x, elected_y
                            
        return int( round( elected_x ) ), int( round( elected_y ) ), 170

        # # If an intersection is found
        # if ( v_o2_next_pt_1_in_r2[ 1 ] * v_o2_next_pt_2_in_r2[ 1 ] < 0 ) :
        #     # Find the intersection


        #     # Find the target point and the correct power to arrive to the intersection

        #     # Find the target point and the correct power to arrive to the nearest point
        #     # to the direction
        #     dist_min = 16000 * 16000
        #     elected_x, elected_y = None, None
        #     for tmp_angle in range( -18, 18 + 1 ) :
        #         tmp_trg_v = rot_vect( v_normal, tmp_angle )
        #         tmp_next_pt_x, tmp_next_pt_y, predicted_angle = \
        #             predict_next_pt_n_angle( x, y, prev_x, prev_y,
        #                                      vx, vy, angle, tmp_trg_v[ 0 ], tmp_trg_v[ 1 ],
        #                                      200 )
        #         tmp_vect = ( trg_x - tmp_next_pt_x, trg_y - tmp_next_pt_y )
        #         tmp_dist_min = norme( tmp_vect )
        #         if ( tmp_dist_min < dist_min ) :
        #             dist_min = tmp_dist_min
        #             elected_x, elected_y = tmp_trg_v


        #     return elected_x, elected_y, 200

        # # If no intersection, rotate again
        # else :
        #     return trg_x, trg_y, 10

    
def predict_next_pt_n_angle( x, y, prev_x, prev_y, vx, vy, angle, trg_x, trg_y, power ) :
    # Compute the next angle
    next_angle = -1
    if ( ( trg_x != x ) and ( trg_y != y ) ) :
        direction_vector = ( trg_x - x, trg_y - y )
        direction_angle  = math.atan2( direction_vector[ 1 ], direction_vector[ 0 ] )
        direction_angle  = math.degrees( direction_angle )
        if ( direction_angle < 0 ) :
            direction_angle += 360
        direction_angle = int( round( direction_angle ) )
        # print >> sys.stderr, "direction_angle", direction_angle
        
        if ( angle == -1 ):
            next_angle = direction_angle
        elif ( abs ( angle - direction_angle ) <= 18 ) :
            next_angle = direction_angle
        else :
            if ( angle < direction_angle ) :
                next_angle = angle + 18
            else :
                next_angle = angle - 18
            
    # Compute the next point position
    virtual_pt = ( x + vx, y + vy )
    v_dep = ( 0, 0 )
    next_pt = ( None, None )

    if ( angle == -1 ) :
        v_dep = ( math.cos( math.radians( next_angle ) ) * power, \
                  math.sin( math.radians( next_angle ) ) * power )
    else :
        v_dep = ( math.cos( math.radians( angle ) ) * power, \
                  math.sin( math.radians( angle ) ) * power )

    # if ( prev_x is not None ) :
    #     # v_tang = ( x - prev_x, y - prev_y )
    #     # v_u_tang = ( float( v_tang[ 0 ] ) / norme( v_tang ), float( v_tang[ 1 ] ) / norme( v_tang ) )
    #     # v_dep = ( int( round( v_u_tang[ 0 ] * power ) ), int ( round (v_u_tang[ 1 ] * power ) ) )

    #     v_dep = ( math.cos( math.radians( angle ) ) * power, \
    #               math.sin( math.radians( angle ) ) * power )
        
    if ( ( norme( ( vx, vy ) ) != 0. )  and ( angle != next_angle ) ):
        alpha = next_angle - angle
        # alpha = next_angle - angle + math.copysign( 1, next_angle - angle )
        rayon = float( power ) / abs( math.radians( alpha ) )
        # print >> sys.stderr, "rayon", rayon

        # v_tang = ( x - prev_x, y - prev_y )
        # v_tang = ( x + vx - prev_x, y + vy - prev_y )

        direction_vector = ( trg_x - x, trg_y - y )
        v_tang = direction_vector

        # v_tang_previous = ( x - prev_x, y - prev_y )
        # direction_vector = ( trg_x - x, trg_y - y )
        # v_tang = ( direction_vector[ 0 ] + v_tang_previous[ 0 ],
        #            direction_vector[ 1 ] + v_tang_previous[ 1 ] )

        v_u_tang = ( float( v_tang[ 0 ] ) / norme( v_tang ), float( v_tang[ 1 ] ) / norme( v_tang ) )
        # v_tang = ( vx / norme( ( vx, vy ) ), vy / norme( ( vx, vy ) ) )
        # print >> sys.stderr, "v_u_tang", v_u_tang
        
        v_u_norm = rot_vect( v_u_tang, - math.copysign( 90, alpha ) )
        # print >> sys.stderr, "v_u_norm", v_u_norm
        
        v_ao = ( - v_u_norm[ 0 ] * rayon, - v_u_norm[ 1 ] * rayon )
        # print >> sys.stderr, "v_ao", v_ao
        # v_ao = ( v_u_norm[ 0 ] * rayon, v_u_norm[ 1 ] * rayon )
        v_oa = ( -v_ao[ 0 ], -v_ao[ 1 ] )
        # print >> sys.stderr, "v_oa", v_oa
        
        v_ob = rot_vect( v_oa, alpha )
        # print >> sys.stderr, "v_ob", v_ob
        
        pt_a = ( x, y )
        pt_o = ( pt_a[ 0 ] - v_u_norm[ 0 ] * rayon, pt_a[ 1 ] - v_u_norm[ 1 ] * rayon )
        # print >> sys.stderr, "pt_o", pt_o
        
        pt_b = [ None, None ]
        pt_b[ 0 ] = pt_a[ 0 ] - v_u_norm[ 0 ] * rayon + v_ob[ 0 ]
        pt_b[ 1 ] = pt_a[ 1 ] - v_u_norm[ 1 ] * rayon + v_ob[ 1 ]
        # print >> sys.stderr, "pt_b", pt_b

        v_ab = ( - v_u_norm[ 0 ] * rayon + v_ob[ 0 ] ,
                 - v_u_norm[ 1 ] * rayon + v_ob[ 1 ] )
        # print >> sys.stderr, "v_ab", v_ab
        # v_dep = tuple( pt_b )
        v_dep = tuple( v_ab )
        

    next_pt = ( int( round( virtual_pt[ 0 ] + v_dep[ 0 ] ) ), \
                int( round( virtual_pt[ 1 ] + v_dep[ 1 ] ) ) )
        
    return next_pt[ 0 ], next_pt[ 1 ], next_angle
    

checkpoints = []

laps = int(raw_input())
checkpoint_count = int(raw_input())
for i in xrange(checkpoint_count):
    checkpoint_x, checkpoint_y = [int(j) for j in raw_input().split()]

    checkpoints.append( ( checkpoint_x, checkpoint_y ) )

target_checkpoint_1 = checkpoints[ 0 ]
target_checkpoint_2 = checkpoints[ 0 ]
power_1 = 100
power_2 = 100

i = 0

previous_pos_1 = ( None, None )
previous_pos_2 = ( None, None )

# game loop
while True :
    x, y, vx, vy, angle, nextCheckPointId = [int(j) for j in raw_input().split()]

    # print >> sys.stderr, "x", x
    # print >> sys.stderr, "y", y
    # print >> sys.stderr, "vx", vx
    # print >> sys.stderr, "vy", vy
    # print >> sys.stderr, "angle", angle
    # print >> sys.stderr, "nextCheckPointId", nextCheckPointId

    # print >> sys.stderr, "previous_pos", previous_pos
    
    # print >> sys.stderr, "dist( ( x, y ), target_checkpoint_1 )", dist( ( x, y ), target_checkpoint_1 )
    target_checkpoint_1 = checkpoints[ nextCheckPointId ]

    # power_1 = f1( ( vx, vy ), ( x, y ), target_checkpoint_1, verbose = True )
    # print target_checkpoint_1[ 0 ], target_checkpoint_1[ 1 ], power_1

    # next_pt_x, next_pt_y, power = get_target_pt_n_power( x, y, previous_pos[ 0 ], previous_pos[ 1 ], 
    #                                                      vx, vy, angle, nextCheckPointId )
    next_pt_x, next_pt_y, power = get_target_pt_n_power_2( x, y,
                                                           previous_pos_1[ 0 ], previous_pos_1[ 1 ], 
                                                           vx, vy, angle, nextCheckPointId )
    print next_pt_x, next_pt_y, power
    
    # print >> sys.stderr, "prediction", \
    #     predict_next_pt_n_angle( x, y, previous_pos[ 0 ], previous_pos[ 1 ], vx, vy, angle,
    #                              target_checkpoint_1[ 0 ], target_checkpoint_1[ 1 ], power_1 )
    
    previous_pos_1 = ( x, y )

    # if ( i < 4 ) :
    #     print x, y, 0
    #     i += 1
    # elif( ( 4 <= i ) and ( i < 10 ) ):
    #     print 15000, y, 50
    #     i += 1
    # elif( ( 10 <= i ) and ( i < 11 ) ):
    #     print x, y, 0
    #     i += 1        
    # else :
    #     print 2000, y, 50

    # if ( i < 4 ) :
    #     print x, y, 0
    # elif( ( 4 <= i ) and ( i < 10 ) ):
    #     print x, 8000, 50
    # elif( ( 10 <= i ) and ( i < 11 ) ):
    #     print x, y, 0
    # elif( ( 11 <= i ) and ( i < 20 ) ):
    #     print x, 2000, 0
    # else :
    #     print x, 2000, 50

    # if ( i < 4 ) :
    #     print x, y, 0
    # else :
    #     print 15000, y, 50
    #     print 4912 + 9000, 1738 - 9000, 50

    x, y, vx, vy, angle, nextCheckPointId = [int(j) for j in raw_input().split()]

    # print >> sys.stderr, "x", x
    # print >> sys.stderr, "y", y
    # print >> sys.stderr, "vx", -vx
    # print >> sys.stderr, "vy", vy
    # print >> sys.stderr, "angle", angle

    # print >> sys.stderr, "POD2 : angle", angle
    # print >> sys.stderr, "dist( ( x, y ), target_checkpoint_2 )", dist( ( x, y ), target_checkpoint_2 )
    target_checkpoint_2 = checkpoints[ nextCheckPointId ]
    # power_2 = f1( ( vx, vy ), ( x, y ), target_checkpoint_2 )
    # print target_checkpoint_2[ 0 ], target_checkpoint_2[ 1 ], power_2

    next_pt_x, next_pt_y, power = get_target_pt_n_power_2( x, y,
                                                           previous_pos_2[ 0 ], previous_pos_2[ 1 ], 
                                                           vx, vy, angle, nextCheckPointId )
    print next_pt_x, next_pt_y, power

    previous_pos_2 = ( x, y )

    # t1 = 10
    # t2 = 100
    # t3 = 180

    # b1 = -10000
    # b2 = 15000

    # v = 25
    
    # if ( i < t1 ) :
    #     print b1, y, 0
    #     i += 1
    # elif( ( t1 <= i ) and ( i < t2 ) ):
    #     print b1, y, v
    #     i += 1
    # elif( ( t2 <= i ) and ( i < t3 ) ):
    #     print b2, y, 0
    #     i += 1        
    # else :
    #     print b2, y, v

    i += 1        

    # Write an action using print
    # To debug: print >> sys.stderr, "Debug messages..."

    # print "8000 4500 100"
    x, y, vx, vy, angle, next_check_point_id = [int(j) for j in raw_input().split()]
    # print >> sys.stderr, "EN POD1 : angle", angle
    x, y, vx, vy, angle, next_check_point_id = [int(j) for j in raw_input().split()]
    # print >> sys.stderr, "EN POD1 : angle", angle

