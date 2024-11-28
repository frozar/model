# -*- coding: utf-8 -*-
import sys

# Le nom du fichier a parser doit être donner en premier argument    
f_in = sys.argv[1]
f_in = open(f_in)

# On récupère le nombre de cas
T = int(f_in.readline())

trans_dico = dict()

in_str='ejp mysljylc kd kxveddknmc re jsicpdrysi rbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcd de kr kd eoya kw aej tysr re ujdr lkgc jv'
out_str='our language is impossible to understand there are twenty six factorial possibilities so it is okay if you want to just give up'

for i in range(len(in_str)):
    trans_dico[in_str[i]] = out_str[i]

trans_dico['z'] = 'q'
trans_dico['q'] = 'z'

for i in range(1, T+1):
    G = f_in.readline().strip("\n")

    res = str()
    for l in G:
        res += trans_dico[l]

    print "Case #%d: %s" % (i, res)
    
