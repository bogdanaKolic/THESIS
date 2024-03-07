#!/usr/bin/env python3
# -*- coding: utf-8 -*-

with open(f'stream_sampled.dat', 'r') as file:
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
    with open(f'stream.dat', 'w') as outfile:
        sorted_list = sorted(list(events.items()))
        for line in sorted_list:
            if len(line[1]) != 4:
                continue
            outfile.write('2')
            outfile.write(f'{line[0]} {line[1][0]} {line[1][1]} {line[1][2]} {line [1][3]}\n') 