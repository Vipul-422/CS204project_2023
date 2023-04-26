// use the following command in src directory to compile the code :
// g++ main.cpp riscv.cpp functions.cpp components.cpp

#include "../include/riscv.h"
#include "../include/components.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern Cache inst_cache, cache;

int main(int argc, char** argv) {

	srand(time(NULL));

	int isPipelined;
	std::cout << "Enter 0 for single cycle\n";
	std::cout << "Enter 1 for pipelined\n";
	std::cin >> isPipelined;

	
	
	
	inst_cache.initialise(64, 8, "SA", "Random", 2);


	cache.initialise(64, 8, "FA", "Random", 1);
	
  	//reset the processor
	reset_proc();
	//load the program memory
	load_program_memory(argv[1]);
	//run the simulator
	run_riscvsim();

	return 1;
}



	//Instruction cache user interactions
	// cout<<"For Instruction cache please follow these instruction\n";
	// cout<<"Type FA for Fully Associative mapping\nType DM for Direct mapping\nType SA for Set Associative mapping\n(case sensitive)\n";
	// //cache type is case sensitive
	// cin >> inst_cache.type;
	// cout<<"Give size of Cache in Bytes\n";
	// cin >> inst_cache.cache_size;
	// cout<<"Give size of Block in Bytes\n";
	// cin >> inst_cache.block_size;
	// if(inst_cache.type == "SA") {
	// 	cout<<"Give number of ways for set associative mapping\n";
	// 	cin >> inst_cache.sa_ways;
	// }
	// if(inst_cache.type == "SA" || inst_cache.type == "FA") {
	// 	cout<<"Give the policy for set associative mapping and fully associative mapping\nFIFO/LRU/Random/LFU all are case sensitive\n";
	// 	cin >> inst_cache.policy;
	// }














	//Data cache user interactions
	// cout<<"For Data cache please follow these instruction\n";
	// cout<<"Type FA for Fully Associative mapping\nType DM for Direct mapping\nType SA for Set Associative mapping\n";
	// //cache type is case sensitive
	// cin >> cache.type;
	// cout<<"Give size of Cache in Bytes\n";
	// cin >> cache.cache_size;
	// cout<<"Give size of Block in Bytes\n";
	// cin >> cache.block_size;
	// if(cache.type == "SA") {
	// 	cout<<"Give number of ways for set associative mapping\n";
	// 	cin >> cache.sa_ways;
	// }
	// if(cache.type == "SA" || cache.type == "FA") {
	// 	cout<<"Give the policy for set associative mapping and fully associative mapping\nFIFO/LRU/Random/LFU all are case sensitive\n";
	// 	cin >> cache.policy;
	// }
	// cache.initialise(cache.cache_size, cache.block_size, cache.type, cache.policy, cache.sa_ways);