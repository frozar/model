import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

def get_right_direction( next_pos, clone_pos, origin_direction ) :
    if ( next_pos - clone_pos < 0 ):
        return "LEFT"
    elif ( next_pos - clone_pos > 0 ):
        return "RIGHT"
    else :
        return origin_direction

        
# nb_floors: number of floors
# width: width of the area
# nb_rounds: maximum number of rounds
# exit_floor: floor on which the exit is found
# exit_pos: position of the exit on its floor
# nb_total_clones: number of generated clones
# nb_additional_elevators: ignore (always zero)
# nb_elevators: number of elevators
nb_floors, width, nb_rounds, exit_floor, exit_pos, nb_total_clones, nb_additional_elevators, nb_elevators = [int(i) for i in raw_input().split()]

all_elevator = []

for i in xrange(nb_elevators):
    # elevator_floor: floor on which this elevator is found
    # elevator_pos: position of the elevator on its floor
    elevator_floor, elevator_pos = [int(j) for j in raw_input().split()]
    # print >> sys.stderr, "elevator_floor, elevator_pos", elevator_floor, elevator_pos
    all_elevator.append( ( elevator_floor, elevator_pos ) )

# print >> sys.stderr, "all_elevator", all_elevator
# print >> sys.stderr, "nb_floors, width, nb_rounds, exit_floor, exit_pos, nb_total_clones, nb_additional_elevators, nb_elevators", nb_floors, width, nb_rounds, exit_floor, exit_pos, nb_total_clones, nb_additional_elevators, nb_elevators
# print >> sys.stderr, "exit_floor, exit_pos", exit_floor, exit_pos

# game loop
while True:
    # clone_floor: floor of the leading clone
    # clone_pos: position of the leading clone on its floor
    # direction: direction of the leading clone: LEFT or RIGHT
    clone_floor, clone_pos, direction = raw_input().split()
    clone_floor = int(clone_floor)
    clone_pos = int(clone_pos)

    # Write an action using print
    # To debug: print >> sys.stderr, "Debug messages..."

    # print >> sys.stderr, "clone_floor, clone_pos, direction", clone_floor, clone_pos, direction

    # action: WAIT or BLOCK
    if ( ( clone_floor, clone_pos ) == ( -1, -1 ) ):
        print "WAIT"
        continue

    if ( clone_floor == exit_floor ) :
        if ( get_right_direction( exit_pos, clone_pos, direction ) == direction ) :
            print "WAIT"
            continue
        else :
            print "BLOCK"
            continue

    target_elevator = None
    for elevator_floor, elevator_pos in all_elevator :
        if ( clone_floor == elevator_floor ) :
            target_elevator = elevator_floor, elevator_pos
            break
        
    if ( get_right_direction( target_elevator[ 1 ], clone_pos, direction ) == direction ) :
        print "WAIT"
        continue
    else :
        print "BLOCK"
        continue
