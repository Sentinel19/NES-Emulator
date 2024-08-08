#include <stdio.h>
#include <stdlib.h>


// instructions

//  Load Accumulator   
#define LDA_IM 	0xA9 // immediate
#define LDA_ZP 	0xA5 // zero page
#define LDA_ZPX 0xB5 // zero page X
#define LDA_AB 	0xAD // absolute
#define LDA_ABX 0xBD // absolute X
#define LDA_ABY 0xB9 // absolute Y
#define LDA_INX 0xA1 // indirect X
#define LDA_INY 0xB1 // indirect Y

// Load X              
#define LDX_IM	0xA2 // immediate
#define LDX_ZP 	0xA6 // zero page
#define LDX_ZPY 0xB6 // zero page Y
#define LDX_AB	0xAE // absolute
#define LDX_ABY 0xBE // absolute Y

// Load Y             
#define LDY_IM 	0xA0 // immediate
#define LDY_ZP 	0xA4 // zero page
#define LDY_ZPX 0xB4 // zero page X
#define LDY_AB 	0xAC // absolute
#define LDY_ABX 0xBC // absolute X

// Store Accumulator
#define STA_ZP	0x85 // zero page
#define STA_ZPX 0x95 // zero page X
#define STA_AB	0x8D // absolute
#define STA_ABX 0x9D // absolute X
#define STA_ABY 0x99 // absolute Y
#define STA_INX 0x81 // indirect X
#define STA_INY 0x91 // indirect Y


// Store X
#define STX_ZP 	0x86 // zero page
#define STX_ZPY 0x96 // zero page Y
#define STX_AB 	0x8E // absolute



// Store Y
#define STY_ZP 	0x84 // zero page
#define STY_ZPX 0x94 // zero page X
#define STY_AB 	0x8C // absolute


// Register Transfer
#define TAX 	0xAA // implied
#define TAY 	0xA8 // implied
#define TSX 	0xBA // implied
#define TXA 	0x8A // implied
#define TXS 	0x9A // implied
#define TYA 	0x98 // implied

// Logical Shift Right    
#define LSR_AC 	0x4A    // accumulator

// Logical AND
#define AND_IM	0x29 // immediate
#define AND_ZP	0x25 // zero page
#define AND_ZPX	0x35 // zero page X
#define AND_AB	0x2D // absolute
#define AND_ABX	0x3D // absolute X
#define AND_ABY	0x39 // absolute Y
#define AND_INX	0x21 // indirect X
#define AND_INY	0x31 // indirect Y

// Logiacal Exlusive OR
#define EOR_IM	0x49 // immediate
#define EOR_ZP	0x45 // zero page
#define EOR_ZPX	0x55 // zero page X
#define EOR_AB	0x4D // absolute
#define EOR_ABX	0x5D // absolute X
#define EOR_ABY	0x59 // absolute Y
#define EOR_INX	0x41 // indirect X
#define EOR_INY	0x51 // indirect Y

// Logical Inclusive OR
#define ORA_IM	0x09 // immediate
#define ORA_ZP	0x05 // zero page
#define ORA_ZPX	0x15 // zero page X
#define ORA_AB	0x0D // absolute
#define ORA_ABX	0x1D // absolute X
#define ORA_ABY	0x19 // absolute Y
#define ORA_INX	0x01 // indirect X
#define ORA_INY	0x11 // indirect Y

// Logical Bit Test
#define BIT_ZP	0x24 // zero page
#define BIT_AB	0x2C // absolute

// Return from Subroutine
#define RTS 	0x60 // implied

// Jump to Subroutine
#define JSR 	0x20 // absolute

// Status Flag Changes
#define	CLC		0x18 // implied
#define CLD		0xD8 // implied
#define CLI		0x58 // implied
#define CLV		0xB8 // implied
#define SEC		0x38 // implied
#define SED		0xF8 // implied
#define SEI		0x78 // implied



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
void output_zero_page(void);
void output_page(unsigned int offset);

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

// Stack Operations:------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void push_word_to_stack(unsigned short data);
unsigned short pop_word_from_stack(void);

// General byte and word writing:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void write_byte(unsigned short addr, unsigned char data);
void write_word(unsigned short addr, unsigned char data);