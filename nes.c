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
	mem.data[0] = 0xAD;
	mem.data[1] = 0xBB;
	mem.data[2] = 0xBB;
	mem.data[0xBBBB] = 0xAD;


	



	// execute 2 clock cycles
	//unsigned int cycles = 4;
	cpu.cycles = 4;
	execute();

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
	cpu.SP = 0xFF;

	// reset memory
	mem_init();
}



// execute function
void execute(){
	// variable to hold zero page addres for "_ZP", "_ZPX" instructions
	unsigned char zp_addr;

	// variable to hold absolute address for absolute addressing instructions
	unsigned short abs_addr;

	// variable to hold effective address for indirect addressing instructions
	unsigned short eff_addr;

	// execute as many cycles as inputted
	while(cpu.cycles > 0){

		// fetch instruction byte (fetching instruction byte always takes 1 cycle)
		unsigned char instruction = get_byte(cpu.PC);



		switch(instruction){

			case LDA_IM: // 2 cycles
				// load next byte into accumulator register
				cpu.A = get_byte(cpu.PC);
				// set status register flags
				LDA_set_status();
				break;

			case LDA_ZP: // 3 cycles
				// in this mode, next byte after opcode is address in zero page
				zp_addr = read_byte(cpu.PC);
				cpu.A = get_byte(zp_addr);
				// set status register flags
				LDA_set_status();
				break;

			case LDA_ZPX: // 4 cycles
				// in this mode, accumulator = memory[zeroPageAddress + X]
				zp_addr = get_byte(cpu.PC);
				// add contents of X register to value from zero page
				zp_addr += cpu.X;
				// handle "wrap around" if overflow (does not touch lower 8 bits so if no overflow its fine still I believe)
				zp_addr &= 0xFF;
				cpu.cycles--;
				// we dont want to increment the program counter again here (we already did in the "get_byte()" above)
				cpu.A = read_byte(zp_addr);
				// set status register flags
				LDA_set_status();
				break;

			case LDA_AB: // 4 cycles			
				abs_addr = get_word(cpu.PC);
				cpu.A = read_byte(abs_addr);
				// set status register flags
				LDA_set_status();	
				break;

			case LDA_ABX: // 4 cycles (5 if page boundary is crossed...)
				abs_addr = get_word(cpu.PC);
				cpu.A = read_byte(abs_addr + (cpu.X));
				// check if page boundary was crossed
				if((abs_addr + (cpu.X)) - abs_addr >= 0xFF){
					cpu.cycles--;
				}
				// set status register flags
				LDA_set_status();	
				break;

			case LDA_ABY: // 4 cycles (5 if page boundary is crossed...)
				abs_addr = get_word(cpu.PC);
				cpu.A = read_byte(abs_addr + (cpu.Y));
				// check if page boundary was crossed
				if((abs_addr + (cpu.Y)) - abs_addr >= 0xFF){
					cpu.cycles--;
				}
				// set status register flags
				LDA_set_status();	
				break;

			case LDA_INX: // 6 cycles
				zp_addr = get_byte(cpu.PC);
				zp_addr += cpu.X;
				cpu.cycles--;
				eff_addr = read_word(zp_addr);
				cpu.A = read_byte(eff_addr);
				// check if page boundary was crossed
				if((eff_addr + (cpu.Y)) - eff_addr >= 0xFF){
					cpu.cycles--;
				}
				// set status register flags
				LDA_set_status();					
				break;

			case LDA_INY: // 5 cycles (6 if page boundary is crossed...)
				zp_addr = get_byte(cpu.PC);
				eff_addr = read_word(zp_addr);
				cpu.A = read_byte(eff_addr + (cpu.Y));
				// set status register flags
				LDA_set_status();
				break;

			case LDX_IM:
				// load next byte into accumulator register
				cpu.X = get_byte(cpu.PC);
				// set status register flags
				LDX_set_status();
				break;

			case LDX_ZP:
				// in this mode, next byte after opcode is address in zero page
				zp_addr = read_byte(cpu.PC);
				cpu.X = get_byte(zp_addr);
				// set status register flags
				LDX_set_status();
				break;

			case LDX_ZPY:
				// in this mode, accumulator = memory[zeroPageAddress + X]
				zp_addr = get_byte(cpu.PC);
				// add contents of X register to value from zero page
				zp_addr += cpu.Y;
				// handle "wrap around" if overflow (does not touch lower 8 bits so if no overflow its fine still I believe)
				zp_addr &= 0xFF;
				cpu.cycles--;
				// we dont want to increment the program counter again here (we already did in the "get_byte()" above)
				cpu.X = read_byte(zp_addr);
				// set status register flags
				LDX_set_status();
				break;

			case LDX_AB:				
				break;

			case LDY_IM:
				// load next byte into accumulator register
				cpu.Y = get_byte(cpu.PC);
				// set status register flags
				LDY_set_status();
				// execute one cycle
				break;

			case LDY_ZP:
				// in this mode, next byte after opcode is address in zero page
				zp_addr = read_byte(cpu.PC);
				cpu.Y= get_byte(zp_addr);
				// set status register flags
				LDY_set_status();
				break;

			case LDY_ZPX:
				// in this mode, accumulator = memory[zeroPageAddress + X]
				zp_addr = get_byte(cpu.PC);
				// add contents of X register to value from zero page
				zp_addr += cpu.X;
				// handle "wrap around" if overflow (does not touch lower 8 bits so if no overflow its fine still I believe)
				zp_addr &= 0xFF;
				cpu.cycles--;
				// we dont want to increment the program counter again here (we already did in the "get_byte()" above)
				cpu.Y = read_byte(zp_addr);
				// set status register flags
				LDY_set_status();
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
				cpu.cycles--;
				// increment program counter
				cpu.PC++;
				break;

			case JSR_AB: // (uses Absolute addressing mode)
				abs_addr = get_word(cpu.PC);
				// push address (-1) of the return point onto stack
				write_word( 0x0100 | (cpu.SP), (cpu.PC - 1));
				// increment stack pointer
				cpu.SP--;
				// update program counter with subroutine address (what is being jumped to...)
				cpu.PC = abs_addr;
				cpu.cycles--;
				break;

			default:
				printf("Instruction: $%hhx not handled yet\n\n", instruction);




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

// retrieve byte from memory (increments PC)
unsigned char get_byte(unsigned short addr){

	unsigned char byte = mem.data[addr];
	cpu.cycles--;
	cpu.PC++;
	return byte;
}

// reading byte from zero page (not the same as above, does not "execute code" which means PC does not get incremented)
unsigned char read_byte(unsigned short addr){
	unsigned char byte = mem.data[addr];
	cpu.cycles--;
	return byte;
}

// retrieve word (two bytes) from memory (increments PC)
// retrieves byte at address and byte at next address
// system and 6502 are little endian (watch for preserving endianness)
unsigned short get_word(unsigned short addr){
	

	// 6502 and this VM are little endian (so first read byte is on LSB side)
	// fetch lower byte
	unsigned short word = mem.data[addr];
	cpu.PC++;
	cpu.cycles--;
	// fetch higher byte
	word |= (mem.data[addr + 1] << 8);
	cpu.PC++;
	cpu.cycles--;
	return word;

}

unsigned short read_word(unsigned short addr){

	// 6502 and this VM are little endian (so first read byte is on LSB side)
	// fetch lower byte
	unsigned short word = mem.data[addr];
	cpu.cycles--;
	// fetch higher byte
	word |= (mem.data[addr + 1] << 8);
	cpu.cycles--;
	return word;
}

void write_word(unsigned short addr, unsigned char data){
	// remember this is little endian...
	mem.data[addr] = data & 0xFF;
	cpu.cycles--;
	mem.data[addr + 1] = (data >> 8);
	cpu.cycles--;
}

