# -*- coding: utf-8 -*-
from datetime import date, timedelta
from copy import deepcopy
import cPickle as pk

events = []
paths  = []

def comp_event(e1, e2):
    if (e1[0] < e2[0]):
        return -1
    elif (e1[0] > e2[0]):
        return 1
    else :
        return 0

def beg_event(event):
    return event[0]

def end_event(event):
    return event[0] + timedelta(event[1])

def get_accessible_elt(l, event):
    res = []

    for elt in l :
        if end_event(event) < beg_event(elt):
            res.append(elt)
    return res

def next_paths():
    global events, paths
    for i in range(len(paths)):
        path = paths.pop()
        
        event_current = path[-1]
        accessible = get_accessible_elt(events, event_current)
        # print "path", path
        # print "events", events
        # print "event_current", event_current
        # print "accessible", accessible

        if accessible:
            for neighbourg in accessible:
                cp_path = deepcopy(path)
                cp_path.append(neighbourg)
                paths = [cp_path] + paths
        else:
            paths = [path] + paths

        # print "paths", paths
        # print ""

def keep_greater():
    global events, paths
    
    # On récupère le chemin le plus favorable
    tmp = paths[0]
    for path in paths:
        if ((end_event(path[-1]) < end_event(tmp[-1])) and \
                (len(tmp) <= len(path))):
            tmp = path
    best_path = tmp

    res = []
    for path in paths:
        if (len(path) < len(best_path)):
            continue
        if (end_event(path[-1]) <= end_event(best_path[-1])):
            res.append(path)

    return res

def main():
    global events, paths
    N = int(raw_input())

    for i in range(N):
        line = raw_input()
        year  = int(line[:4])
        month = int(line[5:7])
        day   = int(line[8:10])
        length= int(line[11:])
        d = date(year, month, day)
        events.append((d, length))

    # Classe les événements dans l'ordre chronologique
    events.sort(cmp=comp_event)

    # On initialise les chemins d'exploration
    for event_current in events:
        paths.append([event_current])

    # On itère une première fois
    next_paths()
    paths = keep_greater()

    save_serial = pk.dumps(paths)
    serial      = ""

    # Tant qu'on a pas converger vers un ensemble de chemins optimaux, on itère
    while(serial != save_serial):
        save_serial = pk.dumps(paths)
        
        next_paths()
        paths = keep_greater()
        serial = pk.dumps(paths)

    # print "resultat", paths ; print ""
    print len(paths[0])

main()
