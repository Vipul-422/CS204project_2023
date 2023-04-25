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
extern bool branchjump_stall;
extern map<string, string> util;
extern map<string, int> utilint;
map <int, int> branch_pred;
int forwarding;
int temp_pc;

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
using namespace std;

void run_riscvsim() {
	
	int cycle = 0;
	int stalls = 0;
	int instructions = 0;
	int branc_mispreds = 0;
	int dhstalls = 0;      // stalls due to data hazards
	int chstalls = 0;      // stalls due to control hazards
	
	bool branch_prediction = false;
	cout<<"Press 1 for forwarding\nPress 0 for without forwarding\n";
	cin >> forwarding;
	cout<<"Press 1 for branch prediction\nPress 0 for without branch prediction\n";
	cin >> branch_prediction;

	FILE* fp1 = freopen("output.txt", "w", stdout);
	int endflag = 0;
	branchjump_stall = false;
	is_stall = false;
	// freopen("output.txt", "w", stdout);
	if(forwarding == 1){
		while(1) {
			++cycle;
			is_stall = false;
			branchjump_stall = false;
			
			if(!pipmemory.isEmpty){
				write_back();
				instructions++;

				cout << "WB pc: " << pipmemory.pc;
						
				
				cout << " in writeback " << "\n";
				if(pipexecute.isEmpty && pipdecode.isEmpty && pipfetch.isEmpty && endflag) {
					pipmemory.isEmpty = true;
					break;
				}
				else if (pipexecute.isEmpty&&!pipdecode.isEmpty) {
					pipmemory.isEmpty = true;
				}
			}
			if(!pipexecute.isEmpty) {
				utilint["pipmemrd"] = pipmemory.resultselectmux_out;
				// cout<<pipmemory.RD<<"\n";
				// cout<<"addrress: "<<pipexecute.aluout<<"\n";
				memory_access();
				util["pipmemrd"] = pipmemory.rd;
				utilint["pipmemisempty"] = pipmemory.isEmpty;
				pipmemory.input_vars(pipexecute.rd, pipexecute.pc, mux_isbranch.output(), mux_resultselect.output(), pipexecute.aluout, mem.output());
				pipmemory.input_controls(pipexecute.wb);
				pipmemory.isEmpty = false;
				
				
				cout << "MEM pc: " << pipexecute.pc;
				
				if(pipdecode.isEmpty && pipfetch.isEmpty && endflag)
					pipexecute.isEmpty = true;
				cout << " in memory access " << "\n";
			}
			if(!pipdecode.isEmpty && !is_stall) {
				execute();
				if(is_stall) {
					stalls++;
					dhstalls++;
					pipexecute.isEmpty = true;
					pipdecode.isEmpty = true;
					cout << "Stall at " << "pc =" << pipdecode.pc << "\n\n";
					PC = pipdecode.pc;
					vector<int> temp = fetch();
					PC = pipfetch.pc;
					pipfetch.input(temp, pipdecode.pc);
					pipfetch.isEmpty = false;
					if(endflag==1)   endflag =0;
					continue;
				}
				if(branchjump_stall) {
					// cout << "mux is branch: " << mux_isbranch.output() << endl;
					if(pipfetch.pc != mux_isbranch.output()) {
						stalls++;
						chstalls++;
						pipdecode.isEmpty=true;
						pipfetch.isEmpty = true;
						PC = mux_isbranch.output();
						endflag=0;
						cout << "Branch jump stall at " << pipdecode.pc << " jump to " << mux_isbranch.output() << "\n";

						if(branch_prediction && mux_isbranch.select_line!=0) {
							branch_pred[pipdecode.pc] =  mux_isbranch.output();
							branc_mispreds++;
						}

					}
					else {
						if(branch_prediction && mux_isbranch.select_line!=0) {
							branch_pred[pipdecode.pc] =  mux_isbranch.output();
						}
						branchjump_stall = false;
					}
				}

				// cout<<"pipdecode.op2 = "<<pipdecode.OP2<<"\n";
				pipexecute.input_vars(pipdecode.rs1, pipdecode.rs2, pipdecode.rd, pipdecode.OP2, pipdecode.pc, alu.output(), pipdecode.immu, pipdecode.wbadder_out);
				pipexecute.input_controls(pipdecode.m, pipdecode.wb);
				pipexecute.isEmpty = false;

				cout << "EX pc: " << pipdecode.pc << " ";
				
				if(pipfetch.isEmpty && endflag)
					pipdecode.isEmpty = true;
				cout << " in execution " <<  "\n";
				// cout<<"aluout: "<<alu.output()<<"\n";

			}
			if(!pipfetch.isEmpty && !is_stall && !branchjump_stall) {
				decode();
				pipdecode.input_vars(regs.rs1, regs.rs2, regs.rd, regs.op1(), regs.op2(), pipfetch.pc, mux_op2select.output(), adder_branch.output(), immU.output());
				pipdecode.isEmpty = false;
				map<string, int> ex, m, wb;
				ex["op2mux_sel"] = mux_op2select.select_line;
				ex["AluOperation"] = alu.operation;
				ex["isBranch"] = mux_isbranch.select_line;
				ex["func3"] = pipdecode.func3;
				m["MemOp"] = mem.iswrite;
				m["sltype"] = mem.sltype;
				m["ResultSelect"] = mux_resultselect.select_line;
				wb["RFWrite"] = regs.rfwrite;
				pipdecode.input_controls(ex, m, wb);
				cout << "DEC pc: " << pipfetch.pc << " ";



				cout << " in decode "   << "\n";
			}
			// Fetch();
			if(!endflag && !is_stall && !branchjump_stall){
				vector<int> temp = fetch();
				pipfetch.input(temp, PC);
				pipfetch.isEmpty = false;
				cout << "Fetched " << " "<< PC  << "\n";
				
				
				
				if(branch_prediction) {
					string opcode="";
					for(int i=6; i>=0; i--) {
						opcode += to_string(temp[i]);
					}
					if(opcode=="1101111" && branch_pred[PC]!=0) {
						PC = branch_pred[PC];
					}
					else if(opcode=="1100011" && branch_pred[PC]!=0) {
						PC = branch_pred[PC];
					}
					else {
						PC += 4;
					}
				}
				else
					PC += 4;
				
				
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
			cout << "\n";

		}
	}

	else if (forwarding == 0) {
		while(1) {
			++cycle;
			is_stall = false;
			branchjump_stall = false;
			
			if(!pipmemory.isEmpty){
				write_back();
				instructions++;

				cout << "WB pc: " << pipmemory.pc << " ";
						
				
				cout << " in writeback " << " " "\n";
				if(pipexecute.isEmpty && pipdecode.isEmpty && pipfetch.isEmpty && endflag) {
					pipmemory.isEmpty = true;
					break;
				}
				else if (pipexecute.isEmpty&&!pipdecode.isEmpty) {
					pipmemory.isEmpty = true;
				}
			}
			if(!pipexecute.isEmpty) {
				temp_pc = pipmemory.pc;
				memory_access();
				util["pipmemrd"] = pipmemory.rd;
				utilint["pipmemisempty"] = pipmemory.isEmpty;
				pipmemory.input_vars(pipexecute.rd, pipexecute.pc, mux_isbranch.output(), mux_resultselect.output(), pipexecute.aluout, mem.output());
				pipmemory.input_controls(pipexecute.wb);
				pipmemory.isEmpty = false;
				
				
				cout << "MEM pc: " << pipexecute.pc << " ";
				
				if(pipdecode.isEmpty && pipfetch.isEmpty && endflag)
					pipexecute.isEmpty = true;
				cout << " in memory access " << "\n";
			}
			if(!pipdecode.isEmpty && !is_stall) {
				execute();
				if(is_stall) {
					stalls++;
					dhstalls++;
					pipexecute.isEmpty = true;
					pipdecode.isEmpty = true;
					cout << "Stall at " << cycle << "\n\n";
					PC = pipdecode.pc;
					vector<int> temp = fetch();
					PC = pipfetch.pc;
					pipfetch.input(temp, pipdecode.pc);
					pipfetch.isEmpty = false;
					if(endflag==1)   endflag =0;
					continue;
				}
				else if(branchjump_stall) {
					if(pipfetch.pc != mux_isbranch.output()) {
						stalls++;
						chstalls++;
						pipdecode.isEmpty=true;
						pipfetch.isEmpty = true;
						PC = mux_isbranch.output();
						endflag=0;
						cout << "Branch jump stall at " << pipdecode.pc << " jump to " << mux_isbranch.output() << "\n";

						if(branch_prediction && mux_isbranch.select_line!=0) {
							branch_pred[pipdecode.pc] =  mux_isbranch.output();
						}

					}
					else {
						branchjump_stall = false;
					}
			}
			pipexecute.input_vars(pipdecode.rs1, pipdecode.rs2, pipdecode.rd, pipdecode.OP2, pipdecode.pc, alu.output(), pipdecode.immu, pipdecode.wbadder_out);
			pipexecute.input_controls(pipdecode.m, pipdecode.wb);
			pipexecute.isEmpty = false;

				cout << "EX pc: " << pipdecode.pc << " ";
				
				if(pipfetch.isEmpty && endflag)
					pipdecode.isEmpty = true;
				cout << " in execution " <<  "\n";
			
			}
			if(!pipfetch.isEmpty && !is_stall && !branchjump_stall) {
				decode();
				pipdecode.isEmpty = false;
				pipdecode.input_vars(regs.rs1, regs.rs2, regs.rd, regs.op1(), regs.op2(), pipfetch.pc, mux_op2select.output(), adder_branch.output(), immU.output());
				map<string, int> ex, m, wb;
				ex["op2mux_sel"] = mux_op2select.select_line;
				ex["AluOperation"] = alu.operation;
				ex["isBranch"] = mux_isbranch.select_line;
				ex["func3"] = pipdecode.func3;
				m["MemOp"] = mem.iswrite;
				m["sltype"] = mem.sltype;
				m["ResultSelect"] = mux_resultselect.select_line;
				wb["RFWrite"] = regs.rfwrite;
				pipdecode.input_controls(ex, m, wb);
				cout << "DEC pc: " << pipfetch.pc << " ";



				cout << " in decode "   << "\n";
			}
			// Fetch();
			if(!endflag && !is_stall && !branchjump_stall){
				vector<int> temp = fetch();
				pipfetch.input(temp, PC);
				pipfetch.isEmpty = false;
					cout << "Fetched " << " "<< PC  << "\n";
				


				if(branch_prediction) {
					string opcode="";
					for(int i=6; i>=0; i--) {
						opcode += to_string(temp[i]);
					}
					if(opcode=="1101111" && branch_pred[PC]!=0) {
						PC = branch_pred[PC];
					}
					else if(opcode=="1100011" && branch_pred[PC]!=0) {
						PC = branch_pred[PC];
					}
					else {
						PC += 4;
					}
				}
				else
					PC += 4;
				
				
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
			cout << "\n";
		}
	}
	else {
		cout<<"Please enter valid option!!\n";
	}


	fclose(fp1);


	freopen("Details.txt", "w", stdout);

	cout << "Cycles: " << cycle << endl;
	cout << "Number of instructions: " << instructions << endl;
	cout << "CPI: " << (float)cycle/instructions << endl;
	cout << "Total stalls: " << stalls << endl;
	cout << "Stalls due to data hazards: " << dhstalls << endl;
	cout << "Stalls due to control hazards: " << chstalls << endl;

	
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
