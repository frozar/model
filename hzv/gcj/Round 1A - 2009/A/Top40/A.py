import sys

memo = dict()

def isHappy(n, b):
	if n == 1:
		return True
	if (n,b) in memo:
		return memo[(n,b)]

	s = 0
	i = n
	while i > 0:
		s += (i%b)*(i%b)
		i //= b
	memo[(n,b)] = False
	v = isHappy(s, b)
	memo[(n,b)] = v
	return v


CN = int(sys.stdin.next())
for C in range(1, CN+1):
	print "Case #%d:"%(C),

	B = map(int, sys.stdin.next().split(" "))

	n = 2
	while True:
		happy_all = True
		for b in B:
			if not isHappy(n,b):
				happy_all = False
		if happy_all:
			print n
			break
		n += 1
