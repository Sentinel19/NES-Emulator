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
	mem.data[0] = 0xA0;
	mem.data[1] = 0x01;
	mem.data[2] = 0xB6;
	mem.data[3] = 0xBB;
	mem.data[4] = 0x00;
	mem.data[0xBC] = 0xAD;



	




	cpu.cycles = 6;
	execute();

	// output status
	output_status();



	return 0;
}

//CPU functions:=======================================================================================================================================================================================================

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
		unsigned char instruction = get_byte();



		switch(instruction){

		// LDA Variants:
			case LDA_IM: // 2 cycles
				// load next byte into accumulator register
				cpu.A = get_byte();
				// set status register flags
				LD_set_status((cpu.A));
				break;

			case LDA_ZP: // 3 cycles
				// in this mode, next byte after opcode is address in zero page
				zp_addr = zero_page_addr();
				cpu.A = read_byte(zp_addr);
				// set status register flags
				LD_set_status((cpu.A));
				break;

			case LDA_ZPX: // 4 cycles
				zp_addr = zero_page_X_addr();
				// handle "wrap around" if overflow (does not touch lower 8 bits so if no overflow its fine still I believe)
				zp_addr &= 0x00FF;
				// we dont want to increment the program counter again here (we already did in the "get_byte()" above)
				cpu.A = read_byte(zp_addr);
				// set status register flags
				LD_set_status((cpu.A));
				break;

			case LDA_AB: // 4 cycles			
				abs_addr = absolute_addr();
				cpu.A = read_byte(abs_addr);
				// set status register flags
				LD_set_status((cpu.A));	
				break;

			case LDA_ABX: // 4 cycles (5 if page boundary is crossed...)
				abs_addr = absolute_X_addr();
				cpu.A = read_byte(abs_addr);
				// set status register flags
				LD_set_status((cpu.A));	
				break;

			case LDA_ABY: // 4 cycles (5 if page boundary is crossed...)
				abs_addr = absolute_Y_addr();
				cpu.A = read_byte(abs_addr);
				// set status register flags
				LD_set_status((cpu.A));	
				break;

			case LDA_INX: // 6 cycles
				zp_addr = zero_page_addr();
				zp_addr += cpu.X;
				cpu.cycles--;
				eff_addr = read_word(zp_addr);
				cpu.A = read_byte(eff_addr);
				// set status register flags
				LD_set_status((cpu.A));					
				break;

			case LDA_INY: // 5 cycles (6 if page boundary is crossed...)
				zp_addr = zero_page_addr();
				eff_addr = read_word(zp_addr);
				cpu.A = read_byte(eff_addr + (cpu.Y));
				// check if page boundary was crossed
				if((eff_addr + (cpu.Y)) - eff_addr >= 0xFF){
					cpu.cycles--;
				}
				// set status register flags
				LD_set_status((cpu.A));
				break;

		// LDX Variants:	
			case LDX_IM: // 2 cycles
				// load next byte into accumulator register
				cpu.X = get_byte();
				// set status register flags
				LD_set_status((cpu.X));
				break;

			case LDX_ZP: // 3 cycles
				// in this mode, next byte after opcode is address in zero page
				zp_addr = zero_page_addr();
				cpu.X = read_byte(zp_addr);
				// set status register flags
				LD_set_status((cpu.X));
				break;

			case LDX_ZPY: // 4 cycles
				zp_addr = zero_page_Y_addr();
				// handle "wrap around" if overflow (does not touch lower 8 bits so if no overflow its fine still I believe)
				zp_addr &= 0x00FF;
				// we dont want to increment the program counter again here (we already did in the "get_byte()" above)
				cpu.X = read_byte(zp_addr);
				// set status register flags
				LD_set_status((cpu.X));
				break;

			case LDX_AB: // 4 cycles
				abs_addr = absolute_addr();
				cpu.X = read_byte(abs_addr);
				// set status register flags
				LD_set_status((cpu.X));	
				break;

			case LDX_ABY: // 4 cycles (5 if page boundary is crossed...)
				abs_addr = absolute_Y_addr();
				cpu.X = read_byte(abs_addr);
				// set status register flags
				LD_set_status((cpu.X));	
				break;								

		// LDY Variants:
			case LDY_IM: // 2 cycles
				// load next byte into accumulator register
				cpu.Y = get_byte();
				// set status register flags
				LD_set_status((cpu.Y));
				// execute one cycle
				break;

			case LDY_ZP: // 3 cycles
				// in this mode, next byte after opcode is address in zero page
				zp_addr = zero_page_addr();
				cpu.Y= read_byte(zp_addr);
				// set status register flags
				LD_set_status((cpu.Y));
				break;

			case LDY_ZPX: // 4 cycles
				zp_addr = zero_page_X_addr();
				// handle "wrap around" if overflow (does not touch lower 8 bits so if no overflow its fine still I believe)
				zp_addr &= 0xFF;
				// we dont want to increment the program counter again here (we already did in the "get_byte()" above)
				cpu.Y = read_byte(zp_addr);
				// set status register flags
				LD_set_status((cpu.Y));
				break;

			case LDY_AB: // 4 cycles
				abs_addr = absolute_addr();
				cpu.Y = read_byte(abs_addr);
				// set status register flags
				LD_set_status((cpu.Y));	
				break;		

			case LDY_ABX: // 4 cycles (5 if page boundary is crossed...)
				abs_addr = absolute_X_addr();
				cpu.Y = read_byte(abs_addr);
				// set status register flags
				LD_set_status((cpu.Y));	
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

		// Jump Instructions:
			case JSR_AB: // 6 cycles (uses Absolute addressing mode)
				abs_addr = absolute_addr();
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

// sets the status register for all load ("LD...") instructions
void LD_set_status(unsigned char reg){
	if(reg == 0){ // zero flag
		cpu.SR |= 0x02;
	}
	if((reg >> 7) == 1){ //negative flag
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

//Memory functions:=======================================================================================================================================================================================================

// initialization function
void mem_init(){
	for(unsigned int i = 0; i < MAX_MEM; i++){
		mem.data[i] = 0;
	}
}

// General byte and word fetching/reading:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// retrieve byte from memory (increments PC)
unsigned char get_byte(){

	unsigned char byte = mem.data[cpu.PC];
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
unsigned short get_word(){
	

	// 6502 and this VM are little endian (so first read byte is on LSB side)
	// fetch lower byte
	unsigned short word = mem.data[cpu.PC];
	cpu.PC++;
	cpu.cycles--;
	// fetch higher byte
	word |= (mem.data[cpu.PC] << 8);
	cpu.PC++;
	cpu.cycles--;
	return word;

}

// reads word from zero page (does not "execute code" which means it does not increment the program counter)
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


// fetching/reading in address modes:------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// done because many instructions use zero page addressing mode
unsigned short zero_page_addr(){
	return get_byte();
}

// done because many instructions use zerp page with X offset addressing mode
unsigned short zero_page_X_addr(){
	// in this mode, accumulator = memory[zeroPageAddress + X]
	unsigned char zp_addr = get_byte();
	// add contents of X register to value from zero page
	zp_addr += cpu.X;
	cpu.cycles--;
	return zp_addr;
}


// done because many instructions use zerp page with Y offset addressing mode
unsigned short zero_page_Y_addr(){
	// in this mode, accumulator = memory[zeroPageAddress + X]
	unsigned char zp_addr = get_byte();
	// add contents of X register to value from zero page
	zp_addr += cpu.Y;
	cpu.cycles--;
	return zp_addr;
}

// done because many instructions use absolute addressing mode
unsigned short absolute_addr(){
	return get_word();
}

// done because many instructions use absolute with X offset addressing mode
unsigned short absolute_X_addr(){
	unsigned short abs_addr = get_word();
	// check if page boundary was crossed
	if((abs_addr + (cpu.X)) - abs_addr >= 0xFF){
		cpu.cycles--;
	}
	return (abs_addr + (cpu.X));
}

// done because many instructions use absolute with Y offset addressing mode
unsigned short absolute_Y_addr(){
	unsigned short abs_addr = get_word();
	// check if page boundary was crossed
	if((abs_addr + (cpu.Y)) - abs_addr >= 0xFF){
		cpu.cycles--;
	}
	return (abs_addr + (cpu.Y));
}



// General byte and word writing:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// simply writes a word to memory
void write_word(unsigned short addr, unsigned char data){
	// remember this is little endian...
	mem.data[addr] = data & 0xFF;
	cpu.cycles--;
	mem.data[addr + 1] = (data >> 8);
	cpu.cycles--;
}
