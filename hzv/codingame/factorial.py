
def factorial( n ):
    res = 1
    for i in range( 1, n +1 ):
        res *= i
    return res

def combinaison ( k, n ):
    return factorial( n ) / ( factorial( k ) * factorial( n - k ) )

print factorial ( 3 )
print factorial ( 0 )


print combinaison ( 2, 256 ) ** 2
print combinaison ( 2, 128 ) ** 2
print combinaison ( 2**5, 2**9 ) ** 2
