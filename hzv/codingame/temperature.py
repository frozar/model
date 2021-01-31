n=int(raw_input())
t=map(int,raw_input().split())
print 0 if n==0 else sorted(filter(lambda x:abs(x)==min(map(lambda x:abs(x),t)),t))[-1]
