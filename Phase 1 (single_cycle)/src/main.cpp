// use the following command in src directory to compile the code :
// g++ main.cpp riscv.cpp functions.cpp components.cpp

#include "../include/riscv.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  
  //reset the processor
  reset_proc();
  //load the program memory
  load_program_memory(argv[1]);
  //run the simulator
  run_riscvsim();

  return 1;
}