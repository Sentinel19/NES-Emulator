#include <stdio.h>
#include <stdlib.h>


// instructions

//  Load Accumulator   
#define LDA_IM 0xA9 // immediate
#define LDA_ZP 0xA5 // zero page
#define LDA_ZPX 0xB5 // zero page X
#define LDA_AB 0xAD // absolute
#define LDA_ABX 0xBD // absolute X
#define LDA_ABY 0xB9 // absolute Y
#define LDA_INX 0xA1 // indirect X
#define LDA_INY 0xB1 // indirect Y

// Load X              
#define LDX_IM 0xA2 // immediate
#define LDX_ZP 0xA6 // zero page
#define LDX_ZPY 0xB6 // zero page Y
#define LDX_AB 0xAE // absolute
#define LDX_ABY 0xBE // absolute Y

// Load Y             
#define LDY_IM 0xA0 // immediate
#define LDY_ZP 0xA4 // zero page
#define LDY_ZPX 0xB4 // zero page X
#define LDY_AB 0xAC // absolute
#define LDY_ABX 0xBC // absolute X

// Store Accumulator

// Store X

// Store Y

// Logical Shift Right    
#define LSR_AC 0x4A    // accumulator

// Jump to Subroutine
#define JSR_AB 0x20// absolute


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

	// to run ammount of cycles
	unsigned int cycles;

};

struct MEM {

	unsigned char data[MAX_MEM];

};

//CPU functions:=======================================================================================================================================================================================================
void reset_cpu(void);
void output_status(void);
void execute(void);
void LDA_set_status(void);



//Memory functions:=======================================================================================================================================================================================================

void mem_init(void);

// General byte and word fetching/reading:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

unsigned char get_byte(void);
unsigned char read_byte(unsigned short addr);
unsigned short get_word(void);
unsigned short read_word(unsigned short addr);

// fetching/reading in address modes:------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

unsigned short zero_page_addr(void);
unsigned short zero_page_X_addr(void);
unsigned short zero_page_Y_addr(void);
unsigned short absolute_addr(void);
unsigned short absolute_X_addr(void);
unsigned short absolute_Y_addr(void);

// General byte and word writing:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void write_word(unsigned short addr, unsigned char data);