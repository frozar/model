# -*- coding: utf-8 -*-
import sys

# Le nom du fichier a parser doit être donner en premier argument    
f_in = sys.argv[1]
f_in = open(f_in)

# On récupère le nombre de cas
T = int(f_in.readline())

for i in range(1, T+1):
    N = int(f_in.readline())
    
    lines = []
    for j in range(N):
        lines.append(f_in.readline().upper())

    res = []
    for l in lines:
        for j in range(len(l)-1, 0, -1):
            for k in range(j):
                # print tmp, len(l)
                # print tmp, j, l[tmp:tmp+len(l)-j]
                sub=l[k:k+len(l)-j]
                print sub, sum(map(lambda x: sub in x, lines))
                if (sum(map(lambda x: sub in x, lines)) == 1):
                    res.append(sub)
                    break                
            break
        # break

    # s = f_in.readline().strip('\n')
    # tmp = s.lower()
    
    # if (tmp[-1] in 'aeiou'):
    #     print "Case #%d: %s is ruled by a queen." % (i, s)
    # elif (tmp[-1] == 'y'):
    #     print "Case #%d: %s is ruled by nobody." % (i, s)
    # else :
    #     print "Case #%d: %s is ruled by a king." % (i, s)
