import re
import pprint

mapping = {'A': 'add', 'B': 'addi', 'C': 'sub', 'D': 'subi', 'E': 'and', 'F': 'andi', 'G': 'or', 'H': 'ori', 'I': 'sll', 'J': 'srl', 'K': 'nor', 'L': 'lw', 'M': 'sw', 'N': 'beq', 'O': 'bneq', 'P': 'j'}

sequence = 'JDBLEMIAHPNKGOCF'

R_TYPE = ['add', 'sub', 'and', 'or', 'nor']
S_TYPE = ['sll', 'srl']
I_TYPE = ['addi', 'subi', 'andi', 'ori', 'beq', 'bneq', 'lw', 'sw']
J_TYPE = ['j']

reg_table = {'$zero': '0000', '$t0': '0101', '$t1': '0001', '$t2': '0010', '$t3': '0011', '$t4': '0100', '$sp': '0111'}

ALU_OP = {'and': '000', 'or': '001', 'add': '010', 'sub': '011', 'nor': '100', 'sll': '101', 'srl': '110'}
# this ^ is for normal (non optimized) ALU, last one is 'zero': '111' which is never used

instruction = {}
for i in range(len(sequence)):
    instruction[mapping[sequence[i]]] = bin(i)[2:].zfill(4)

REG_DST_4_7, REG_DST_0_3 = '0', '1' 
ALU_SRC_REG, ALU_SRC_INST = '0', '1'
MEM_TO_REG_ALU_RESULT, MEM_TO_REG_MEM_RESULT = '0', '1'

split_pattern = r'[ \t,\(\)]+'

def split(line):
    return list(filter(None, re.split(split_pattern, line)))

labels = {}

def generate_control_rom_row(reg_dst, reg_write, branch, branch_type, jump, mem_read, mem_to_reg, mem_write, alu_src, alu_op):
    return hex(int(reg_dst + reg_write + branch + branch_type + jump + mem_read + mem_to_reg + mem_write + alu_src + alu_op, 2))[2:].zfill(3)

def generate_control_rom():
    print('Control ROM:')
    print('\t', end='')
    for i in range(len(sequence)):
        inst = mapping[sequence[i]]
        if inst in R_TYPE:
            print(generate_control_rom_row(REG_DST_0_3, '1', '0', '0', '0', '0', MEM_TO_REG_ALU_RESULT, '0', ALU_SRC_REG, ALU_OP[inst]), end=' ')
        elif inst in S_TYPE:
            print(generate_control_rom_row(REG_DST_4_7, '1', '0', '0', '0', '0', MEM_TO_REG_ALU_RESULT, '0', ALU_SRC_INST, ALU_OP[inst]), end=' ')
        elif inst in I_TYPE[:4]:
            print(generate_control_rom_row(REG_DST_4_7, '1', '0', '0', '0', '0', MEM_TO_REG_ALU_RESULT, '0', ALU_SRC_INST, ALU_OP[inst[:-1]]), end=' ')
        elif inst in I_TYPE[4:6]:
            is_bneq = '1' if inst == 'bneq' else '0'
            print(generate_control_rom_row(REG_DST_4_7, '0', '1', is_bneq, '0', '0', MEM_TO_REG_ALU_RESULT, '0', ALU_SRC_REG, ALU_OP['sub']), end=' ')
        elif inst in I_TYPE[6:8]:
            if inst == 'sw':
                print(generate_control_rom_row(REG_DST_4_7, '0', '0', '0', '0', '0', MEM_TO_REG_ALU_RESULT, '1', ALU_SRC_INST, ALU_OP['add']), end=' ')
            else:
                print(generate_control_rom_row(REG_DST_4_7, '1', '0', '0', '0', '1', MEM_TO_REG_MEM_RESULT, '0', ALU_SRC_INST, ALU_OP['add']), end=' ')
        elif inst in J_TYPE:
            print(generate_control_rom_row(REG_DST_4_7, '0', '0', '0', '1', '0', MEM_TO_REG_ALU_RESULT, '0', ALU_SRC_REG, ALU_OP['add']), end=' ')
    print()

def generate_label_location(lines):
    processed_lines = []
    for line in lines:
        parts = [part.strip() for part in line.split(':')]
        if len(parts) == 2:
            labels[parts[0]] = len(processed_lines)
            if parts[1] != '':
                processed_lines.append(parts[1])
        else:
            processed_lines.append(line)
    return processed_lines

def change(line_no, opcode, *args):
    bin_inst = instruction[opcode]
    
    if opcode in R_TYPE:
        bin_inst += reg_table[args[1]]
        bin_inst += reg_table[args[2]]
        bin_inst += reg_table[args[0]]
    
    elif opcode in S_TYPE:
        bin_inst += reg_table[args[1]]
        bin_inst += reg_table[args[0]]
        bin_inst += bin(int(args[2]))[2:].zfill(4)
    
    elif opcode in I_TYPE:
        if opcode in I_TYPE[:6]:
            bin_inst += reg_table[args[1]]
            if opcode in I_TYPE[:4]:
                immediate = int(args[2])
            else:
                immediate = labels[args[2]] - (line_no + 1)
        else:
            bin_inst += reg_table[args[2]]
            immediate = int(args[1])
            
        bin_inst += reg_table[args[0]]
        
        if immediate < 0:
            immediate = 2**4 + immediate
        bin_inst += bin(immediate)[2:].zfill(4)
    
    elif opcode in J_TYPE:
        target_line = labels[args[0]]
        bin_inst += bin(target_line)[2:].zfill(8)
        bin_inst += '0000'
    
    return bin_inst

SP_ENABLED = True

if __name__ == '__main__':
    print('Instructions OPCODE:')
    pprint.pprint(instruction)
    print()

    generate_control_rom()
    print()

    with open('input.asm', 'r') as file:
        lines = [line.split(r'//')[0].strip() for line in file]                 
        lines = list(filter(lambda line : line != '', lines))   
    
    if SP_ENABLED:
        push_pop_re = r'(push|pop)[ ]+(\$t[0-4]|\$zero)'
        original_lines = lines.copy()
        lines = ['addi $sp, $zero, 15']                         
        for line in original_lines:
            matched = re.search(push_pop_re, line)
            if matched is None:
                lines.append(line)
            else:
                if matched.group(1) == 'push':
                    lines.append(f'sw {matched.group(2)}, 0($sp)')        
                    lines.append('subi $sp, $sp, 1')              
                else:
                    lines.append('addi $sp, $sp, 1')              
                    lines.append(f'lw {matched.group(2)}, 0($sp)')        
    
    lines = generate_label_location(lines)                      

    print('ASM Lines (Post-Processing):')
    pprint.pprint(lines)
    
    bin_lines = []
    hex_lines = []

    for i in range(len(lines)):
        tokens = split(lines[i])
        bin_line = change(i, *tokens)
        bin_lines.append(bin_line)
        hex_lines.append(hex(int(bin_line, 2))[2:].zfill(4) + '\n')

    with open('output.bin', 'w') as bin_file:
        for i in range(len(lines)):
            formatted_bin = ' '.join([bin_lines[i][j:j+4] for j in range(0, len(bin_lines[i]), 4)])
            bin_file.write(formatted_bin + ' : ' + lines[i] + '\n')
    
    with open('output.hex', 'w') as hex_file:
        hex_file.writelines(hex_lines)

    print("\nConversion successful! Created 'output.bin' and 'output.hex'.")