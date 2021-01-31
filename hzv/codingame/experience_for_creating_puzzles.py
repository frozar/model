level = int(raw_input())
xp = int(raw_input())
n = int(raw_input())
for i in range( n ) :
    xp -= 300
    while ( xp <= 0 ):
        level += 1
        xp += int( level * level ** 0.5 * 10 )
print "%d\n%d" % ( level, xp )
