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

		//PC
		if(inst_mem[PC] == "") {break;}
		fp << "Value of PC is: " << PC << "\n";

		//fetch
		inst = fetch();
		fp << "Fetch:- Instruction fetched is - " << inst_mem[PC] << "\n";

		//decode
		decode(inst);
		fp << "Decode:- Instruction's mnemonic - "<< inst_type << ", ";
		if(description==1){
			fp << regs.rs1 << " = rs1 = " << regs.op1() << ", "<< regs.rs2 << " = rs2 = " << regs.op2() << "\n";
		}
		else if(description==2) {
			fp << regs.rs1 << " = rs1 = " << regs.op1() << ", imm = " << imm.output() << "\n";
		}
		else if(description==3) {
			fp << regs.rs1 << " = rs1 = " << regs.op1() << ", imm = " << imm.output() << "\n";
		}
		else if(description==4) {
			fp << regs.rs1 << " = rs1 = " << regs.op1() << ", imm = " << imm.output() << "\n";
		}
		else if(description==5) {
			fp << regs.rs1 << " = rs1 = " << regs.op1() << ", imms = " << immS.output() << "\n";
		}
		else if(description==6) {
			fp << regs.rs1 << " = rs1 = " << regs.op1() << ", " << regs.rs2 << " = rs2 = " << regs.op2() << ", immb = " << immB.output() << "\n";
		}
		else if(description==7) {
			fp << "immu = " << immU.output() << "\n";
		}
		else if(description==8) {
			fp << "immu = " << immU.output() << "\n";
		}
		else if(description==9) {
			fp << "immj = " << immJ.output() << "\n";
		}


		//execute
		execute();
		fp << "Execute:- ";
		if(description==1 || description==2){
			fp << "ALU output = " << alu.output() << "\n";
		}
		else if(description==3){
			fp << "ALU output = " << alu.output() << " This address will be sent to memory unit to load data\n";
		}
		else if(description==4) {
			fp << "ALU output = " << alu.output() << " Result of rs1 + imm\n";
		}
		else if(description==5) {
			fp << "ALU output = " << alu.output() << " This address will be sent to memory unit to store data\n";
		}
		else if(description==6) {
			if(bcu.output() == 1) {
				fp << "Branching condition is true. It will go to PC + imm.\n";
			}
			else {
				fp << "Branching condition is false. It will go to PC + 4.\n";
			}
		}
		else if(description==7) {
			fp << "Nothing changed after execution!\n";
		}
		else if(description==8) {
			fp << "Nothing changed after execution!\n";
		}
		else if(description==9) {
			fp << "Nothing changed after execution!\n";
		}


		//memory_access
		memory_access();
		fp << "Memory_access:- ";
		if(description==1 || description==2){
			fp << "Nothing changed after memory_access!\n";
		}
		else if(description==3){
			fp << "Accessing data on " << mem.address << " to load in register in next step.\n";
		}
		else if(description==4) {
			fp << "Nothing changed after memory_access!\n";
		}
		else if(description==5) {
			fp << "Storing data from " + regs.rs1 + " " << regs.regs[regs.rs1] << " to store at address " + mem.address << "\n";
		}
		else if(description==6) {
			fp << "Nothing changed after memory_access!\n";
		}
		else if(description==7) {
			fp << "Nothing changed after memory_access!\n";
		}
		else if(description==8) {
			fp << "Nothing changed after memory_access!\n";
		}
		else if(description==9) {
			fp << "Nothing changed after memory_access!\n";
		}
	

		//write_back
		write_back();
		fp << "Write_back:- ";
		if(description==1 || description==2){
			fp << "rd = "<< regs.rd << " = " << regs.regs[regs.rd] << "\n";
		}
		else if(description==3){
			fp << "rd = "<< regs.rd << " = " << regs.regs[regs.rd] << " data sent by memory was stored in former mentioned register\n";
		}
		else if(description==4) {
			fp << "rd = "<< regs.rd << " = " << regs.regs[regs.rd] << "\n";
		}
		else if(description==5) {
			fp << "Nothing changed after write_back!\n";
		}
		else if(description==6) {
			fp << "Nothing changed after write_back!\n";
		}
		else if(description==7) {
			fp << "rd = "<< regs.rd << " = " << regs.regs[regs.rd] << "\n";
		}
		else if(description==8) {
			fp << "rd = "<< regs.rd << " = " << regs.regs[regs.rd] << "\n";
		}
		else if(description==9) {
			fp << "rd = "<< regs.rd << " = " << regs.regs[regs.rd] << "\n";
		}

		//if PC is changed
		fp << "PC changed to " << mux_isbranch.output() << "\n";
		//
		fp << "\n\n";

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
