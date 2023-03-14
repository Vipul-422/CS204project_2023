
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

#include "riscv.h"
#include "functions.h"
#include "components.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;



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
	ofstream fout("Registers.txt");
	string outRegs[32];
	for (int i = 0; i < 32; i++) {
		string temp = "x" + to_string(i);
		outRegs[i] = to_string(regs.regs[temp]);
		fout<<outRegs[i]<<"\n";
	}
	fout.close();

	write_data_memory();

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
	fileptr.open("student.mc", ios::in);
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
	fstream memoryfile;

	memoryfile.open("Memory.txt", ios::out);

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
