// use the following command in src directory to compile the code :
// g++ main.cpp riscv.cpp functions.cpp components.cpp

#include "../include/riscv.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {

	srand(time(NULL));

	int isPipelined;
	std::cout << "Enter 0 for single cycle\n";
	std::cout << "Enter 1 for pipelined\n";
	std::cin >> isPipelined;

  
  	//reset the processor
	reset_proc();
	//load the program memory
	load_program_memory(argv[1]);
	//run the simulator
	run_riscvsim();

	return 1;
}