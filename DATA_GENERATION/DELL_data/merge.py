#!/usr/bin/env python3
# -*- coding: utf-8 -*-

files = ['attack', 'attack_ntmp', 'double_sided', 'no_flushing', 'random', 'rh4', 'rh8', 'rh9', 'rh10', 'rh20', 'stream']

with open("dell_data.csv", "w") as all_data_out:
    with open("dell_data_no_load.csv", "w") as no_load_out:
        for f in files:
            with open(f'no_load/{f}_dell_processed.csv', 'r') as no_load_in:
                for line in no_load_in.readlines():
                    all_data_out.write(line)
                    no_load_out.write(line)
                
    with open("dell_data_load.csv", "w") as load_out:
        for f in files:
            with open(f'load/{f}_load_dell_processed.csv', 'r') as load_in:
                for line in load_in.readlines():
                    all_data_out.write(line)
                    load_out.write(line)
