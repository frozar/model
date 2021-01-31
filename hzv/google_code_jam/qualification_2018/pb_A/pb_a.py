from __future__ import print_function
import sys

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

nb_cases = int(raw_input())
l_file = []
for i in xrange(1, nb_cases + 1):
  l = raw_input().split(" ")
  l = [int(l[0]), l[1]]
  l_file.append(l)

local_print(l_file[:4])

for i_case, case in enumerate( l_file ):
    D, P = case
    local_print(D, P)

    nb_charge = 0
    origin_damage = 0
    d_P = dict()
    for c in P :
        if ( c == 'S' ):
            origin_damage += 2 ** nb_charge
            d_P[2 ** nb_charge] = d_P.get(2 ** nb_charge, 0) + 1
        else:
            nb_charge += 1

    local_print(origin_damage)

    if (origin_damage <= D):
        print("Case #%d: 0" % ( i_case + 1 ))
        continue
    
    local_print(d_P.keys())
    local_print(set([1]) == set([1]))
    current_damage = origin_damage
    nb_changes = 0
    while (set(d_P.keys()) != set([1])):
        key = filter(lambda x : x != 1, sorted(d_P.keys()))[0]
        local_print(key)
        if (d_P[key] > 0):
            d_P[key] -= 1
            d_P[key / 2] = d_P.get(key / 2, 0) + 1
            current_damage -= key / 2
            nb_changes += 1

        if (d_P[key] == 0):
            d_P.pop(key)

        if (current_damage <= D):
            break

    local_print("current_damage", current_damage)
    local_print("nb_changes", nb_changes)
    
    if (current_damage <= D):
        print("Case #%d: %d" % ( i_case + 1, nb_changes ))
    else:
        print("Case #%d: IMPOSSIBLE" % ( i_case + 1 ))

