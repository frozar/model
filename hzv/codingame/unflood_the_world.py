from sys import stderr

# Really useful class when one cares about pt with coordinates
class Pt( object ) :

    def __init__( self, i, j ) :
        self.i = i
        self.j = j

    def __str__( self ) :
        return "(%d, %d)" % ( self.i, self.j )

    def __eq__( self, pt ) :
        return ( self.i, self.j ) == ( pt.i, pt.j )

    def __cmp__( self, pt ) :
        if ( ( self.i, self.j ) < ( pt.i, pt.j ) ) :
            return -1
        elif ( ( self.i, self.j ) < ( pt.i, pt.j ) ) :
            return 1
        else :
            return 0

    def get_neighbour( self ) :
        global h, w
        
        res = []
    
        N_neig, S_neig, W_neig, E_neig = True, True, True, True
        
        if ( self.i == 0 ) :
            N_neig = False
        if ( self.i == ( h - 1 ) ) :
            S_neig = False
        if ( self.j == 0 ) :
            W_neig = False
        if ( self.j == ( w - 1 ) ) :
            E_neig = False
    
        if ( N_neig ) :
            res.append( Pt( self.i - 1, self.j ) )
        if ( S_neig ) :
            res.append( Pt( self.i + 1, self.j ) )
        if ( W_neig ) :
            res.append( Pt( self.i, self.j - 1 ) )
        if ( E_neig ) :
            res.append( Pt( self.i, self.j + 1 ) )
            
        return res

# Read the input
w, h = [int(i) for i in raw_input().split()]

grid = [ 0 ] * h
for i in xrange( h ) :
    grid[ i ] = [ int( j ) for j in raw_input().split() ]


# Identify every contiguous region where each cell
# is at the same level
l_grps = []

every_pt = []
for i in xrange( h ) :
    for j in xrange( w ) :
        every_pt.append( Pt( i, j ) )

while ( every_pt ) :
    first_pt = every_pt[ 0 ]
    cur_grp = [ first_pt ]
    l_explore = [ first_pt ]

    while ( l_explore ) :
        cur_pt = l_explore.pop( 0 )
        every_pt.remove( cur_pt )

        for pt in cur_pt.get_neighbour( ) :
            if ( grid[ cur_pt.i ][ cur_pt.j ] == grid[ pt.i ][ pt.j ] ) :
                if ( pt not in cur_grp ) :
                    l_explore.append( pt )
                    cur_grp.append( pt )
                 
    l_grps.append( cur_grp )


# Count the number of regien where no any neighbour region as the
# lower level than itself
cpt = 0
for grp in l_grps :
    found = False
    for pt in grp :
        for n_pt in pt.get_neighbour( ) :
            if ( grid[ n_pt.i ][ n_pt.j ] < grid[ pt.i ][ pt.j ] ) :
                found = True
                break

    if ( not found ) :
        cpt += 1


print cpt
