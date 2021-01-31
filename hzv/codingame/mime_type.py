import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

d_types = {}

n = int(raw_input()) # Number of elements which make up the association table.
q = int(raw_input()) # Number Q of file names to be analyzed.
for i in xrange(n):
     # ext: file extension
     # mt: MIME type.
    ext, mt = raw_input().split()
    ext = ext.lower()

    d_types[ ext ] = mt

l_files = []
for i in xrange(q):
    fname = raw_input() # One file name per line.
    l_files.append( fname )

# Write an action using print
# To debug: print >> sys.stderr, "Debug messages..."

# For each of the Q filenames, display on a line the corresponding MIME type. If there is no corresponding type, then display UNKNOWN.
for f in l_files:
    split_f = f.split(".")
    if ( len( split_f ) == 1 ):
        print "UNKNOWN"
    else:
        loop_ext = split_f[ -1 ]
        loop_ext = loop_ext.lower()

        if ( loop_ext in d_types.keys() ):
            print d_types[ loop_ext ]
        else:
            print "UNKNOWN"
