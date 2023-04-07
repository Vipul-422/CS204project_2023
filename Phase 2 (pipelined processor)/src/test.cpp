/* DON'T TOUCH */

#include "../include/riscv.h"
#include "../include/functions.h"
#include "../include/components.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>


extern int PC; //global PC(program counter)
extern map <int, string> inst_mem;
extern ALU alu;
extern Regfile regs;
extern Memory mem;
extern Mux mux_op2select, mux_resultselect, mux_branchTargetSel, mux_isbranch;
extern Adder adder_pc, adder_branch, adder_wb;
extern Sign_ext immB, immJ, imm, immS, immU;
extern BranchControl bcu;
extern Pipfetch pipfetch;
extern Pipdecode pipdecode;
extern Pipexecute pipexecute;
extern Pipmemory pipmemory;
extern string inst_type;
extern int description; 
extern int operation;

/* DON'T TOUCH ENDS */

extern bool is_stall;
extern map<string, int> util;

/*

description = 1 for arithmetic registers instruction
description = 2 for immediates registers instruction
description = 3 for lb, lh, lw instruction
description = 4 for jalr instruction
description = 5 for sb, sh, sw instruction
description = 6 for branching instruction
description = 7 for lui instruction
description = 8 for auipc instruction
description = 9 for jal instruction

*/

void run_riscvsim() {

	int cycle = 0;
	bool fetchrun = false;
	int endflag = 0;
	is_stall = false;
	while(1) {
		++cycle;

        if(!endflag) {
            vector<int> temp = fetch();
			pipfetch.input(temp, PC);
			pipfetch.isEmpty = false;
			PC += 4;
				cout << "fetched " << cycle << "\n";
			
			
			int flag =0;
			for(int i=31; i>=0; i--) {
				if(temp[i]!=0) {
					flag = 1;
					break;
				}
			}
			if(flag==0) {
				pipfetch.isEmpty = true;
				endflag=1;
			}
        }
		

	
	
	
	
	}

	
	
	
	
	swi_exit();

	return;
}

// it is used to set the reset values
//reset all registers and memory content to 0
void reset_proc() {

	PC = 0;
	regs.regs["x2"] = 100000;

	return;
}

//load_program_memory reads the input memory, and pupulates the instruction 
// memory
void load_program_memory(char* filename) {
	fstream fileptr;
	fileptr.open(filename, ios::in);
	int count=0; //temporary PC.
	while(!fileptr.eof()){
		string s;
		getline(fileptr, s);
		inst_mem[count]=s;
		count+=4;
	}
	fileptr.close();
	//conversion done
}

//writes the data memory in "data_out.mem" file
void write_data_memory() {

	fstream registerfile, memoryfile;

	registerfile.open("../Registers and Memory/registers.txt", ios::out);
	for(int i=0; i<32; i++) {
		string temp = "x"+to_string(i);
		registerfile << temp << "\t: " << regs.regs[temp] << "\n";
	}
	registerfile.close();

	memoryfile.open("../Registers and Memory/memory.txt", ios::out);

	for(int i=4096; i<104096; i+=4) {
		int temp = i - 4096;

		memoryfile << i << "\t: ";

		memoryfile << (int)mem.mem[temp] << " " << (int)mem.mem[temp+1] << " " << (int)mem.mem[temp+2] << " " << (int)mem.mem[temp+3] << "\n";

	}
	memoryfile.close();


	return;
}

//should be called when instruction is swi_exit
void swi_exit() {
	write_data_memory();
	exit(0);
}
