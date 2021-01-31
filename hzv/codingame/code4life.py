import sys
import math

from sys import stderr

from types import SimpleNamespace as sns

from operator import itemgetter

def perr( s, verbose = True ) :
    if ( verbose ) :
        print( str( s ), file = stderr )

# Bring data on patient samples from the diagnosis machine to the laboratory
# with enough molecules to produce medicine!

def get_carried_sample( l_sample ) :
    return list( filter( lambda s : s.carried_by == 0, l_sample ) )


def get_nb_carried_sample( l_sample ) :
    return len( get_carried_sample( l_sample ) )


def id_to_str_molecule( id_molecule ) :
    if ( id_molecule == 0 ) :
        return "A"
    elif ( id_molecule == 1 ) :
        return "B"
    elif ( id_molecule == 2 ) :
        return "C"
    elif ( id_molecule == 3 ) :
        return "D"
    elif ( id_molecule == 4 ) :
        return "E"


def get_required_molecule( l_carried_sample ) :
    required_molecule = [ 0 ] * 5
    for s in l_carried_sample :
        required_molecule[ 0 ] += s.cost_a
        required_molecule[ 1 ] += s.cost_b
        required_molecule[ 2 ] += s.cost_c
        required_molecule[ 3 ] += s.cost_d
        required_molecule[ 4 ] += s.cost_e

    return required_molecule


def get_carried_molecule( my_player ) :
    carried_molecule = [ 0 ] * 5
    carried_molecule[ 0 ] = my_player.storage_a
    carried_molecule[ 1 ] = my_player.storage_b
    carried_molecule[ 2 ] = my_player.storage_c
    carried_molecule[ 3 ] = my_player.storage_d
    carried_molecule[ 4 ] = my_player.storage_e

    return carried_molecule


def is_undiagnosed( s ) :
    return ( ( s.cost_a == -1 ) and \
             ( s.cost_b == -1 ) and \
             ( s.cost_c == -1 ) and \
             ( s.cost_d == -1 ) and \
             ( s.cost_e == -1 ) )


def get_nb_carried_molecule( my_player ) :
    return ( my_player.storage_a + \
             my_player.storage_b + \
             my_player.storage_c + \
             my_player.storage_d + \
             my_player.storage_e )


project_count = int(input())
for i in range(project_count):
    a, b, c, d, e = [int(j) for j in input().split()]

