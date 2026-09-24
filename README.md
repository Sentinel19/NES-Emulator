Hopefully a cycle accurate NES Emulator some day!
Adam Dulay
Started: 8/3/24

See this site for info on 6502:
http://www.6502.org/users/obelisk/6502/index.html


See YouTube playlist: 
https://youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37&si=Jz3Vf22q87LIvL29

Instructions Implemented So Far:
  LDA
  LDX
  LDY
  STA
  STX
  STY
  TAX
  TAY
  TSX
  TXA
  TXS
  TYA
  JSR


Gemeni Output of how cycles are spent:
In 6502 microprocessor execution, clock cycles are consumed during the internal execution of an instruction and every time the processor accesses memory (to read or write).Because the 6502 is an 8-bit architecture with an unpipelined design, it requires exactly one clock cycle for every memory bus cycle it performs.
Core Drivers of Cycle Consumption
Instruction Fetch: Every instruction takes at least 1 cycle to read the opcode from memory.Operand Fetch: Reading address bytes or immediate data values takes 1 cycle per byte.
Internal Operations: Some operations require the Arithmetic Logic Unit (ALU) or internal registers to process data before completing, adding overhead cycles.Data Read/Write: Fetching a value from an address or writing a result back to RAM/registers takes 1 cycle per access.Cycles by Addressing ModeThe time spent changes dramatically based on how the instruction finds its data.Addressing ModeExampleBase CyclesWhy Cycles are SpentImplied / AccumulatorPHA, INX, LSR A21 to fetch opcode, 1 to execute internally or modify the stack.ImmediateLDA #$1021 to fetch opcode, 1 to fetch the constant byte value.Zero PageLDA $2031 to fetch opcode, 1 to fetch the 8-bit address, 1 to read the data.AbsoluteLDA $200041 to fetch opcode, 2 to fetch the 16-bit address, 1 to read the data.Zero Page, X / YLDA $20,X43 base cycles + 1 extra cycle while the ALU adds the index register.Absolute, X / YLDA $2000,X4–54 base cycles. Adds 1 cycle if the addition crosses a page boundary.Conditional Cycles (Branching)Instructions like BNE, BEQ, or BCC have variable cycle counts depending on whether the condition is met:Branch Not Taken: 2 cycles (1 to fetch opcode, 1 to fetch the relative offset). The program moves straight to the next instruction.Branch Taken: 3 cycles. The processor takes 1 extra cycle to add the offset to the Program Counter (PC).Branch Taken Across Pages: 4 cycles. If the branch forces the PC to cross into a new 256-byte page memory boundary, a second extra cycle is spent fixing the high byte of the PC.Heavy-Duty OperationsSome instructions interact deeply with the system stack or shift program control, making them highly expensive:JSR (Jump to Subroutine): 6 cycles. It must fetch the opcode, fetch the target address, and push the return address onto the stack.RTS (Return from Subroutine): 6 cycles. It spends time pulling the return address off the stack and incrementing the PC.BRK / Hardware Interrupts (IRQ/NMI): 7 cycles. The CPU pushes the PC and the status register to the stack, fetches the interrupt vector address, and updates the PC.If you are trying to optimize code for a specific project, let me know:Which specific instructions or loops you are looking to speed upIf you are trying to fit code into a strict rasterline or video blanking (VBLANK) windowWhether you are targeting a specific system like the NES, Commodore 64, or Apple II
