inputfile = open('result.out')
outputfile = open('deltas.out', 'w')

line = inputfile.readline()
last_line = line
line_list = line.split()
addr_last = int(line_list[2])

while line:
    line = inputfile.readline()

    line_list = line.split()

    pc = int(line_list[0])
    addr = int(line_list[2])
    delta = addr - addr_last
    
    addr_last = addr

    outline = '{} {} {}\n'.format(pc, addr, delta)
    outputfile.write(outline)

inputfile.close()
outputfile.close()