# game loop
while True:
    l_player = [ ]

    for i in range(2):
        target, eta, score, \
            storage_a, storage_b, storage_c, storage_d, storage_e, \
            expertise_a, expertise_b, expertise_c, expertise_d, expertise_e = input().split()
        eta = int(eta)
        score = int(score)
        storage_a = int(storage_a)
        storage_b = int(storage_b)
        storage_c = int(storage_c)
        storage_d = int(storage_d)
        storage_e = int(storage_e)
        expertise_a = int(expertise_a)
        expertise_b = int(expertise_b)
        expertise_c = int(expertise_c)
        expertise_d = int(expertise_d)
        expertise_e = int(expertise_e)

        # perr( "target : " + target )
        # perr( "score : %d " % score )

        l_player.append( sns( target = target,
                              eta = eta,
                              score = score,
                              storage_a = storage_a,
                              storage_b = storage_b,
                              storage_c = storage_c,
                              storage_d = storage_d,
                              storage_e = storage_e,
                              expertise_a = expertise_a,
                              expertise_b = expertise_b,
                              expertise_c = expertise_c,
                              expertise_d = expertise_d,
                              expertise_e = expertise_e ) )

    my_player = l_player[ 0 ]
    perr( my_player )

    available_a, available_b, available_c, available_d, available_e = [int(i) for i in input().split()]

    l_sample = [ ]
    sample_count = int(input())
    for i in range(sample_count):
        sample_id, carried_by, rank, expertise_gain, health, cost_a, cost_b, cost_c, cost_d, cost_e = input().split()
        sample_id = int(sample_id)
        carried_by = int(carried_by)
        rank = int(rank)
        health = int(health)
        cost_a = int(cost_a)
        cost_b = int(cost_b)
        cost_c = int(cost_c)
        cost_d = int(cost_d)
        cost_e = int(cost_e)

        # perr( "carried_by : %d" % ( carried_by ) )
        # perr( "rank : %d" % ( rank ) )

        # if ( i < 2 ) :
        #     perr( "sample : %d %d %d %d %d %d %d %d %d " %
        #           ( sample_id, carried_by, rank, health, cost_a, cost_b, cost_c, cost_d, cost_e ) )

        l_sample.append( sns( sample_id = sample_id,
                              carried_by = carried_by,
                              rank = rank,
                              health = health,
                              cost_a = cost_a,
                              cost_b = cost_b,
                              cost_c = cost_c,
                              cost_d = cost_d,
                              cost_e = cost_e ) )
        
    l_sample.sort( key = lambda s : s.health, reverse = True )

    perr( "BEGIN SAMPLES" )
    # for s in l_sample[ : 3 ] :
    for s in l_sample :
        perr( s )
    perr( "END SAMPLES" )
    
    # Write an action using print
    # To debug: print("Debug messages..." = , file=sys.stderr)

    # SAMPLES
    # DIAGNOSIS
    # MOLECULES
    # LABORATORY
    l_carried_sample    = get_carried_sample( l_sample )
    l_required_molecule = get_required_molecule( l_carried_sample )
    l_carried_molecule  = get_carried_molecule( my_player )

    go_to_diagnosis = False
    target_sample   = -1
    for s in l_carried_sample :
        if is_undiagnosed( s ) :
            target_sample   = s.sample_id
            go_to_diagnosis = True
            break


    nb_carried_molecule = get_nb_carried_molecule( my_player )


    perr( "nb_carried_sample %d" % get_nb_carried_sample( l_sample ) )
    if ( get_nb_carried_sample( l_sample ) < 3 ) :
        if ( my_player.target != "SAMPLES" ) :
            print( "GOTO SAMPLES" )
        elif ( get_nb_carried_sample( l_sample ) < 3 ) :
            print( "CONNECT 3" )

    # if ( my_player.target != "DIAGNOSIS" ) :
    elif ( go_to_diagnosis ) :
        if ( my_player.target != "DIAGNOSIS" ) :
            print( "GOTO DIAGNOSIS" )
        else :
            print( "CONNECT %d" % target_sample )

    elif ( nb_carried_molecule < 10 ) :
        if ( my_player.target != "MOLECULES" ) :
            print( "GOTO MOLECULES" )

        else :

            if ( 10 <= nb_carried_molecule ) :
                print( "GOTO LABORATORY" )

            else :
                l_carried_sample = get_carried_sample( l_sample )
                l_carried_sample.sort( key = lambda s : s.health, reverse = True )

                carried_molecule = get_carried_molecule( my_player )

                need_molecule = False
                for s in l_carried_sample :
                    carried_molecule[ 0 ] -= s.cost_a
                    carried_molecule[ 1 ] -= s.cost_b
                    carried_molecule[ 2 ] -= s.cost_c
                    carried_molecule[ 3 ] -= s.cost_d
                    carried_molecule[ 4 ] -= s.cost_e

                    if ( list( filter( lambda x : x < 0, carried_molecule ) ) ) :
                        need_molecule = True
                        break
                    
                if ( need_molecule ) :
                    id_molecule = -1
                    for i, m in enumerate( carried_molecule ) :
                        if ( m < 0 ) :
                            id_molecule = i
                            break

                    print ( "CONNECT " + id_to_str_molecule( id_molecule ) )
        
    else :
        if ( my_player.target != "LABORATORY" ) :
            print( "GOTO LABORATORY" )
        else :
            perr( "################## LAST CASE" )
            id_sample     = None
            target_sample = None
            for s in get_carried_sample( l_sample ) :
                # perr( "SAMPLE" )
                # perr( s )
                if ( ( s.cost_a <= my_player.storage_a ) and \
                     ( s.cost_b <= my_player.storage_b ) and \
                     ( s.cost_c <= my_player.storage_c ) and \
                     ( s.cost_d <= my_player.storage_d ) and \
                     ( s.cost_e <= my_player.storage_e ) ) :
                    id_sample = s.sample_id
                    target_sample = s
                    break
            # perr( "id_sample" )
            # perr( id_sample )
            # perr( "target_sample" )
            # perr( target_sample )

            if ( id_sample ) :
                print( "CONNECT %d" % id_sample )
                carried_molecule[ 0 ] -= target_sample.cost_a
                carried_molecule[ 1 ] -= target_sample.cost_b
                carried_molecule[ 2 ] -= target_sample.cost_c
                carried_molecule[ 3 ] -= target_sample.cost_d
                carried_molecule[ 4 ] -= target_sample.cost_e

            else :
                print( "GOTO MOLECULES" )

