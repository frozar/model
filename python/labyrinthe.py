# -*- coding: utf-8 -*-
import sys, math

import copy

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

# R: number of rows.
# C: number of columns.
# A: number of rounds between the time the alarm countdown is activated and the time the alarm goes off.
R, C, A = [int(i) for i in raw_input().split()]

# avoir une matrice qui représente le terrain : chaque case a un état parmi '#.TC?'
grid       = [ ['?'] * C ] * R

cur_pos    = ( 0, 0 )
init_pos   = None
target_pos = None

reach_target_pos = False

cur_path = []

def update_grid ( line, ind_i ):
    grid[ ind_i ] = list( line )


def get_init_pos( ):
    global init_pos
    if not init_pos :
        for i in range( R ):
            for j in range( C ):
                if grid[i][j] == 'T':
                    init_pos = ( i, j )
                    return


def get_vec_move( mv ):
    vec_move = None
    if ( mv == 'RIGHT' ):
        vec_move = ( 0, 1 )
    if ( mv == 'LEFT' ):
        vec_move = ( 0, -1 )
    if ( mv == 'UP' ):
        vec_move = ( -1, 0 )
    if ( mv == 'DOWN' ):
        vec_move = ( 1, 0 )
    return vec_move


def get_mv( vec_move ):
    mv = None
    if ( vec_move == ( 0, 1 ) ):
        mv = 'RIGHT'
    if( vec_move == ( 0, -1 ) ):
        mv = 'LEFT'
    if( vec_move == ( -1, 0 ) ):
        mv = 'UP'
    if( vec_move == ( 1, 0 ) ):
        mv = 'DOWN'
    return mv

    
def get_target_pos( ):
    global target_pos
    if not target_pos :
        for i in range( R ):
            for j in range( C ):
                if grid[i][j] == 'C':
                    target_pos = ( i, j )
                    return


def get_possible_move ( grid, pos, forbidden_pos = None ):
    global R, C

    all_move = ['RIGHT', 'LEFT', 'UP', 'DOWN']

    possible_move = []
    for mv in all_move:
        vec_move = get_vec_move( mv )
        forbidden_next = []
        if grid[ pos[0] ][ pos[1] ] != '?':
            forbidden_next = ['#']
        else:
            forbidden_next = ['#', '?']

        tmp_pos = ( pos[0] + vec_move[0], pos[1] + vec_move[1] )
        if ( ( tmp_pos[0] < 0 ) or ( R <= tmp_pos[0] ) or \
            ( tmp_pos[1] < 0 ) or ( C <= tmp_pos[1] ) ) :
            continue

        if ( not grid[ tmp_pos[0] ][ tmp_pos[1] ] in forbidden_next ) :
            if not forbidden_pos :
                possible_move.append( mv ) 
            elif ( not tmp_pos in forbidden_pos ):
                possible_move.append( mv ) 
    
    return possible_move


def get_distances( distances, forbidden_pos, pos ):
    # print >> sys.stderr, "0", pos

    vecs_mov = get_possible_move( grid, pos, forbidden_pos = forbidden_pos )
    if not vecs_mov :
        return
    else :
        vecs_mov = map( lambda x : get_vec_move( x ), vecs_mov )

    next_pos = []
    for vec_mov in vecs_mov:
        tmp_pos = ( pos[0] + vec_mov[0], pos[1] + vec_mov[1] )
        # print >> sys.stderr, "1", vec_mov, pos
        # print >> sys.stderr, "2", tmp_pos
        forbidden_pos.append( tmp_pos )
        next_pos.append(tmp_pos)

        distances[ tmp_pos ] = distances[ pos ] + 1

    # print >> sys.stderr, "dist", distances

    while next_pos :
        # print >> sys.stderr, "3", next_pos
        pos = next_pos.pop(0)
        vecs_mov = get_possible_move( grid, pos, forbidden_pos = forbidden_pos )
        if not vecs_mov :
            continue
        else :
            vecs_mov = map( lambda x : get_vec_move( x ), vecs_mov )

        for vec_mov in vecs_mov:
            tmp_pos = ( pos[0] + vec_mov[0], pos[1] + vec_mov[1] )
            forbidden_pos.append( tmp_pos )
            next_pos.append( tmp_pos )

            distances[ tmp_pos ] = distances[ pos ] + 1

        # print >> sys.stderr, "dist", distances


