n = int(raw_input())
x = int(raw_input()) % ( n * n )

lines = [ list( raw_input() ) for i in xrange( n ) ]
lines = [ line[ :: ( 1, -1 )[ i % 2 == 0 ] ]
          for i, line in enumerate( map( lambda l : list( l ), zip( *lines ) ) ) ]

string = reduce( lambda x, y : x + y, sum( lines, [] ) )
string = string[ -x : ] + string[ : -x ]
    
lines = [ string[ i * n : ( i + 1 ) * n ][ :: ( 1, -1 )[ i % 2 == 0 ] ]
          for i in xrange( n ) ]
lines = [ reduce( lambda x, y : x + y, line )
          for line in map( lambda l : list( l ), zip( *lines ) ) ]

print "\n".join( lines )
