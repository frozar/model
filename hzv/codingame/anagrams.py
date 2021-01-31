import sys

phrase = raw_input()

# To debug: print >> sys.stderr, "Debug messages..."

print >> sys.stderr, "origin", phrase
print >> sys.stderr, map( lambda c : ( ord( c ) - ord( 'A' ) ) % 2 == 1 ,
                          phrase )

origin_space = filter( lambda cuple : cuple[ 1 ] == ' ', enumerate( phrase ) )

print >> sys.stderr, origin_space
phrase = phrase.replace( ' ', '' )

to_reverse = filter( lambda cuple : ( ord( cuple[ 1 ] ) - ord( 'A' ) ) % 2 == 1, enumerate( phrase ) )
print >> sys.stderr, to_reverse
print >> sys.stderr, phrase
for i, c in zip( zip( *to_reverse )[ 0 ], zip( *to_reverse )[ 1 ][ :: -1 ]  ) :
    phrase = phrase[ : i ] + c + phrase[ i + 1 : ]
print >> sys.stderr, phrase

# to_reverse = zip( zip( *to_reverse )[ 0 ], zip( *to_reverse )[ 1 ][ :: -1 ]  )
# print >> sys.stderr, to_reverse

# print >> sys.stderr, phrase
# print >> sys.stderr, [ phrase[ i : cuple[ 0 ] ] + cuple[ 1 ] for i, cuple in enumerate( to_reverse ) ]

print "answer"
