import sys
# import math
from math import sqrt

def pp(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

# def cost( water, dist, dist_max ):
def cost( water, dist ):
    dist = max( dist, 1.0 )
    return (1.0 / float( dist / 3000 )) + (water / 10.0)

def dist( e0, e1 ):
    e0_x = e0[5]
    e0_y = e0[6]
    e1_x = e1[5]
    e1_y = e1[6]
    return sqrt( float( ( e0_x - e1_x ) ** 2 + ( e0_y - e1_y ) ** 2 ) )

def dist_to_center( e ):
    e_x = e[5]
    e_y = e[6]
    return sqrt( float( ( e_x ) ** 2 + ( e_y ) ** 2 ) )

def speed( e ):
    e_vx = e[7]
    e_vy = e[8]
    return sqrt(e_vx ** 2 + e_vy ** 2)

def compute_vadd(x, y, p_x, p_y, power, mass):
    distance = sqrt((x - p_x) ** 2 + (y - p_y) ** 2)
    distance = max( distance, 1.0 )

    coef   = ((float( power )) / mass) / distance
    vadd_x = (p_x - x) * coef;
    vadd_y = (p_y - y) * coef;
    return (vadd_x, vadd_y)

def compute_thrust_coor(unit, target, mass):
    unit_x, unit_y, unit_vx, unit_vy  = unit[5], unit[6], unit[7], unit[8]

    target_x, target_y, target_vx, target_vy = target[5], target[6], target[7], target[8]

    next_target_pos = ( target_x + target_vx, target_y + target_vy )
    v_dir = ( next_target_pos[0] - unit_x - unit_vx, next_target_pos[1] - unit_y - unit_vy )
    v_dir = ( v_dir[0] * mass, v_dir[1] * mass )
    v_dir_norm = sqrt( v_dir[0] ** 2 + v_dir[1]  ** 2 )
    v_dir_norm = max( v_dir_norm, 1.0 )
    v_dir = ( v_dir[0] / v_dir_norm, v_dir[1] / v_dir_norm )

    d_x = v_dir[0] * 1000
    d_y = v_dir[1] * 1000

    thrust_x = d_x + unit_x
    thrust_y = d_y + unit_y

    power = round( v_dir_norm )

    return thrust_x, thrust_y, power


MASS_REAPER    = 0.5
MASS_DESTROYER = 1.5
MASS_DOOF      = 1.

TYPE_REAPER    = 0
TYPE_DESTROYER = 1
TYPE_DOOF      = 2
TYPE_TANKER    = 3
TYPE_WRECK     = 4

# game loop
while True:
    my_score = int(input())
    enemy_score_1 = int(input())
    enemy_score_2 = int(input())
    my_rage = int(input())
    enemy_rage_1 = int(input())
    enemy_rage_2 = int(input())
    unit_count = int(input())
    l_entities = []
    for i in range(unit_count):
        unit_id, unit_type, player, mass, radius, x, y, vx, vy, extra, extra_2 = input().split()
        unit_id = int(unit_id)
        unit_type = int(unit_type)
        player = int(player)
        mass = float(mass)
        radius = int(radius)
        x = int(x)
        y = int(y)
        vx = int(vx)
        vy = int(vy)
        extra = int(extra)
        extra_2 = int(extra_2)
        l_entities.append( (unit_id, unit_type, player, mass, radius, x, y, vx, vy, extra, extra_2 ) )

    l_wreck      = []
    my_reaper    = None
    my_destroyer = None
    my_doof      = None
    one_oppo_reaper = None
    l_tanker     = []
    for entity in l_entities:
        if ( entity [2] == 0 ):
            if ( entity [1] == TYPE_REAPER ):
                my_reaper = entity
            if ( entity [1] == TYPE_DESTROYER ):
                my_destroyer = entity
            if ( entity [1] == TYPE_DOOF ):
                my_doof = entity

        if ( (one_oppo_reaper is None ) and ( entity [2] != 0 ) ):
            if ( entity [1] == TYPE_REAPER ):
                one_oppo_reaper = entity

        if ( entity [1] == TYPE_TANKER ):
            l_tanker.append( entity )

        if ( entity [1] == TYPE_WRECK ):
            l_wreck.append( entity )
            
    l_tanker = list( filter( lambda t : dist_to_center( t ) <= 6000, l_tanker ) )

    my_reaper_x = my_reaper[5]
    my_reaper_y = my_reaper[6]

    target_wreck  = None
    target_tanker = None

    cost_max = -1
    for wreck in l_wreck:
        wreck_x = wreck[5]
        wreck_y = wreck[6]
        d = dist( my_reaper, wreck )
        water   = wreck[9]

        tmp_cost = cost( water, d )
        if (cost_max < tmp_cost):
            target_wreck = wreck
            cost_max     = tmp_cost
                
    water_max = 0
    for tanker in l_tanker:
        tanker_x = tanker[5]
        tanker_y = tanker[6]
        d        = dist( my_reaper, tanker )
        water    = tanker[9]
        if (water_max < water):
            target_tanker = tanker
            water_max     = water

    pp( "target_tanker: ", target_tanker )
    pp( "target_wreck: ", target_wreck )

    target = None
    if ( target_wreck is not None ):
        target = target_wreck
    else :
        target = target_tanker

    # GAME OUTPUT
    # Reaper
    if ( target ):
        thrust_x, thrust_y, my_throttle = compute_thrust_coor (my_reaper, target, MASS_REAPER)
        print("%d %d %d" % ( thrust_x, thrust_y, my_throttle ))
    else:
        print( "WAIT" )

    # Destroyer
    if ( target_tanker ):
        thrust_x, thrust_y, power = compute_thrust_coor (my_destroyer, target_tanker, MASS_DESTROYER)
        print("%d %d %d" % ( thrust_x, thrust_y, power ))
    else:
        print( "WAIT" )

    # Doof
    thrust_x, thrust_y, power = compute_thrust_coor (my_doof, one_oppo_reaper, MASS_DOOF)
    print("%d %d %d" % ( thrust_x, thrust_y, power ))

