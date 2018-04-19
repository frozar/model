#pythran export scrabble_score(str, str:int dict)
def scrabble_score(word, scoretable):
    score = 0
    for letter in word:
        if letter in scoretable:
            score += scoretable[letter]
    return score

# To compile it with pythran:
# $ pythran scrabble.py
