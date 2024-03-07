#!/usr/bin/env python3
# -*- coding: utf-8 -*-

with open("all_data.csv", 'w') as outfile:
    with open("DELL_data/dell_data.csv", "r") as dell_in:
        for line in dell_in.readlines():
            outfile.write(line)
                
    with open("ASUS_data/asus_data.csv", 'r') as asus_in:
        for line in asus_in.readlines():
            outfile.write(line)
            
    with open("LENOVO_data/lenovo_data.csv", 'r') as lenovo_in:
        for line in lenovo_in.readlines():
            outfile.write(line)