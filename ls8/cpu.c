#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

// added path to handle argv
void cpu_load(struct cpu *cpu, int argc, char *path)
{
  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT,
  // };

  // TODO: Replace this with something less hard-coded

  if (argc < 2)
  {
    printf("No path provided in command line\n");
    return;
  }

  FILE *fp = fopen(path, "r"); // read from file

  if (fp == NULL)
  {
    printf("Couldn't read file: %s\n", path);
    return;
  }

  int address = 0;
  char line[1024];

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *endptr;
    unsigned char val = strtol(line, &endptr, 2);

    if (endptr == line)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, val);
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB]; // MULTIPLY regA * regB
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO

    // 1. Get the value of the current instruction (in address PC).
    // IR: Instruction Register, contains a copy of the currently executing instruction
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    unsigned char ops = IR >> 6;

    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
      // 5. Do whatever the instruction should do according to the spec.

    // Set the value of a register to an integer.
    case LDI:
      cpu->registers[operandA] = operandB;
      break;

    // Print to the console the decimal integer value that is stored in the given register.
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    // Halt the CPU (and exit the emulator).
    case HLT:
      running = 0;
      break;

    default:
      printf("Invalid Instruction: %d\n", IR);
      running = 0;
      break;
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += (ops + 1);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}