def compute_path_from_p1_to_p2_aux( distances, pos ) :
    vecs_mov = get_possible_move( grid, pos, forbidden_pos = None )
    vecs_mov = map( lambda x : get_vec_move( x ), vecs_mov )

    # print >> sys.stderr, "path_aux", vecs_mov
    # print >> sys.stderr, "path_aux", pos

    for vec_mov in vecs_mov:
        tmp_pos = ( pos[0] + vec_mov[0], pos[1] + vec_mov[1] )
        #print >> sys.stderr, tmp_pos, tmp_pos in distances.keys()
        #if ( tmp_pos in distances.keys() ):
        #    print >> sys.stderr, pos, tmp_pos
        #    print >> sys.stderr, distances[  pos ], distances[ tmp_pos ]
        if ( tmp_pos in distances.keys() and \
            distances[  pos ] - distances[ tmp_pos ] == 1 ):
            next_pos = tmp_pos
            break
    return next_pos


def compute_path_from_p1_to_p2( distances, p1, p2 ) :
    # Construction à rebour d'un chemin p2 => p1

    # print >> sys.stderr, "p1", p1
    # print >> sys.stderr, "p2", p2

    vecs_mov = get_possible_move( grid, p2 )
    vecs_mov = map( lambda x : get_vec_move( x ), vecs_mov )

    next_pos = None
    for vec_mov in vecs_mov:
        tmp_pos = ( p2[0] + vec_mov[0], p2[1] + vec_mov[1] )
        # print >> sys.stderr, "path: tmp_pos", tmp_pos
        # print >> sys.stderr, "path: distances", distances[tmp_pos], distances[p2]
        if ( tmp_pos in distances.keys() and \
            distances[ p2 ] - distances[ tmp_pos ] == 1 ):
            next_pos = tmp_pos
            break
    
    # print >> sys.stderr, "distances", distances
    # print >> sys.stderr, "vecs_mov", vecs_mov
    # print >> sys.stderr, "next_pos", next_pos
    path = [ p2 ]
    
    while ( next_pos != p1 ):
        path.insert( 0, next_pos )
        next_pos = compute_path_from_p1_to_p2_aux( distances, next_pos )

    # print >> sys.stderr, path
    return path


def get_vec_mv_p1_to_p2( p1, p2 ):
    delta_i = p2[0] - p1[0]
    delta_j = p2[1] - p1[1]

    return ( delta_i, delta_j )


def get_next_move_target_known( grid, pos ):
    # Est-ce qu'il est possible d'atteindre la salle commande avec la
    # connaissance actuelle du graphe ?
    #   -> si non : continuer à explorer le graphe
    #   -> si oui : aller à la salle commmande

    # Est-ce qu'il existe un chemin entre cur_pos et target_pos ?
    # -> parcours du graphe en largeur à partir de cur_pos

    global A, init_pos, target_pos

    # Calculer le dict de distance des cases accessible à partir de cur_pos
    distances = {}
    distances[ pos ] = 0
    forbidden_pos = [ pos ]
    
    get_distances( distances, forbidden_pos, pos )

    next_move = None
    if ( target_pos in distances.keys() ):
        distances_target = {}
        distances_target[ target_pos ] = 0
        forbidden_pos_target = [ target_pos ]

        get_distances( distances_target, forbidden_pos_target, target_pos )

        path = compute_path_from_p1_to_p2( distances_target, 
                                           target_pos, init_pos )

        if ( len( path ) <= A ) :
            next_move = get_next_move( grid, pos, target_pos )
        else:
            print >> sys.stderr, "Not so short", 
            print >> sys.stderr, "Alarm", A
            print >> sys.stderr, "longueur du chemin", len(path)

            next_move = get_next_move_unknown( grid, pos, [target_pos] )

    else:
        print >> sys.stderr, "Not reachable", target_pos in distances.keys()
        next_move = get_next_move_unknown( grid, cur_pos )

    return next_move


def get_next_move( grid, cur_pos, pos ):
    # Calculer le dict de distance des cases accessible à partir de cur_pos
    distances = {}
    distances[ cur_pos ] = 0
    forbidden_pos = [ cur_pos ]
    
    get_distances( distances, forbidden_pos, cur_pos )
    
    path = compute_path_from_p1_to_p2( distances, cur_pos, pos )
    # print >> sys.stderr, "path : return 2"
    next_pos = path[ 0 ]
    vec_mv = get_vec_mv_p1_to_p2( cur_pos, next_pos )
    # print >> sys.stderr, "vec_mv", vec_mv
    return get_mv( vec_mv )


