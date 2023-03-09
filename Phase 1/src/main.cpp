// To test the running use:
// gcc ./src/main.cpp ./src/riscvSimulator.cpp ./src/function.cpp ./src/components.cpp ./include/risv.h ./include/functions.h ./include/components.h

#include "../include/riscv.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  char* prog_mem_file; 
  if(argc < 2) {
    printf("Incorrect number of arguments. Please invoke the simulator \n\t./myRISCVSim <input mem file> \n");
    exit(1);
  }
  
  //reset the processor
  reset_proc();
  //load the program memory
  load_program_memory(argv[1]);
  //run the simulator
  run_riscvsim();

  return 1;
}