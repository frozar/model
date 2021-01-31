
def printCoins(N, X):
  # N digits, 10 1s, N-10 0s
  for i in range(N-10):
    for j in range(N-10-i):
      for k in range(N-10-i-j):
        l = N-10-i-j-k
        # print "l", l
        assert l >= 0
        template = "11{}11{}11{}11{}11"
        output = template.format("0"*i, "0"*j, "0"*k, "0"*l)
        factors = "3 2 5 2 7 2 3 2 11"
        print output, factors
        X -= 1
        if X == 0:
          return
  # If we get here, we didn't mine enough jamcoins!
  assert False

nb = 50
printCoins( 20, nb )
