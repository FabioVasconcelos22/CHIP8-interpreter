# Chip 8 description

## 16 8-bit register

registers labeled from V0 to VF.

VF is a flag to hold information about the result of operations.

## 4kB of RAM

Memory space is 4096 bytes of memory (0 - 4095; 0x000 - 0xFFF)

Normally divided in:

* 0x000-0x1FF: Originally reserved for CHIP8 interpreter. 
  * 0x050-0x0A0: Stored space for 16 built in chars (0-9, A-F).
  * 0x200-0xFFF: Instructions from ROM. Any space left can be used.

## 16-bit index register

Used to store memory addresses for use in operations.

## 16-bit Program Counter (PC)

The program instructions are stored in memory starting address 0x200.
The PC is a special register to hold the address of the next instruction to execute.

Note: The PC should be incremented before execution of the instruction (2 bytes) because some opcodes manipulates it.

## 16-level 16-bit Stack

Way for CPU to keep track of program execution order.

When an instruction CALL is called, PC + 2 (next instruction) should be pushed to stack, then jump.

Then the instruction RET is called, PC should point to the popped address from the stack.

## 8-bit Stack Pointer

Normally, the people implement the stack as an array, saying that, we need a way to point to the "top of the stack". 
Maybe we will not need as a C++ exercise.

## 8-bit Delay Timer

Should decrease at 60Hz. As mention on literature, 60Hz is a good cycle clock for your code. So can be used.

## 8-bit Sound Timer

Similar with Delay Timer. It buzzes when non-zero.

## 8-bit 16 Input Keys

From 0 to F.

## 64x32 Monochrome Display

Memory buffer with 2048 bytes (63x32).

The output works as XOR between the sprite and the pixel display.

## Sources:

* https://austinmorlan.com/posts/chip8_emulator/
* https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/