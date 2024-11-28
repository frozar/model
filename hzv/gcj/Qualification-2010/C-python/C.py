# -*- coding: utf-8 -*-
import sys

# Le nom du fichier a parser doit être donner en premier argument    
f_in = sys.argv[1]
f_in = open(f_in)

# On récupère le nombre de cas
T = int(f_in.readline())

R = 0

def print_res(i, tot_sum):
    if R == 0:
        print "Case #%d: %d" % (i, tot_sum)
        return 1
    return 0

for i in range(1, T+1):
    [R, k, N] = map(lambda x : int(x), f_in.readline().split())
    g = map(lambda x : int(x), f_in.readline().split())

    # On détecte le cycle, construction de L
    L = []
    tot_sum = 0

    l = []
    index = n = 0
    begin_cycle = -1
    # On construit L tel que len(L) >= N
    while (1):
        if ((sum(l) + g[index] <= k) & (n < N)):
            l.append(g[index])
            index = (index + 1) % N
            n += 1
        else:
            if ((not l in L) | (sum(map(len, L)) < N )):
                L.append(l)
                l = []
                n = 0
            else:
                begin_cycle = L.index(l)
                break
    
    # On traite les index avant cycle
    for j in range(0, begin_cycle):
        tot_sum += sum(L[j])
        R -= 1
        if (print_res(i, tot_sum)):
            continue

    # On compte le nombre de cycle qu'on peut encore effectuer
    len_cycle = len(L) - begin_cycle
    nb_cycle = R / len_cycle
    value_cycle = sum(map(sum, L[begin_cycle:len(L)]))

    tot_sum += nb_cycle * value_cycle
    R = R%len_cycle
    if (print_res(i, tot_sum)):
        continue

    # On traite les index qu'il reste à effectuer
    tot_sum += sum(map(sum, L[begin_cycle:begin_cycle+R]))
    R = 0
    if (print_res(i, tot_sum)):
        continue
