# Chip 8 description

## 16 8-bit register

_registers labeled from V0 to VF.

VF is a flag to hold information about the result of operations.

## 4kB of RAM

Memory space is 4096 bytes of memory (0 - 4095; 0x000 - 0xFFF)

Normally divided in:

* 0x000-0x1FF: Originally reserved for CHIP8 interpreter. 
  * 0x050-0x0A0: Stored space for 16 built in chars (0-9, A-F).
  * 0x200-0xFFF: Instructions from ROM. Any space left can be used.
    * 0xEA0-0xEFF: call stack, variables and internal use.
    * 0xF00-0xFFF: Display refresh

## 16-bit index register

Used to store memory addresses for use in operations.

## 16-bit Program Counter (PC)

The program instructions are stored in memory starting address 0x200.
The PC is a special register to hold the address of the next instruction to execute.

Note: The PC should be incremented before execution of the instruction (2 bytes) because some opcodes manipulates it.

## 16-level 16-bit Stack

Way for CPU to keep track of program execution order.

When an instruction CALL is called, PC + 2 (next instruction) should be pushed to _stack, then jump.

Then the instruction RET is called, PC should point to the popped address from the _stack.

## 8-bit Stack Pointer

Normally, the people implement the _stack as an array, saying that, we need a way to point to the "top of the _stack". 
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

## Instructions
| Opcode |   Type  |                            C Pseudo                            |                                                                                                                                                                                        Explanation                                                                                                                                                                                        |
|:------:|:-------:|:--------------------------------------------------------------:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
|  0NNN  | Call    |                                                                |                                                                                                                                             Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN. Not necessary for most ROMs.                                                                                                                                             |
|  00E0  | Display |                          disp_clear()                          |                                                                                                                                                                                    Clears the screen.                                                                                                                                                                                     |
|  00EE  | Flow    |                            return;                             |                                                                                                                                                                                Returns from a subroutine.                                                                                                                                                                                 |
|  1NNN  | Flow    |                           goto NNN;                            |                                                                                                                                                                                   Jumps to address NNN.                                                                                                                                                                                   |
|  2NNN  | Flow    |                           *(0xNNN)()                           |                                                                                                                                                                                 Calls subroutine at NNN.                                                                                                                                                                                  |
|  3XNN  | Cond    |                         if (Vx == NN)                          |                                                                                                                                        Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block);                                                                                                                                         |
|  4XNN  | Cond    |                         if (Vx != NN)                          |                                                                                                                                    Skips the next instruction if VX does not equal NN. (Usually the next instruction is a jump to skip a code block);                                                                                                                                     |
|  5XY0  | Cond    |                         if (Vx == Vy)                          |                                                                                                                                        Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block);                                                                                                                                         |
|  6XNN  | Const   |                            Vx = NN                             |                                                                                                                                                                                      Sets VX to NN.                                                                                                                                                                                       |
|  7XNN  | Const   |                            Vx += NN                            |                                                                                                                                                                        Adds NN to VX. (Carry flag is not changed);                                                                                                                                                                        |
|  8XY0  | Assig   |                            Vx = Vy                             |                                                                                                                                                                                Sets VX to the value of VY.                                                                                                                                                                                |
|  8XY1  | BitOp   |                            Vx \= Vy                            |                                                                                                                                                                       Sets VX to VX or VY. (Bitwise OR operation);                                                                                                                                                                        |                                                                                                                                                                                                                                                                                                                                              |
|  8XY2  | BitOp   |                            Vx &= Vy                            |                                                                                                                                                                      Sets VX to VX and VY. (Bitwise AND operation);                                                                                                                                                                       |
|  8XY3  | BitOp   |                            Vx ^= Vy                            |                                                                                                                                                                                   Sets VX to VX xor VY.                                                                                                                                                                                   |
|  8XY4  | Math    |                            Vx += Vy                            |                                                                                                                                                      Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not.                                                                                                                                                      |
|  8XY5  | Math    |                            Vx -= Vy                            |                                                                                                                                                 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not.                                                                                                                                                  |
|  8XY6  | BitOp   |                            Vx >>= 1                            |                                                                                                                                                    Stores the least significant bit of VX in VF and then shifts VX to the right by 1.                                                                                                                                                     |
|  8XY7  | Math    |                          Vx = Vy - Vx                          |                                                                                                                                                  Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.                                                                                                                                                   |
|  8XYE  | BitOp   |                            Vx <<= 1                            |                                                                                                                                                     Stores the most significant bit of VX in VF and then shifts VX to the left by 1.                                                                                                                                                      |
|  9XY0  | Cond    |                         if (Vx != Vy)                          |                                                                                                                                    Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block);                                                                                                                                     |
|  ANNN  | MEM     |                            I = NNN                             |                                                                                                                                                                                Sets I to the address NNN.                                                                                                                                                                                 |
|  BNNN  | Flow    |                         PC = V0 + NNN                          |                                                                                                                                                                             Jumps to the address NNN plus V0.                                                                                                                                                                             |
|  CXNN  | Rand    |                        Vx = rand() & NN                        |                                                                                                                                             Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.                                                                                                                                             |
|  DXYN  | Display |                        draw(Vx, Vy, N)                         | Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value does not change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen |
|  EX9E  | KeyOp   |                        if (key() == Vx)                        |                                                                                                                               Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block);                                                                                                                               |
|  EXA1  | KeyOp   |                        if (key() != Vx)                        |                                                                                                                             Skips the next instruction if the key stored in VX is not pressed. (Usually the next instruction is a jump to skip a code block);                                                                                                                             |
|  FX07  | Timer   |                        Vx = get_delay()                        |                                                                                                                                                                         Sets VX to the value of the _delay timer.                                                                                                                                                                         |
|  FX0A  | KeyOp   |                         Vx = get_key()                         |                                                                                                                                     A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event);                                                                                                                                     |
|  FX15  | Timer   |                        delay_timer(Vx)                         |                                                                                                                                                                               Sets the _delay timer to VX.                                                                                                                                                                                |
|  FX18  | Sound   |                        sound_timer(Vx)                         |                                                                                                                                                                               Sets the _sound timer to VX.                                                                                                                                                                                |
|  FX1E  | MEM     |                            I += Vx                             |                                                                                                                                                                             Adds VX to I. VF is not affected.                                                                                                                                                                             |
|  FX29  | MEM     |                      I = sprite_addr[Vx]                       |                                                                                                                               Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.                                                                                                                                |
|  FX33  | BCD     | set_BCD(Vx) *(I+0) = BCD(3); *(I+1) = BCD(2); *(I+2) = BCD(1); |    Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.);     |
|  FX55  | MEM     |                        reg_dump(Vx, &I)                        |                                                                                                            Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.                                                                                                             |
|  FX65  | MEM     |                        reg_load(Vx, &I)                        |                                                                                                       Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value read, but I itself is left unmodified.                                                                                                        |

## Sources:

* https://en.wikipedia.org/wiki/CHIP-8
* https://austinmorlan.com/posts/chip8_emulator/
* https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
* https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#dxyn-display