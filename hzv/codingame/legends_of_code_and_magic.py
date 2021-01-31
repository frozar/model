import sys
import math
from typing import NamedTuple

def pp(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

class Player(NamedTuple):
    health: int
    mana: int
    deck: int
    rune: int

class Card(NamedTuple):
    card_number: int
    instance_id: int
    location: int
    card_type: int
    cost: int
    attack: int
    defense: int
    abilities: str
    my_health_change: int
    opponent_health_change: int
    card_draw: int

    def __str__(self):
        return "(" + \
            ", ".join([str(self.card_number),
                       str(self.instance_id),
                       str(self.location),
                       str(self.card_type),
                       str(self.cost),
                       str(self.attack),
                       str(self.defense),
                       str(self.abilities),
                       str(self.my_health_change),
                       str(self.opponent_health_change),
                       str(self.card_draw)]) + ")"

class InGame(object):
    def __init__(self, cards):
        self.cards = cards

    def getMyHand(self):
        return list(filter( lambda c : c.location == 0, self.cards) )

    def getCreatureInHand(self):
        return list(filter( lambda c : c.card_type == 0, self.getMyHand()) )

    def getMyItem(self, item_type = 0):
        if (item_type == 0):
            return list(filter( lambda c : (c.card_type != 0), self.getMyHand()))
        else:
            return list(filter( lambda c : (c.card_type == item_type), self.getMyHand()))

    def getMySide(self):
        return list(filter( lambda c : c.location == 1, self.cards) )

    def getHisSide(self):
        return list(filter( lambda c : c.location == -1, self.cards) )


def readInput():
    players = []
    for i in range(2):
        player_health, player_mana, player_deck, player_rune = [int(j) for j in input().split()]
        p = Player(player_health, player_mana, player_deck, player_rune)
        players.append(p)

    opponent_hand = int(input())
    card_count = int(input())
    cards = []
    for i in range(card_count):
        card_number, instance_id, location, card_type, cost, attack, defense, \
            abilities, my_health_change, opponent_health_change, card_draw = input().split()
        card_number = int(card_number)
        instance_id = int(instance_id)
        location = int(location)
        card_type = int(card_type)
        cost = int(cost)
        attack = int(attack)
        defense = int(defense)
        my_health_change = int(my_health_change)
        opponent_health_change = int(opponent_health_change)
        card_draw = int(card_draw)
        card = Card(card_number, instance_id, location, card_type, cost, attack, defense,
                    abilities, my_health_change, opponent_health_change, card_draw)
        
        cards.append(card)
    
    return players, InGame(cards)

def takeDecision(players, in_game):
    to_print = []

    my_hand = in_game.getMyHand()
    my_side = in_game.getMySide()
    his_side = in_game.getHisSide()

    me = players[0]
    # pp("me:", me)

    # If possible, summon creature
    pp("my hand:", "\n".join(map(lambda s : str(s), my_hand)))
    pp("my mana:", me.mana)
    my_mana = me.mana
    # for card in my_hand:
    for card in in_game.getCreatureInHand():
        if (card.cost <= my_mana):
            my_mana -= card.cost
            to_print.append("SUMMON {}".format(card.instance_id))

            # If the creature has the charge capacity, can attack immediatly
            if ('C' in card.abilities):
                my_side.append(card)

    # Deal with item cards
    green_item = in_game.getMyItem(1)
    red_item = in_game.getMyItem(2)
    blue_item = in_game.getMyItem(3)

    # pp("green_item:", "\n".join(map(str, green_item)))
    my_side.sort(reverse = True, key = lambda c : c.attack)
    if (my_side):
        greatest_attack_id = my_side[0].instance_id
        for card in green_item:
            if (card.cost <= my_mana):
                my_mana -= card.cost
                to_print.append("USE {} {}".format(card.instance_id, greatest_attack_id))

    his_side.sort(reverse = True, key = lambda c : c.defense)
    if (his_side):
        greatest_defense_id = his_side[0].instance_id
        for card in red_item:
            if (card.cost <= my_mana):
                my_mana -= card.cost
                to_print.append("USE {} {}".format(card.instance_id, greatest_defense_id))

    for card in blue_item:
        if (card.cost <= my_mana):
            my_mana -= card.cost
            to_print.append("USE {} -1".format(card.instance_id))

                
    # Deal with guard creature in priority
    guard_to_attack = list(filter(lambda c : 'G' in c.abilities, his_side))
    guard_to_attack.sort(reverse = True, key = lambda c : c.defense)
    other_to_attack = list(filter(lambda c : not 'G' in c.abilities, his_side))
    other_to_attack.sort(reverse = True, key = lambda c : c.defense)
    if (other_to_attack):
        other_to_attack = other_to_attack[ :1]

    my_side.sort(reverse = True, key = lambda c : c.attack)
    to_attack = guard_to_attack + other_to_attack

    for his_card in to_attack:
        his_defense = his_card.defense
        while(my_side):
            my_card = my_side.pop(0)
            his_defense -= my_card.attack
            to_print.append("ATTACK {} {}".format(my_card.instance_id, his_card.instance_id))
            if (his_defense <= 0):
                break

    # If no more creature on the opponent side, attack directly the opponent
    while(my_side):
        my_card = my_side.pop(0)
        to_print.append("ATTACK {} -1".format(my_card.instance_id))

    return to_print


idx_iter = 0

# game loop
while True:
    idx_iter += 1

    players, in_game = readInput()

    if (idx_iter <= 30):
        print("PASS")

    else :
        to_print = takeDecision(players, in_game)
        
        if (to_print):
            print(";".join(to_print))
        else:
            print("PASS")


