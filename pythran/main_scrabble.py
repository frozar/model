import scrabble

word       = "toto"
scoretable = dict({ ("t", 1),
                    ("o", 8) })
score      = scrabble.scrabble_score(word, scoretable)

print "score =", score

# To execute this script:
# $ python main_scrabble.py
