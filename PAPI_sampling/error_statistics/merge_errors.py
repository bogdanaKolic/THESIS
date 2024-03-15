with open("errors.csv", 'w') as out:
    with open("asus_no_load.csv", 'r') as in_asus:
        lines = in_asus.readlines()
        for line in lines:
            out.write(line)
    with open("asus_load.csv", 'r') as in_asus_load:
        lines = in_asus_load.readlines()
        for line in lines:
            out.write(line)
    with open("lenovo_no_load.csv", 'r') as in_lenovo:
        line = in_lenovo.readlines()
        for line in lines:
            out.write(line)
    with open("lenovo_load.csv", 'r') as in_lenovo_load:
        line = in_lenovo_load.readlines()
        for line in lines:
            out.write(line)