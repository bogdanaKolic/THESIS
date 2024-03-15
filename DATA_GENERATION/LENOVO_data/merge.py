#!/usr/bin/env python3
# -*- coding: utf-8 -*-

files = ['attack', 'attack_ntmp', 'double_sided', 'no_flushing', 'random', 'rh4', 'rh8', 'rh9', 'rh10', 'rh20', 'stream']

with open("lenovo_data.csv", "w") as all_data_out:
    all_data_out.write("laptop,load,code_type,timestamp,llcm,l2ch,l1ch,stalls,label\n")
    with open("lenovo_data_no_load.csv", "w") as no_load_out:
        no_load_out.write("laptop,load,code_type,timestamp,llcm,l2ch,l1ch,stalls,label\n")
        for f in files:
            with open(f'no_load/{f}_lenovo.csv', 'r') as no_load_in:
                for line in no_load_in.readlines():
                    all_data_out.write(line.replace(' ', ''))
                    no_load_out.write(line.replace(' ', ''))
                
    with open("lenovo_data_load.csv", "w") as load_out:
        load_out.write("laptop,load,code_type,timestamp,llcm,l2ch,l1ch,stalls,label\n")
        for f in files:
            with open(f'load/{f}_load_lenovo.csv', 'r') as load_in:
                for line in load_in.readlines():
                    all_data_out.write(line.replace(' ', ''))
                    load_out.write(line.replace(' ', ''))
