# -*- coding: utf-8 -*-
import sys

# Le nom du fichier a parser doit être donner en premier argument    
f_in = sys.argv[1]
f_in = open(f_in)

# On récupère le nombre de cas
N = int(f_in.readline())

def len_int(l):
    return l[1] - l[0] + 1

for i in range(1, N+1):
    [P, Q] = map(lambda x : int(x), f_in.readline().split())
    q = map(lambda x : int(x), f_in.readline().split())
    
    tab_bool = [0] * len(q)

    print "P", P
    # print "Q", Q
    print q
    # print tab_bool

    L = [[1, P]]
    tot_sum = 0
    while (q != []):
        print "L", L
        q0 = q[0]
        min = P
    ## On récupère le bon q0
        for l in L:
            lon_int = len_int(l)
            if (lon_int%2):
                median = [lon_int/2 + l[0]]
            else:
                median = [lon_int/2 - 1 + l[0], lon_int/2 + l[0]]

            print "l", l, "len_int", lon_int, " median", median
            for j in q:
                for k in median:
                    if (min > abs(k - j)):
                        min = abs(k - j)
                        q0 = j

            # print "q0", q0

    ## On détecte l'intervale contenant q0
        l0 = []
        for l in L:
            if ((l[0] <= q0) &(q0 <= l[1])):
                l0 = l
                break
        
    ## On retire l'intervalle de L, on le scinde en deux
        index = L.index(l0)
        # print index
        L.remove(l0)
        l1 = [l0[0], q0-1]
        l2 = [q0+1, l0[1]]

    ## On compte la longueur des deux intervalles
        print "l0", l0, " q0", q0
        print "l1", l1, " l2", l2
        tot_sum += len_int(l1) + len_int(l2)
        print "tot_sum", tot_sum
        print ""

    ## On insére les deux intervalles dans L au bon endroit
        L.insert(index, l2)
        L.insert(index, l1)

    ## On retire q0 de q
        q.remove(q0)

    print "Case #%d: %d" % (i, tot_sum)
    #print "tot", tot_sum
