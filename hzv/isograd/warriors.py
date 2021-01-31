from __future__ import print_function
import sys

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

lines = []
for line in sys.stdin:
	lines.append(line.rstrip('\n'))

lines.pop(0)

lines = list( map( lambda s : list( map( lambda number : int( number ), s.split()) ), lines ) )

def count_nb_warriors( lines, i, already_used ):
    tribu = lines[ i ]
    nb_warriors = tribu[ 0 ]

    for i in range( 1, len( tribu ) ):
        idx_tribu = tribu[ i ]
        if ( idx_tribu not in already_used ):
            already_used.append( idx_tribu )
            nb_warriors += count_nb_warriors( lines, idx_tribu, already_used )

    return nb_warriors

max_warriors = 0
for i in range( len( lines ) ):
    nb_warriors = count_nb_warriors( lines, i, [] )
    max_warriors = max( max_warriors, nb_warriors )
    # local_print( nb_warriors )

print( max_warriors )
