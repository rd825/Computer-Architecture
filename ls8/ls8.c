#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argc, argv[1]); // accept filepath from command line
  cpu_run(&cpu);

  return 0;
}