
import atexit

def print_int( i ):
    print "toto", i

atexit.register( print_int, 0 )

i = 0 
while ( True ):
    i = i
