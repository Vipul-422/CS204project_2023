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

/* DON'T TOUCH ENDS */

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
  
	vector<int> inst;
	fstream fp;
	fp.open("../output/output.txt", ios::out);

	while(1) {
		
		if(!pipmemory.isEmpty){
			write_back();
		}
		if(!pipexecute.isEmpty) {
			memory_access();
			pipmemory.input_vars(pipexecute.rd, pipexecute.pc, mux_isbranch.output(), mux_resultselect.output(), pipexecute.aluout);
			pipmemory.input_controls(pipexecute.wb);
			pipmemory.isEmpty = false;
		}
		if(!pipdecode.isEmpty) {
			execute();
			pipexecute.input_vars(pipdecode.rs2, pipdecode.rd, pipdecode.OP2, pipdecode.pc, alu.output(), pipdecode.immu, pipdecode.wbadder_out);
			pipexecute.input_controls(pipdecode.m, pipdecode.wb);
			pipexecute.isEmpty = false;
		}
		if(!pipfetch.isEmpty) {
			decode();
			pipdecode.isEmpty = false;
		}
		// Fetch();
		vector<int> temp = fetch();
		pipfetch.input(temp, PC);
		pipfetch.isEmpty = false;

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
