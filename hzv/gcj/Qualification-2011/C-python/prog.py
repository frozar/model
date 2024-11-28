# -*- coding: utf-8 -*-
import sys

# Le nom du fichier a parser doit être donner en premier argument    
f_in = sys.argv[1]
f_in = open(f_in)

# On récupère le nombre de cas
T = int(f_in.readline())

for i in range(1, T+1):
    N = int(f_in.readline())
    tab = f_in.readline().split()
    tab = map(lambda x : int(x), tab)
    
    if (reduce(lambda x, y: x ^ y, tab) == 0) :
        print "Case #%d: %d" % (i, sum(tab) - min(tab))
    else :
        print "Case #%d: %s" % (i, "NO")
