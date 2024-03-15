#!/usr/bin/env python3
# -*- coding: utf-8 -*-

files = ['attack', 'attack_ntmp', 'double_sided', 'no_flushing', 'random', 'rh4', 'rh8', 'rh9', 'rh10', 'rh20', 'stream']
no_attack = ['no_flushing', 'random', 'stream']
           
for f in files:
    with open(f'{f}_dell.dat', 'r') as file:
        events = {}
        for line in file.readlines():
            if line[0] != '#' and line[0] != '\n':
                fields = line.split()
                if fields[1][0] != '<':
                    count = 0
                    i = 1
                    while fields[i][0] in "0123456789":
                        count = 1000*count + int(fields[i])
                        i += 1
                    if float(fields[0]) in events:
                        events[float(fields[0])] += (count, )
                    else:
                        events[float(fields[0])] = (count, )
        if f in no_attack:
            label = 'no_attack'
        else:
            label = 'attack'
        if f == 'no_flushing':
            file = 'no_flush'
        else:
            file = f
        with open(f'{f}_dell_processed.csv', 'w') as outfile:
            sorted_list = sorted(list(events.items()))
            for line in sorted_list:
                if len(line[1]) != 4:
                    continue
                outfile.write(f'dell, 0, {file}, {line[0]}, {line[1][0]}, {line[1][1]}, {line[1][2]}, {line [1][3]}, {label}\n') 
                
for f in files:
    with open(f'{f}_load_dell.dat', 'r') as file:
        events = {}
        for line in file.readlines():
            if line[0] != '#' and line[0] != '\n':
                fields = line.split()
                if fields[1][0] != '<':
                    count = 0
                    i = 1
                    while fields[i][0] in "0123456789":
                        count = 1000*count + int(fields[i])
                        i += 1
                    if float(fields[0]) in events:
                        events[float(fields[0])] += (count, )
                    else:
                        events[float(fields[0])] = (count, )
        if f in no_attack:
            label = 'no_attack'
        else:
            label = 'attack'
        if f == 'no_flushing':
            file = 'no_flush'
        else:
            file = f
        with open(f'{f}_load_dell_processed.csv', 'w') as outfile:
            sorted_list = sorted(list(events.items()))
            for line in sorted_list:
                if len(line[1]) != 4:
                    continue
                outfile.write(f'dell, 8, {file}, {line[0]}, {line[1][0]}, {line[1][1]}, {line[1][2]}, {line [1][3]}, {label}\n') 