wolfram_code = map( int, bin( int(raw_input()) )[ 2 : ].zfill( 8 )[ :: -1 ] )
n = int(raw_input())
start_pattern = raw_input()
print start_pattern
for k in range( n - 1 ) :
    tmp_pattern = start_pattern[ -1 : ] + start_pattern + start_pattern[ : 1 ]
    wk_pattern = map( lambda x : ''.join( map( lambda y : '1' if y == '@' else '0',
                                               list( x ) ) ),
                      [ tmp_pattern[ i - 1 : i + 2 ] for i in \
                        range( 1, 1 + len( start_pattern ) ) ] )
    wk_pattern = ''.join( map( lambda x : '@' if wolfram_code[ int( x, 2 ) ] == 1 else '.',
                               wk_pattern ) )
    print wk_pattern
    start_pattern = wk_pattern
