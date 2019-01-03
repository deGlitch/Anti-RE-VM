#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <String.h>
#include <string>

#define DEBUG 1

#define IMPORT_DATA_AS_STRING(str_name, symbol_name) \
    extern "C" char symbol_name ## _start; \
    extern "C" char symbol_name ## _end; \
    const std::string str_name(\
    &symbol_name ## _start, \
    &symbol_name ## _end - &symbol_name ##_start)

IMPORT_DATA_AS_STRING(bytecode, binary_output_vm_asm);

class VM {
    uint32_t rip{}; //instruction pointer
    public:
        bool exec_instr();
        uint32_t r[16]{}; //registers
        std::vector<uint8_t> memory;
};

bool VM::exec_instr(){

    

    if(DEBUG){
        printf("rip is at %x\n", rip);
        printf("executing instruction %x\n", memory[rip]);
    }

    switch(memory[rip]){
        case 0x00: // MOVI dst_reg, imm32 
        {
            printf("int value at arg2 is %u\n", memory[rip+2]);
            memcpy(&r[memory[rip+1]], &memory[rip+2], 4);
            rip+= 6;
            return false;
        }

        case 0x01: // MOV dst_reg, src_reg
        {
            r[memory[rip+1]] = r[memory[rip+2]];
            rip+=3;
            return false;
        }

        case 0x02: // AND dst_reg, src_reg
        {
            r[memory[rip+1]] &= r[memory[rip+2]];
            rip+= 3;
            return false;
        }

        case 0x03: // OR dst_reg, src_reg
        {
            r[memory[rip+1]] |= r[memory[rip+2]];
            rip+= 3;
            return false;
        }

        case 0x04: // XOR dst_reg, src_reg
        {
            r[memory[rip+1]] ^= r[memory[rip+2]];
            rip+= 3;
            return false;
        }

        case 0x05: // LSHIFT dst_reg, imm32
        {
            r[memory[rip+1]] <<= r[memory[rip+2]];
            rip+= 6;
            return false;
        }

        case 0x06: // RSHIFT dst_reg, imm32
        {
            r[memory[rip+1]] >>= r[memory[rip+2]];
            rip+= 6;
            return false;
        }

        case 0x07: // NOT dst_reg
        {
            r[memory[rip+1]] = ~r[memory[rip+1]];
            rip+= 2;
            return false;
        }

        case 0x08: // INC dst_reg
        {
            r[memory[rip+1]] += 1;
            rip+= 2;
            return false;
        }

        case 0x09: // DEC dst_reg
        {
            r[memory[rip+1]] -= 1;
            rip+= 2;
            return false;
        }

        case 0x10: //ADD dst_reg, src_reg
        {
            r[memory[rip+1]] += r[memory[rip+2]];
            rip += 3;
            return false;
        }

        case 0x11: //SUB dst_reg, src_reg
        {
            r[memory[rip+1]] -= r[memory[rip+2]];
            rip += 3;
            return false;
        }

        case 0xff: {
            return true;
        }
    }

    printf("why are you here ?");
    exit(1);
}

int main(){
    printf("Hello there general Kanobi !\n");

    VM vm;
    vm.memory.resize(4096);
    memcpy(&vm.memory[0], bytecode.data(), bytecode.size());

    bool the_end = false;
    while(!the_end){
        the_end = vm.exec_instr();
    }

    printf("The Answer is %u\n", vm.r[0]);
}