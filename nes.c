#include <stdio.h>
#include <stdlib.h>
#include "nes.h"

// create instance of CPU
struct CPU cpu;
// create instance of ROM
struct MEM mem;

int main(){


	// create instance of CPU
	reset_cpu();
	

	// load example instruction into ROM
	mem.data[0] = 0xA9;
	mem.data[1] = 0xAB;
	mem.data[2] = 0x4A;

	// execute 2 clock cycles
	//unsigned int cycles = 4;
	execute(4);

	// output status
	output_status();


	return 0;
}

//CPU functions:

// reset function
void reset_cpu(){
	cpu.PC = 0x0000; // set to 0xFFFC later bc that is what 6502 actually does;
	cpu.X = 0;
	cpu.Y = 0;
	cpu.A = 0;
	cpu.SR = 0;
	cpu.SP = 0x00;

	// reset memory
	mem_init();
}



// execute function
void execute(unsigned int cycles){
	// variable to hold zero page addres for "_ZP instructions"
	unsigned char zp_addr;


	// execute as many cycles as inputted
	while(cycles > 0){

		// fetch instruction byte (fetching instruction byte always takes 1 cycle)
		unsigned char instruction = get_byte(cpu.PC);
		// increment program counter
		cpu.PC++;
		// execute one cycle
		cycles--;

		// Immediate addressing mode instructions "_IM" are 2 bytes and take 2 cycles (1: fetch opcode 2: fetch immediate value from ROM)
		// Zero Papge addressing mode instructions "_ZP" are 2 bytes and take 3 cycles (1: fetch opcode 2: fetch address in zero page 3: load value from ZP)
		switch(instruction){

			case LDA_IM:
				// load next byte into accumulator register
				cpu.A = get_byte(cpu.PC);
				// set status register flags
				LDA_set_status();
				// increment program counter
				cpu.PC++;
				// execute one cycle
				cycles--;
				break;

			case LDA_ZP:
				// in this mode, next byte after opcode is address in zero page
				zp_addr = get_byte(cpu.PC);
				cycles--;
				cpu.A = get_byte(zp_addr);
				// set status register flags
				LDA_set_status();
				cycles--;
				cpu.PC++;
				break;

			case LDX_IM:
				// load next byte into accumulator register
				cpu.X = get_byte(cpu.PC);
				// set status register flags
				LDX_set_status();
				// increment program counter
				cpu.PC++;
				// execute one cycle
				cycles--;
				break;

			case LDX_ZP:
				// in this mode, next byte after opcode is address in zero page
				zp_addr = get_byte(cpu.PC);
				cycles--;
				cpu.X = get_byte(zp_addr);
				// set status register flags
				LDX_set_status();
				cycles--;
				cpu.PC++;
				break;

			case LDY_IM:
				// load next byte into accumulator register
				cpu.Y = get_byte(cpu.PC);
				// set status register flags
				LDY_set_status();
				// increment program counter
				cpu.PC++;
				// execute one cycle
				cycles--;
				break;

			case LDY_ZP:
				// in this mode, next byte after opcode is address in zero page
				zp_addr = get_byte(cpu.PC);
				cycles--;
				cpu.Y = get_byte(zp_addr);
				// set status register flags
				LDY_set_status();
				cycles--;
				cpu.PC++;
				break;

			case LSR_AC:
				// store accumulator's 0 bit in carry flag bit
				cpu.SR |= ((cpu.A & 0x01) << 7);
				// shift contents of accumulator right by 1
				cpu.A = cpu.A >> 1;
				// set status register flags
				if(cpu.A == 0){
					cpu.SR |= 0x02;
				}
				// execute one cycle
				cycles--;
				break;




		}


	}
}

// sets the status register for all LDA type instructions
void LDA_set_status(){
	if(cpu.A == 0){ // zero flag
		cpu.SR |= 0x02;
	}
	if((cpu.A >> 7) == 1){ //negative flag
		cpu.SR |= 0x80;
	}
}

// sets the status register for all LDX type instructions
void LDX_set_status(){
	if(cpu.X == 0){ // zero flag
		cpu.SR |= 0x02;
	}
	if((cpu.X >> 7) == 1){ //negative flag
		cpu.SR |= 0x80;
	}
}

// sets the status register for all LDY type instructions
void LDY_set_status(){
	if(cpu.Y == 0){ // zero flag
		cpu.SR |= 0x02;
	}
	if((cpu.Y >> 7) == 1){ //negative flag
		cpu.SR |= 0x80;
	}
}

// display registers function
void output_status(){
	printf("6502 Simulator\n");
	printf("=====================\n");
	printf(" PC  X   Y   A   SR   SP  \n");
	printf("%04x %02x  %02x  %02x  %02x   %02x\n", cpu.PC, cpu.X, cpu.Y, cpu.A, cpu.SR, cpu.SP);
	printf("=====================\n\n");
}

//Memory functions:

// initialization function
void mem_init(){
	for(unsigned int i = 0; i < MAX_MEM; i++){
		mem.data[i] = 0;
	}
}

// retrieve byte from ROM
unsigned char get_byte(unsigned short addr){
	return mem.data[addr];
}

// retrieve word (two bytes) from ROM
// retrieves byte at address and byte at next address
// system and 6502 are little endian (watch for preserving endianness)
unsigned short get_word(unsigned short addr){

	// add functionality...
	return 0;
}