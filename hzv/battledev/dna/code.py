from __future__ import print_function
import sys
import itertools

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

lines = []
for line in sys.stdin:
    lines.append(line.rstrip('\n'))

lines.pop(0)
local_print(lines)

corres = { "A" : "T", "T" : "A", "C" : "G", "G" : "C" }
# local_print( corres["A"] )

N = len( lines )

dna_size = sum( list( map( lambda s : len(s ), lines ) ) ) // 2
# local_print( dna_size)

def get_binary( n ):
    binary = ""
    for i in range( N ):
        # local_print( i )
        # local_print( ( 10 >> i ) & 1 )
        binary += str( ( n >> i ) & 1 )
        # local_print( "" )

    binary = binary[ :: -1 ]
    return binary


# local_print( 2 ** N )
res = None
good_perm = []
for pos in range( 2 ** N ):
    pos = get_binary( pos )
    local_print( pos )
    chain_a = ""
    chain_b = ""
    position_a = []
    position_b = []
    for i in range( N ) :
        if ( pos[i] == "1" ):
            chain_a += lines[ i ]
            position_a.append( i )
        else :
            chain_b += lines[ i ]
            position_b.append( i )

    if ( len( chain_a ) != len( chain_b ) ):
        continue

    local_print( chain_a )
    local_print( chain_b )
    local_print( position_a )
    local_print( position_b )

    for perm_a in itertools.permutations( position_a ):
        for perm_b in itertools.permutations( position_b ):
            chain_a = ""
            chain_b = ""
            for i in perm_a :
                chain_a += lines[i]
            for i in perm_b :
                chain_b += lines[i]
            local_print( chain_a )
            local_print( chain_b )

            res = True
            for i in range( len( chain_a ) ):
                a   = chain_a[ i ]
                val = corres[ a ]
                if ( val != chain_b[ i ] ):
                    res = False

            local_print( res )
            if (res):
                good_perm.append( perm_a )
                good_perm.append( perm_b )
                break
        if (res):
            break

    local_print( "" )

    if (res):
        break

local_print( good_perm )
perm_a, perm_b = good_perm
chain_a = ""
chain_b = ""
for i in perm_a:
    chain_a += lines[i] + " "
for i in perm_b:
    chain_b += lines[i] + " "

res = chain_a[ : -1 ] + "#" + chain_b[ : -1 ]
print( res )

