# -*- coding: utf-8 -*-
import sys

# Le nom du fichier a parser doit être donner en premier argument    
f_in = sys.argv[1]
f_in = open(f_in)

# On récupère le nombre de cas
T = int(f_in.readline())

for i in range(1, T+1):
    L = f_in.readline().split()

    res = 0
    [N, S, p] = map(lambda x: int(x), L[:3])
    t = map(lambda x: int(x), L[3:])

    t_mod = map(lambda x: [x%3, x/3], t)

    # print L
    # print  [N, S, p]
    # print t
    # print t_mod

    for l in t_mod:
        if (l[0] == 0):
            if (p <= (l[1])):
                res += 1
            elif ((l[1] > 0) & (p <= (l[1]+1)) & (S > 0)):
                S -= 1
                res += 1
        elif (l[0] == 1):
            res += (p <= (l[1]+1))
        elif (l[0] == 2):
            if (p <= (l[1]+1)):
                res += 1
            elif ((p <= (l[1]+2)) & (S > 0)):
                S -= 1
                res += 1

    print "Case #%d: %s" % (i, res)
