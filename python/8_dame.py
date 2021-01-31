import random, string

GA_POPSIZE = 2048
# GA_POPSIZE = 20
GA_ELITRATE= 0.10 # elitism rate
GA_MUTATIONRATE = 0.25 # mutation rate
GA_TARGET = "Hello world!"
GA_CHARS = string.letters + string.digits + string.punctuation + string.whitespace
GA_POS_MAX = 8
GA_NB_DAME = 8

def randcoor():
    res = [ ( random.choice( xrange( GA_POS_MAX ) ),
              random.choice( xrange( GA_POS_MAX ) ) )
            for i in xrange( GA_NB_DAME ) ]

    return res

def fitness( l_dame_coor ):
    res = 0
    for i, coor in enumerate( l_dame_coor ) :
        for j, c in enumerate( l_dame_coor[ i + 1 : ] ) :
            if ( ( coor[ 0 ] == c[ 0 ] ) or ( coor[ 1 ] == c[ 1 ] ) ):
                res += 1
            elif( abs( coor[ 0 ] - c[ 0 ] ) == abs( coor[ 1 ] - c[ 1 ] ) ):
                res += 2

    return res

def mate(population, buffer):
    esize = int( GA_POPSIZE * GA_ELITRATE )
    for i in xrange( esize ): # Elitism
        buffer[i] = population[i]
    for i in range(esize, GA_POPSIZE):
        i1 = random.randint(0, GA_POPSIZE / 2)
        i2 = random.randint(0, GA_POPSIZE / 2)
        spos = random.randint( 0, GA_NB_DAME )
        buffer[i] = population[i1][:spos] + population[i2][spos:] # Mate
        if random.random() < GA_MUTATIONRATE: # Mutate
            pos = random.randint( 0, GA_NB_DAME - 1 )
            coor = [ ( random.choice( xrange( GA_POS_MAX ) ),
                       random.choice( xrange( GA_POS_MAX ) ) ) ]
            buffer[i] = buffer[i][:pos] + coor + buffer[i][pos+1:]

population = []
for i in xrange( GA_POPSIZE ):
    population.append( randcoor() )

buffer = []
for i in xrange( GA_POPSIZE ):
    buffer.append( randcoor() )


it = 0
while True:
    population = sorted(population, key=lambda c: fitness(c))

    print "[%03d]\tBest (%04d)\t%s" % ( it, fitness(population[0]), population[0] )
    it += 1
    
    if not fitness( population[ 0 ] ) : # We're done, best match found
        break
    mate( population, buffer )
    population, buffer = buffer, population
