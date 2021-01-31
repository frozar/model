import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

dictionnaire = []

n = int(raw_input())
for i in xrange(n):
    w = raw_input()
    dictionnaire.append( w )
    
letters = raw_input()

# print >> sys.stderr, "dictionnaire", dictionnaire

letter_weight = {}
for l in [ "e", "a", "i", "o", "n", "r", "t", "l", "s", "u" ] :
    letter_weight[ l ] = 1
for l in [ "d", "g" ] :
    letter_weight[ l ] = 2
for l in [ "b", "c", "m", "p" ] :
    letter_weight[ l ] = 3
for l in [ "f", "h", "v", "w", "y" ] :
    letter_weight[ l ] = 4
for l in [ "k" ] :
    letter_weight[ l ] = 5
for l in [ "j", "x" ] :
    letter_weight[ l ] = 8
for l in [ "q", "z" ] :
    letter_weight[ l ] = 10

tmp_res = []

for word in dictionnaire :
    cp_word = list( word )
    
    b_next = False

    for l in letters :
        if ( l in cp_word ) :
            cp_word.remove( l )

    if ( cp_word == [] ) :
        tmp_res.append( ( sum( [ letter_weight[ l ] for l in word ] ), word ) )
    

tmp_res.sort( reverse = True )
# print >> sys.stderr, "tmp_res", tmp_res

max_score = tmp_res[ 0 ][ 0 ]
tmp_res_2 = []
for score, word in tmp_res :
    if ( score == max_score ) :
        tmp_res_2.append( word )

res = [ ( dictionnaire.index( word ), word ) for word in tmp_res_2 ]
res.sort()

# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."
# print "invalid word"
print res[ 0 ][ 1 ]
