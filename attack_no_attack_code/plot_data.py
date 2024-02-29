#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Feb 29 13:00:37 2024

@author: bogdana
"""

files = ['attack', 'attack_memset', 'attack_ntmp', 'double_sided', 'no_flushing', 'random', 'rh']

for f in files:
    with open(f'{f}_sampled.dat', 'r') as file:
        events = {}
        for line in file.readlines():
            if line[0] != '#' and line[0] != '\n':
                fields = line.split()
                count = 0
                i = 1
                while fields[i][0] in "0123456789":
                    count = 1000*count + int(fields[i])
                    i += 1
                if float(fields[0]) in events:
                    events[float(fields[0])] += (count, )
                else:
                    events[float(fields[0])] = (count, )
        with open(f'{f}.dat', 'w') as outfile:
            sorted_list = sorted(list(events.items()))
            for line in sorted_list:
                for fn in range(7):
                    if f == files[fn]:
                        outfile.write(f'{fn+1} ')
                outfile.write(f'{line[0]} {line[1][0]} {line[1][1]} [line[1][2] {line [1][3]}\n')