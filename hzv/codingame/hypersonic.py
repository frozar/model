from sys import stderr
import math

from operator import itemgetter

class Pt( object ) :

    def __init__( self, y, x ) :
        self.x = x
        self.y = y

    def __str__( self ) :
        return "(%d, %d)" % ( self.x , self.y )

    def __eq__( self, pt ) :
        return ( self.y, self.x ) == ( pt.y, pt.x )

    def __cmp__( self, pt ) :
        if ( ( self.y, self.x ) < ( pt.y, pt.x ) ) :
            return -1
        elif ( ( self.y, self.x ) > ( pt.y, pt.x ) ) :
            return 1
        else :
            return 0

    def dist( self, p0 ) :
        return abs( self.y - p0.y ) + abs( self.x - p0.x )

    def true_dist( self, p0, grid, l_entity ) :
        already = set( )
        already.add( ( self, 0 ) )

        l_explore = [ ( self, 0 ) ]
    
        bomb = get_bomb( l_entity )

        res = 0
        
        while ( l_explore ) :

            cur_pos, d = l_explore.pop( 0 )

            if ( cur_pos == p0 ) :
                res = d
                break
            
            l_neighbour = cur_pos.get_neighbour( )

            # print >> stderr, "l_neighbour", l_neighbour

            for n in l_neighbour :
                if ( n not in list( map( lambda x : x[ 0 ], already ) ) ) :
                    if ( ( grid[ n.y ][ n.x ] == "." ) and \
                         ( n not in map( lambda b : b.pos, bomb ) ) ) :
                        already.add( ( n, d + 1 ) )
                        l_explore.append( ( n, d + 1 ) )

        return res

    def get_neighbour( self ) :
        global height, width
        
        res = []
    
        N_neig, S_neig, W_neig, E_neig = True, True, True, True

        if ( self.y == 0 ) :
            N_neig = False
        if ( self.y == ( height - 1 ) ) :
            S_neig = False
        if ( self.x == 0 ) :
            W_neig = False
        if ( self.x == ( width - 1 ) ) :
            E_neig = False
    
        if ( N_neig ) :
            res.append( Pt( self.y - 1, self.x ) )
        if ( S_neig ) :
            res.append( Pt( self.y + 1, self.x ) )
        if ( W_neig ) :
            res.append( Pt( self.y, self.x - 1 ) )
        if ( E_neig ) :
            res.append( Pt( self.y, self.x + 1 ) )
            
        return res


class Entity( object ) :
    def __init__( self, entity_type, owner, x, y, param_1, param_2 ) :
        self.entity_type = entity_type
        self.owner       = owner
        self.pos         = Pt( y, x )
        self.param_1     = param_1
        self.param_2     = param_2

    def __str__( self ) :
        res = "%d %d p:%s %d %d" % ( self.entity_type, self.owner, self.pos, self.param_1, self.param_2 )
        return res
        
width, height, my_id = [int(i) for i in raw_input().split()]

# count = 13 * 11 / 2
count = 0
sub_count = 0

# x, y  = 0, -1
# x_way = "left"


def get_my_ent( l_entity ) :
    res = None
    for e in l_entity :
        if ( ( e.entity_type == 0 ) and ( e.owner == my_id ) ) :
            res = e

    return res


def get_my_bomb( l_entity ) :
    return filter( lambda e : ( e.entity_type == 1 ) and ( e.owner == my_id ),
                   l_entity )


def get_bomb( l_entity ) :
    return filter( lambda e : e.entity_type == 1,
                   l_entity )


def get_safe_pos( my_ent, grid, l_entity ) :
    reachable_pos = get_reachable_pos( my_ent.pos, grid, l_entity )

    safe_pos = []
    bomb_pos = get_my_bomb( l_entity ) + [ my_ent ]

    if ( not bomb_pos ) :
        return reachable_pos

    for p in reachable_pos :
        for b in bomb_pos :
            if ( p.y != b.pos.y ) and ( p.x != b.pos.x ) :
                safe_pos.append( p )
            elif ( p.y == b.pos.y ) and ( my_ent.param_2 < abs( p.y - b.pos.y ) ) :
                safe_pos.append( p )
            elif ( p.x == b.pos.x ) and ( my_ent.param_2 < abs( p.x - b.pos.x ) ) :
                safe_pos.append( p )

    safe_pos.sort( key = lambda p : p.true_dist( my_ent.pos, grid, l_entity ) )
                
    return safe_pos


