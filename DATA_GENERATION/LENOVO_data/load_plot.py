with open('lenovo_data_load.csv', 'r') as infile:
    lines = infile.readlines()
    with open('lenovo_load_attack.dat', 'w') as out_attack:
        attack_num = 0
        with open('lenovo_load_no_attack.dat', 'w') as out_benchmarks:
            no_attack_num = 0
            with open('lenovo_load_stream.dat', 'w') as out_stream:
                stream_num = 0
                sample = 0
                for line in lines[1:]:
                    data = line.split(',')
                    if data == ['lenovo\n']:
                        continue
                    if data[8] == 'attack\n':
                        out_attack.write(f'1 {sample} {data[4]} {data[5]} {data[6]} {data[7]}\n')
                        attack_num += 1
                        sample += 1
                    elif data[2] == 'stream':
                        out_stream.write(f'2 {sample} {data[4]} {data[5]} {data[6]} {data[7]}\n')
                        stream_num += 1
                        sample += 1
                    else:
                        out_benchmarks.write(f'3 {sample} {data[4]} {data[5]} {data[6]} {data[7]}\n')
                        no_attack_num += 1
                        sample += 1