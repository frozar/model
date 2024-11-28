# -*- coding: utf-8 -*-
import sys

# Le nom du fichier a parser doit être donner en premier argument    
f_in = sys.argv[1]
f_in = open(f_in)

# On récupère le nombre de cas
T = int(f_in.readline())

for i in range(1, T+1):
    l = map(lambda x : int(x), f_in.readline().split())
    [R, k, N] = l
    # print "R", R
    # print "k", k
    # print "N", N

    g = map(lambda x : int(x), f_in.readline().split())
    # print "g", g
    
    index = 0
    tot_sum = 0
    for j in range(R):
        loc_sum = 0
        n = 0
        while (1):
            if ((loc_sum + g[index] <= k) & (n < N)):
                loc_sum += g[index]
                index = (index + 1) % N
                n += 1
            else:
                break
            # print "loc", loc_sum
        tot_sum += loc_sum
        #print tot_sum
    print "Case #%d: %d" % (i, tot_sum)
