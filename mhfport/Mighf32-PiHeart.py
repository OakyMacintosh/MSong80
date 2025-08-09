import struct
import sys

# Constants
MEM_SIZE = 65536
NUM_REGS = 8

class MHFHeader:
    def __init__(self, data):
        # Unpack the header: signature(4), version(2), flags(2), head_offset(4),
        # code_offset(4), data_offset(4), symbol_offset(4), file_size(4)
        unpacked = struct.unpack('<4sHHLLLLL', data[:28])
        self.signature = unpacked[0]
        self.version = unpacked[1] 
        self.flags = unpacked[2]
        self.head_offset = unpacked[3]
        self.code_offset = unpacked[4]
        self.data_offset = unpacked[5]
        self.symbol_offset = unpacked[6]
        self.file_size = unpacked[7]

class MighfCPU:
    def __init__(self):
        self.regs = [0] * NUM_REGS
        self.pc = 0
        self.memory = bytearray(MEM_SIZE)
        self.running = False

    def load_mhfb(self, filename):
        """Load MHF bytecode file into memory"""
        try:
            with open(filename, 'rb') as f:
                # Read header
                header_data = f.read(28)
                if len(header_data) < 28:
                    raise ValueError("File too small for header")
                
                header = MHFHeader(header_data)
                
                # Validate signature
                if header.signature[:3] != b'MHF':
                    raise ValueError("Invalid signature")
                
                # Read remaining file content into memory
                remaining_size = header.file_size - 28
                if remaining_size > 0:
                    content = f.read(remaining_size)
                    # Copy content to memory starting at head_offset
                    start_addr = header.head_offset
                    end_addr = min(start_addr + len(content), MEM_SIZE)
                    self.memory[start_addr:end_addr] = content[:end_addr - start_addr]
                
                return header.code_offset
                
        except OSError as e:
            print(f"Error opening file: {e}")
            sys.exit(1)

    def run(self, entry_point):
        """Execute the loaded program"""
        self.pc = entry_point
        self.running = True
        
        while self.running:
            if self.pc >= MEM_SIZE:
                print(f"[ERROR] PC out of bounds: 0x{self.pc:04X}")
                break
                
            opcode = self.memory[self.pc]
            self.pc += 1
            
            if opcode == 0x00:  # NOP
                pass
                
            elif opcode == 0x01:  # HALT
                print("\n[HALT] CPU halted.")
                self.running = False
                
            elif opcode == 0x10:  # LOAD reg, imm
                if self.pc + 1 >= MEM_SIZE:
                    print("[ERROR] Instruction extends beyond memory")
                    break
                reg = self.memory[self.pc]
                self.pc += 1
                imm = self.memory[self.pc]
                self.pc += 1
                
                if reg < NUM_REGS:
                    self.regs[reg] = imm
                    
            elif opcode == 0x20:  # ADD reg, reg
                if self.pc + 1 >= MEM_SIZE:
                    print("[ERROR] Instruction extends beyond memory")
                    break
                reg1 = self.memory[self.pc]
                self.pc += 1
                reg2 = self.memory[self.pc]
                self.pc += 1
                
                if reg1 < NUM_REGS and reg2 < NUM_REGS:
                    # Handle overflow by keeping only lower 8 bits
                    self.regs[reg1] = (self.regs[reg1] + self.regs[reg2]) & 0xFF
                    
            elif opcode == 0x11:  # STORE reg, addr
                if self.pc + 2 >= MEM_SIZE:
                    print("[ERROR] Instruction extends beyond memory")
                    break
                reg = self.memory[self.pc]
                self.pc += 1
                lo = self.memory[self.pc]
                self.pc += 1
                hi = self.memory[self.pc]
                self.pc += 1
                
                addr = (hi << 8) | lo
                
                if reg < NUM_REGS and addr < MEM_SIZE:
                    self.memory[addr] = self.regs[reg]
                
                # Memory-mapped output
                if addr == 0xFF00:
                    if reg < NUM_REGS:
                        print(chr(self.regs[reg]), end='')
                        
            else:
                print(f"[ERROR] Unknown opcode 0x{opcode:02X} at 0x{self.pc - 1:04X}")
                self.running = False

    def print_state(self):
        """Print CPU state"""
        print("Execution finished.")
        for i in range(NUM_REGS):
            print(f"r{i} = {self.regs[i]}")

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} program.mhfb")
        sys.exit(1)
    
    cpu = MighfCPU()
    entry_point = cpu.load_mhfb(sys.argv[1])
    cpu.run(entry_point)
    cpu.print_state()

if __name__ == '__main__':
    main()