def get_nearest_box( my_pos, grid, l_entity ) :
    # boxes = []
    # for i in xrange( height ) :
    #     for j in xrange( width ) :
    #         if ( grid[ i ][ j ].isdigit() ) :
    #             pos_box = Pt( i, j )
    #             boxes.append( ( my_pos.dist( pos_box ), pos_box ) )

    boxes = get_reachable_box( my_pos, grid, l_entity )
    boxes = map( lambda b : ( my_pos.dist( b ), b ), boxes )

    print >> stderr, "0 boxes", map( lambda x : str( x[ 1 ] ), boxes )
    # print >> stderr, "boxes", boxes

    my_bomb = get_my_bomb( l_entity )

    print >> stderr, "my_bomb", map( lambda e : str( e.pos ), my_bomb )

    for box in boxes[ : ] :
        d, pos_box = box
        for bomb in my_bomb :
            # TODO: check the walls
            if ( ( ( bomb.pos.y == pos_box.y ) and \
                 ( bomb.pos.dist( pos_box ) < bomb.param_2 ) ) or \
                 ( ( bomb.pos.x == pos_box.x ) and \
                   ( bomb.pos.dist( pos_box ) < bomb.param_2 ) ) ) :
                boxes.remove( box )
                break
    
    boxes.sort( )
    print >> stderr, "1 boxes", map( lambda x : str( x[ 1 ] ), boxes )

    if ( not boxes ) :
        return None
    
    dist_min = boxes[ 0 ][ 0 ]
    # print >> stderr, "0 boxes", map( lambda x : str( x[ 1 ] ), boxes )
    boxes = filter( lambda c : c[ 0 ] == dist_min, boxes )
    # print >> stderr, "1 boxes", map( lambda x : str( x[ 1 ] ), boxes )
    boxes = map( lambda c : c[ 1 ], boxes )
    print >> stderr, "2 boxes", map( lambda x : str( x ), boxes )
    first_box = boxes[ 0 ]

    return first_box


def get_target_position( my_pos, grid, l_entity ) :

    boxes   = get_reachable_box( my_pos, grid, l_entity )

    if ( not boxes ) :
        return None
    
    my_bomb = get_my_bomb( l_entity )

    print >> stderr, "my_bomb", map( lambda e : str( e.pos ), my_bomb )

    for box in boxes[ : ] :
        for bomb in my_bomb :
            # TODO: check the walls
            if ( ( ( bomb.pos.y == box.y ) and \
                 ( bomb.pos.dist( box ) < bomb.param_2 ) ) or \
                 ( ( bomb.pos.x == box.x ) and \
                   ( bomb.pos.dist( box ) < bomb.param_2 ) ) ) :
                boxes.remove( box )
                break

    reachable_pos = get_reachable_pos( my_pos, grid, l_entity )

    positions = []
    
    for pos in reachable_pos :
        score = 0
        l_neighbour = pos.get_neighbour( )
        for n in l_neighbour :
            if ( n in boxes ) :
                score += 1

        if ( score != 0 ) :
            # positions.append( ( score, my_pos.dist( pos ), pos ) )
            positions.append( ( score, my_pos.true_dist( pos, grid, l_entity ), pos ) )

    # Sort by multiple attribute (it's written in reverse order):
    #  1st: good position target for bomb
    #  2nd: distance to boxes
    # doc: http://stackoverflow.com/questions/4233476/sort-a-list-by-multiple-attributes
    positions.sort( key = itemgetter( 1 ) )
    positions.sort( reverse = True, key = itemgetter( 0 ) )
    
    print >> stderr, "positions", ",".join( map( lambda c : "| %d %d " % ( c[ 0 ], c[ 1 ] ) + str( c[ 2 ] ), positions ) )

    first_pos = None
    if ( positions ) :
        first_pos = positions[ 0 ][ 2 ]

    return first_pos
    

