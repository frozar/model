from sys import stderr
from operator import itemgetter

def dist( ruine_states, cur_pos, nb_ruine,
          target_letter, len_alphabet, len_range ) :
    """ 
    Return: [ ( distance, way ), ... ]
    way = 0: standard way
        = 1: reverse way
    """

    res = []

    # origin_states = [ ruine_states[ ( cur_pos + offset ) % nb_ruine ]
    #                   for offset in range( -1, 1 +1 ) ]

    # print >> stderr, "origin_states", origin_states

    for offset in range( -len_range, len_range +1 ) :

        ruine_state = ruine_states[ ( cur_pos + offset ) % nb_ruine ]

        if ( abs( ruine_state - target_letter ) <= ( len_alphabet / 2 ) ) :
            res.append( ( abs( ruine_state - target_letter ) + abs( offset ), 0 ) )
        else :
            if ( ruine_state < target_letter ) :
                res.append( ( ruine_state + len_alphabet - target_letter + abs( offset ), 1 ) )
            else :
                res.append( ( target_letter + len_alphabet - ruine_state + abs( offset ), 1 ) )

    return res


magic_phrase = raw_input()

print >> stderr, "magic_phrase", magic_phrase
l_phrase = map( lambda c : ord( c ) - ord( 'A' ) + 1 if c != ' ' else 0, list( magic_phrase ) )

print >> stderr, "l_phrase", l_phrase


res = ""
nb_ruine = 30
ruine_states = [ 0 ] * nb_ruine
len_alphabet = ord( 'Z' ) - ord( 'A' ) + 1 + 1

cur_pos = 0

for target_letter in l_phrase :

    len_range = 7
    dist_n_move = dist( ruine_states, cur_pos, nb_ruine,
                        target_letter, len_alphabet, len_range )

    # print >> stderr, "dist_n_move", dist_n_move

    dist_min = min( map( itemgetter( 0 ), dist_n_move ) )

    l_indexes = filter( lambda couple : couple[ 1 ][ 0 ] == dist_min,
                        list( enumerate( dist_n_move ) ) )
    dist_min, switch_way = l_indexes[ 0 ][ 1 ]
    
    l_indexes = map( itemgetter( 0 ), l_indexes )

    # print >> stderr, "l_indexes", l_indexes

    offset = l_indexes[ 0 ] - len_range

    if ( offset < 0 ) :
        res += "<" * ( - offset )
    elif( offset > 0 ) :
        res += ">" * ( offset )

    cur_pos = ( cur_pos + offset ) % nb_ruine

    nb_move = abs( offset )
    nb_switch = dist_min - nb_move

    if ( switch_way == 0 ) :
        if ( ruine_states[ cur_pos ] < target_letter ) :
            res += "+" * nb_switch
        elif ( ruine_states[ cur_pos ] > target_letter ) :
            res += "-" * nb_switch
            
    elif ( switch_way == 1 ) :
        if ( ruine_states[ cur_pos ] < target_letter ) :
            res += "-" * nb_switch
        elif ( ruine_states[ cur_pos ] > target_letter ) :
            res += "+" * nb_switch
        
    res += "."
    ruine_states[ cur_pos ] = target_letter

    # print >> stderr, ""
                
# print res
print "+>-[<.-->++++++++++++++]"
