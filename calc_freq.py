
inputfile = open('deltas.out')

line = inputfile.readline()

addr_dict = {}
delta_dict = {}
pc_dict = {}

while line:
    line_split = line.split()
    pc = int(line_split[0])
    addr = int(line_split[1])
    delta = int(line_split[2])
    if addr_dict.__contains__(addr):
        addr_dict[addr] += 1
    else:
        addr_dict[addr] = 1

    if pc_dict.__contains__(pc):
        pc_dict[pc] += 1
    else:
        pc_dict[pc] = 1

    if delta_dict.__contains__(delta):
        delta_dict[delta] += 1
    else:
        delta_dict[delta] = 1

    line = inputfile.readline()
    
print('start write')
inputfile.close()

addr_sorted = sorted(addr_dict.items(), key=lambda item:item[1], reverse=True)
delta_sorted = sorted(delta_dict.items(), key=lambda item:item[1], reverse=True)
pc_sorted = sorted(pc_dict.items(), key=lambda item:item[1], reverse=True)

outputfile = open('calc.out', 'w')
outputfile.write('addr: \n')
for i in addr_sorted:
    outputfile.writelines(str(i) + '\n')

outputfile.write('pc: \n')
for i in pc_sorted:
    outputfile.write(str(i) + '\n')

outputfile.write('delta: \n')
for i in delta_sorted:
    outputfile.write(str(i) + '\n')

outputfile.close()