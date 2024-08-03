#include <stdio.h>
#include <stdlib.h>


// instructions

//  Load Accumulator   
#define LDA_IM 0xA9 // immediate
#define LDA_ZP 0xA5 // zero page

// Load X              
#define LDX_IM 0xA2 // immediate
#define LDX_ZP 0xA6 // zero page

// Load Y             
#define LDY_IM 0xA0 // immediate
#define LDY_ZP 0xA4 // zero page

// Logical Shift Right    
#define LSR_AC 0x4A    // Accumulator


// CPU constants

// ROM constants
#define MAX_MEM (1024*64)

struct CPU {

	unsigned short PC; // program counter
	unsigned char A; // accumulator register
	unsigned char X; // X register
	unsigned char Y; // Y register
	unsigned char SR; // status register
	unsigned char SP; // stack pointer

};

struct MEM {

	unsigned char data[MAX_MEM];

};

// CPU functions
void reset_cpu(void);
void output_status(void);
void execute(unsigned int cycles);


// ROM functions
void mem_init(void);
unsigned char get_byte(unsigned short addr);
unsigned short get_word(unsigned short addr);