def get_reachable_pos( pos, grid, l_entity ) :
    res = set( )

    res.add( pos )

    l_explore = [ pos ]
    
    bomb = get_bomb( l_entity )

    while ( l_explore ) :
        # print >> stderr, "l_explore", ",".xoin( map( lambda x : str( x ), l_explore ) )
        # print >> stderr, "res", ",".xoin( map( lambda x : str( x ), list( res ) ) )

        cur_pos = l_explore.pop( 0 )
        l_neighbour = cur_pos.get_neighbour( )

        for n in l_neighbour :
            # print >> stderr, "n not in res", n, n in list( res )
            # for p in res :
            #     print >> stderr, "p", p, p == n#, hash( p ), hash( n ), hash( p ) == hash( n )
            if ( n not in list( res ) ) :
                if ( ( grid[ n.y ][ n.x ] == "." ) and \
                     ( n not in map( lambda b : b.pos, bomb ) ) ) :
                    res.add( n )
                    l_explore.append( n )

    return res
                
    # print >> stderr, "l_neighbour", str( l_neighbour[ 0 ] ), str( l_neighbour[ 1 ] ), map( lambda x : str( x ), l_neighbour )
    # print >> stderr, "l_neighbour", map( lamda x : str( x ), l_neighbour )

    # exit()


def get_reachable_box( pos, grid, l_entity ) :

    reachable_pos = get_reachable_pos( pos, grid, l_entity )

    boxes = []
    
    for pos in reachable_pos :
        l_neighbour = pos.get_neighbour( )
        for n in l_neighbour :
            if ( grid[ n.y ][ n.x ].isdigit() ) :
                boxes.append( n )

    return list( set( boxes ) )
            

# game loop
while True:

    grid = []
    
    for i in xrange(height):
        row = raw_input()
        grid.append( row )

    # for r in grid :
    #     print >> stderr, r

    entities = int(raw_input())
    l_entity = []
    for i in xrange(entities):
        entity_type, owner, x0, y0, param_1, param_2 = [int(j) for j in raw_input().split()]

        e = Entity( entity_type, owner, x0, y0, param_1, param_2 )
        l_entity.append( e )
        
    # print >> stderr, "\n".join( map( lambda e : str( e ), l_entity ) )

    my_ent = get_my_ent( l_entity )
    print >> stderr, "my_ent", my_ent

    nearest_box = get_nearest_box( my_ent.pos, grid, l_entity )

    print >> stderr, "nearest_box", nearest_box

    target_position = get_target_position( my_ent.pos, grid, l_entity )
    
    print >> stderr, "target_position", target_position

    if ( target_position is not None ) :
        if ( my_ent.pos != target_position ) :
            print "MOVE", target_position.x, target_position.y
        else :
            if ( my_ent.param_1 != 0 ) :
                print "BOMB", target_position.x, target_position.y
            else :
                safe_pos = get_safe_pos( my_ent, grid, l_entity )

                # print >> stderr, "safe_pos", ", ".join( map( lambda p : str( p ), safe_pos ) )
                # print >> stderr, "target_position in safe_pos", target_position in safe_pos
                if ( safe_pos ) :
                    if ( target_position in safe_pos ) :
                        print "MOVE", target_position.x, target_position.y
                    else :
                        first_safe_pos = safe_pos[ 0 ]
                        print "MOVE", first_safe_pos.x, first_safe_pos.y
                        
                else :
                    print "MOVE", my_ent.pos.x, my_ent.pos.y, "DEAD"

    else :
        safe_pos = get_safe_pos( my_ent, grid, l_entity )

        print >> stderr, "0 safe_pos", ",".join( map( lambda x : str( x ), safe_pos ) )

        if ( safe_pos ) :
            first_safe_pos = safe_pos[ 0 ]
            print "MOVE", first_safe_pos.x, first_safe_pos.y
        else :
            print "MOVE", my_ent.pos.x, my_ent.pos.y, "DEAD"

        
