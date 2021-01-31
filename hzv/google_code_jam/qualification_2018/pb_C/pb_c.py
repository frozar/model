from __future__ import print_function
import sys

def local_print(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

nb_cases = int(raw_input())
l_file = []
for i in xrange(1, nb_cases + 1):
    nb_int = int(raw_input())
    l = [int(x) for x in raw_input().split(" ")]
    l_file.append(l)

local_print(l_file)

for i_case, l in enumerate( l_file ):
    even_l = sorted(l[::2])
    odd_l = sorted(l[1::2])

    local_print("even_l", even_l)
    local_print("odd_l", odd_l)

    adjusted = False
    if (len(even_l) == len(odd_l) + 1):
        odd_l += [None]
        adjusted = True

    res_l = sum(map(lambda t : list(t), zip(even_l, odd_l)), [])

    if (adjusted):
        res_l.pop()
    local_print("res_l", res_l)

    len_l = len(res_l)
    fail = False
    for i in xrange(len_l - 1):
        if (res_l[i] > res_l[i+1]):
            print("Case #%d: %d" % ( i_case + 1, i ))
            fail = True
            break

    if (not fail):
        print("Case #%d: OK" % ( i_case + 1 ))

    sys.stdout.flush()
