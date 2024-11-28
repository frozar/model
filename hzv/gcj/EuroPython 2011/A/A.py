# -*- coding: utf-8 -*-
import sys

# Le nom du fichier a parser doit être donner en premier argument    
f_in = sys.argv[1]
f_in = open(f_in)

# On récupère le nombre de cas
T = int(f_in.readline())

for i in range(1, T+1):
    s = f_in.readline().strip('\n')
    tmp = s.lower()
    
    if (tmp[-1] in 'aeiou'):
        print "Case #%d: %s is ruled by a queen." % (i, s)
    elif (tmp[-1] == 'y'):
        print "Case #%d: %s is ruled by nobody." % (i, s)
    else :
        print "Case #%d: %s is ruled by a king." % (i, s)
