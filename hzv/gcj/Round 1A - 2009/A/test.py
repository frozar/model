file = "A-small-practice"
inp = open(file + ".in")

import pdb

# retourne le somme des carres des chiffres de num
# num etant exprimer dans la base b
def dsum(num, b):
  n = num
  sumd = 0
  while n > 0:
    sumd += (n % b)**2
    n /= b
  return sumd

# num : le nombre considere
# b : la base considere
# hp : tableau de booleen, Vrai si l'indice est un "happy number"
# Retourne un booleen, Vrai si num est "happy number" dans la base b (construction d'un tableau de booleen hp pour eviter un nombre important de recursion)
def happy(num, b, hp):
  if num in hp:
    return hp[num]
  if num == 1:
    return True
  ds = dsum(num,b)
  hp[num] = False
  if happy(ds, b, hp):
    hp[num] = True
  return hp[num]


for it, case in enumerate(list(inp)[1:]):
  bases = map(int, case.split())
  hps=[]
  for i in range(11):
    hps.append({})
  print hps
  for num in range(2,10000000):
    # si le nombre num est "happy number" dans toutes les bases b
    if all([happy(num, b, hps[b]) for b in bases]):
      break
  print "Case #%d: %d" % (it + 1, num)
