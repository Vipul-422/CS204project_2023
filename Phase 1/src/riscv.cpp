
/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of RISCV Processor

Developer's Name:
Developer's Email id:
Date: 

*/


/* riscv.cpp
   Purpose of this file: implementation file for riscv simulator
*/


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
extern string inst_type;

/* DON'T TOUCH ENDS */



void run_riscvsim() {
  
	vector<int> inst;
	fstream fp;
	fp.open("output.txt", ios::out);

	while(1) {

		if(inst_mem[PC] == "") {break;}
		fp << "Value of PC is: " << PC << "\n";
		inst = fetch();
		fp << "Fetch:- Instruction fetched is - " << inst_mem[PC] << "\n";
		decode(inst);
		fp << "Decode:- Instruction's mneomic - "<< inst_type << "\n";
		execute();
		memory_access();
		write_back();
		fp << "\n\n";

	}

	for(int i=0;i<41;i+=4){
		cout<<(int)mem.mem[i]<<" "<<(int)mem.mem[i+1]<<" "<<(int)mem.mem[i+2]<<" "<<(int)mem.mem[i+3]<<"\n";
	}
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
void load_program_memory() {
	//converting user .mc file to input.txt file as per our instruction memory update part needs.
	// string file_name="../.mcfile/";
	// int j=0;
	// while(_file_name[j]!='\0'){
	//   file_name.push_back(_file_name[j]);
	//   j++;
	// }
	fstream fileptr, filewrite;
	fileptr.open("../../.mc files/input.mc", ios::in);
	filewrite.open("input.txt", ios::out);
	int count=0; //temporary PC.
	while(!fileptr.eof()){
		string s;
		getline(fileptr, s);
		inst_mem[count]=s;
		s="0x"+s;
		ostringstream ss;
		ss<<hex<<count;
		s="0x"+ss.str()+" "+s;
		filewrite<<s;
		filewrite<<"\n";
		count+=4;
	}
	//conversion done
}

//writes the data memory in "data_out.mem" file
void write_data_memory() {
	return;
}

//should be called when instruction is swi_exit
void swi_exit() {
	write_data_memory();
	exit(0);
}
