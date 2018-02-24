
n = 26

l_trigit = []

i = 0
while 3 < i:
    rest = n - ( n / 3 ) * 3
    n   /= 3

    l_trigit.append( rest )
    i += 1

l_trigit.reverse( )

print( l_trigit )