def get_nearest_accessible_unknowns( grid, cur_pos, forbidden_pos = None ):
    # Calculer le dict de distance des cases accessible à partir de cur_pos
    distances = {}
    distances[ cur_pos ] = 0

    if forbidden_pos :
        forbidden_pos.append( cur_pos )
    else:
        forbidden_pos = [ cur_pos ]
    
    # print >> sys.stderr, "forbidden_pos", forbidden_pos
    get_distances( distances, forbidden_pos, cur_pos )
    
    # print >> sys.stderr, "current position", cur_pos
    # print >> sys.stderr, "target position", target_pos
    # print >> sys.stderr, "distances", distances

    unknowns = {}
    for pos in distances.keys():
        if ( grid[ pos[0] ][ pos[1] ] == '?' ):
            unknowns[ pos ] = distances[ pos ]

    dist_min = min ( unknowns.values() )
    res = []
    for k, v in unknowns.items() :
        # if ( v <= dist_min + 1 ):
        if ( v <= dist_min ):
            res.append( k )
    
    return res


def get_next_move_unknown( grid, pos, forbidden_pos = None ):
    # print >> sys.stderr, "1 : forbidden_pos", forbidden_pos

    forbidden_pos_2 = copy.deepcopy( forbidden_pos )

    unknowns = get_nearest_accessible_unknowns( grid, pos, forbidden_pos_2 )

    distances = {}
    distances[ pos ] = 0
    if forbidden_pos :
        forbidden_pos.append( pos )
    else:
        forbidden_pos = [ pos ]

    # forbidden_pos = [ pos ]
    # print >> sys.stderr, "2 : forbidden_pos", forbidden_pos
    # print >> sys.stderr, "unknowns", unknowns

    get_distances( distances, forbidden_pos, pos )
    
    path = compute_path_from_p1_to_p2( distances, pos, unknowns[0] )
    next_pos = path[ 0 ]
    vec_mv = get_vec_mv_p1_to_p2( pos, next_pos )

    next_move = get_mv( vec_mv )

    return next_move


# game loop
while 1:
    # global R, C, A
    # global grid, cur_pos, init_pos, target_pos, reach_target_pos, cur_path
    # KR: row where Kirk is located.
    # KC: column where Kirk is located.
    KR, KC = [int(i) for i in raw_input().split()]

    # Mis à jour de la position courante, et du chemin déjà fait
    cur_pos = ( KR, KC )
    print >> sys.stderr, "position courante: ", cur_pos
    cur_path.append( cur_pos )

    for i in xrange(R):
        ROW = raw_input() # C of the characters in '#.TC?' (i.e. one line of the ASCII maze).
        update_grid( ROW, i )
        if i == KR:
            tmp_row = ROW[ : KC ] + 'X' + ROW[ KC + 1 : ]
            print >> sys.stderr, tmp_row
        else:
            print >> sys.stderr, ROW

    # Avoir la position de la position d'origine
    get_init_pos()

    # Avoir la position de la salle de commande (si disponible)
    get_target_pos()
    if target_pos :
        reach_target_pos = target_pos in cur_path

    # a partir de la position de Kirk, lister les possibilités de déplacement
    #  - exclure les murs
    #  - essayer d'exclure les positions déjà prise
    possible_move = get_possible_move ( grid, cur_pos, forbidden_pos = None )
    # print >> sys.stderr, "Mouvement possible", possible_move
    
    # Le choix du prochain mouvement se fait selon 3 modes :
    #  - découverte de la map/recherche du charactère C
    #  - aller vers le charactère C
    #  - retourner vers le charactère T
    # Pour chacun de ces modes, le prochain mouvement doit être décidé, 
    # en prenant en compte le compte à rebour A
    # Ce choix dépend du calcul d'un chemin optimal

    next_move = None
    # mode découverte
    if not target_pos:
        print >> sys.stderr, "mode découverte"
        # Calculer l'ensemble des positions inconnues accessibles 
        # les plus proches
        #unknows = get_nearest_accessible_unknowns( grid, cur_pos )
        #next_move = get_next_move( grid, cur_pos, unknows[0] )
        next_move = get_next_move_unknown( grid, cur_pos )

    # mode atteindre la salle commande
    elif not reach_target_pos:
        print >> sys.stderr, "mode atteindre la salle commande"
        next_move = get_next_move_target_known( grid, cur_pos )

    # mode retour à la position initiale
    else:
        print >> sys.stderr, "mode retour à la position initiale"
        next_move = get_next_move( grid, cur_pos, init_pos )

    # Write an action using print
    # To debug: print >> sys.stderr, "Debug messages..."
    
    # print "RIGHT" # Kirk's next move (UP DOWN LEFT or RIGHT).
    print next_move
