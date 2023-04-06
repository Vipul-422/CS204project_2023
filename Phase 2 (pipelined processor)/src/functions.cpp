/* DON'T TOUCH */


#include "../include/riscv.h"
#include "../include/functions.h"
#include <bitset>
#include "../include/components.h"
#include <iostream>
#include <vector>
#include <string>


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
int isBranchInst = 0;
string inst_type;
int description = 0;

/* DON'T TOUCH ENDS */


//reads from the instruction memory and updates the instruction register
vector<int> fetch() {
    string hex_string=inst_mem[PC];
    vector<int> bin_string; //lsb at bin_string[0]
    char *p;
    long n = strtoul( hex_string.c_str(), & p, 16 );
    bitset<32> binary_form(n);
    for(int i=0;i<32;i++){
        bin_string.push_back(binary_form[i]);
    }
    //updating PC adder
    adder_pc.input(PC, 4);
    //done with adder

    return bin_string;
}




// reads the instruction register, reads operand1, operand2 from register file, decides the operation to be performed in execute stage
void decode(vector<int> inst) {


    alu.operation = 1;
    mux_op2select.select_line = 0;
    mux_branchTargetSel.select_line = 0;
    mux_isbranch.select_line = 2;
    mux_resultselect.select_line = 0;
    mem.iswrite = false;
    regs.rfwrite = false;
    mem.sltype = 2;

    isBranchInst = 0;
    
    
    // Basic setup for decode
    string opcode="";
    for(int i=6; i>=0; i--) {
        opcode += to_string(inst[i]);
    }


    // Immediates

    vector<int> immb = {inst[31], inst[7]};
    for(int i=30; i>=25; i--) { immb.push_back(inst[i]); }
    for(int i=11; i>=8; i--) { immb.push_back(inst[i]); }
    immb.push_back(0);
    immB.input(immb);  // immB is now live

    vector<int> immj(21, 0);
    immj[0] = inst[31];
    for(int i=30, j=0; i>=21; i--) { immj[10+j] = inst[i]; j++; }
    immj[9] = inst[20];
    for(int i=19, j=1; i>=12; i--) { immj[j] = inst[i]; j++;}
    immJ.input(immj);  // immJ is now live

    vector<int> immvec;
    for(int i=31; i>=20; i--) { immvec.push_back(inst[i]); }
    imm.input(immvec); // imm is now live

    vector<int> imms;
    for(int i=31; i>=25; i--) { imms.push_back(inst[i]); }
    for(int i=11; i>=7; i--) { imms.push_back(inst[i]); }
    immS.input(imms);  // immS is now live

    vector<int> immu;
    for(int i=31; i>=12; i--) { immu.push_back(inst[i]); }
    for(int i=11; i>=0; i--) { immu.push_back(0); }
    immU.input(immu);  // immU is now live


    // rs1, rs2, rd

    vector<int> rs1vec;
    for(int i=19; i>=15; i--) { rs1vec.push_back(inst[i]); }

    vector<int> rs2vec;
    for(int i=24; i>=20; i--) { rs2vec.push_back(inst[i]); }

    vector<int> rdvec;
    for(int i=11; i>=7; i--) { rdvec.push_back(inst[i]); }

    regs.input(rs1vec, rs2vec, rdvec);  // register values are now live


    // func3, func7
    int func3=0, func7=0;

    for(int i=14; i>=12; i--) { func3 = func3*2 + inst[i]; }
    for(int i=31; i>=25; i--) { func7 = func7*2 + inst[i]; }
    bcu.input_func3(func3);


    // Basic setup end

    if (opcode == "0110011") {
        // arithmetic register
        description = 1;
        regs.rfwrite = true;
        switch(func3) {
            case 0: {
                if(func7 == 0){
                    alu.operation = 1;
                    inst_type = "ADD";
                }
                else{
                    alu.operation = 2;
                    inst_type = "SUB";
                }
                break;
            }
            case 4: {alu.operation = 9; inst_type = "XOR"; break;}
            case 6: {alu.operation = 4; inst_type = "OR"; break;}
            case 7: {alu.operation = 3; inst_type = "AND"; break;}
            case 1: {alu.operation = 5; inst_type = "SLL"; break;}
            case 5: {
                if(func7 == 0){
                    alu.operation = 8;
                    inst_type = "SRL";
                }
                else{
                    alu.operation = 7;
                    inst_type = "SRA";
                }
                break;
            }
            case 2: {alu.operation = 6; inst_type = "SLT"; break;}
        }
        mux_op2select.select_line = 0;
        mux_resultselect.select_line = 3;
    }
    else if (opcode == "0010011") {
        // arithmetic immediate
        description = 2;
        regs.rfwrite = true;
        switch(func3) {
            case 0: {alu.operation = 1; inst_type = "ADDI"; break;}
            case 4: {alu.operation = 9; inst_type = "XORI"; break;}
            case 6: {alu.operation = 4; inst_type = "ORI"; break;}
            case 7: {alu.operation = 3; inst_type = "ANDI"; break;}
            case 1: {alu.operation = 5; inst_type = "SLLI"; break;}
            case 5: {
                if(func7 == 0){
                    alu.operation = 8;
                    inst_type = "SRLI";
                }
                else{
                    alu.operation = 7;
                    inst_type = "SRAI";
                }
                break;
            }
            case 2: {alu.operation = 6; inst_type = "SLTI"; break;}
        }
        mux_op2select.select_line = 1;
        mux_resultselect.select_line = 3;
    }
    else if (opcode == "0000011") {
        // lb, lh, lw
        description = 3;
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_op2select.select_line = 1;
        alu.operation = 1;
        mux_resultselect.select_line = 2;
        mem.sltype = func3;   // 0 for b, 1 for h, 2 for w 
        if(mem.sltype==0){
            inst_type = "LB";
        }
        else if(mem.sltype==1){
            inst_type = "LH";
        }
        else if(mem.sltype==2){
            inst_type = "LW";
        }
    }
    else if (opcode == "1100111") {
        // jalr
        description = 4;
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_op2select.select_line = 1;
        alu.operation = 1;
        mux_resultselect.select_line = 3;
        mux_isbranch.select_line = 0;
        inst_type = "JALR";
    }
    else if (opcode == "0100011") {
        // sb, sh, sw
        description = 5;
        regs.rfwrite = false;
        mem.iswrite = true;
        alu.operation = 1;
        mux_op2select.select_line = 2;
        mem.sltype = func3;
        if(mem.sltype==0){
            inst_type = "SB";
        }
        else if(mem.sltype==1){
            inst_type = "SH";
        }
        else if(mem.sltype==2){
            inst_type = "SW";
        }
    }
    else if (opcode == "1100011") {
        // branching
        description = 6;
        isBranchInst = 1;
        regs.rfwrite = false;
        mem.iswrite = false;
        mux_op2select.select_line = 0;
        mux_branchTargetSel.select_line = 0;
        alu.operation = 2;
        bcu.input_func3(func3);
        if(func3==0) {
            inst_type = "BEQ";
        }
        else if(func3==1) {
            inst_type = "BNE";
        }
        else if(func3==4) {
            inst_type = "BLT";
        }
        else if(func3==5) {
            inst_type = "BGE";
        }
    }
    else if (opcode == "0110111") {
        // lui
        // rd = imm << 12
        description = 7;
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_resultselect.select_line = 1;
        inst_type = "LUI";
    }
    else if (opcode == "0010111") {
        // auipc
        // rd = pc + imm << 12
        description = 8;
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_resultselect.select_line = 4;
        inst_type = "AUIPC";
    }
    else if (opcode == "1101111") {
        // jal
        description = 9;
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_resultselect.select_line = 0;
        mux_branchTargetSel.select_line = 1;
        mux_isbranch.select_line = 1;
        inst_type = "JAL";
    }
    else {
        //for any wrong instruction
        cout<<"Wrong instruction!!! instruction: "<<inst_mem[PC]<<" at PC = "<<PC<<"\n";
        exit(1);
    }
    

    //populating mux_branchTargetSel.
    vector<int> _input_lines;
    _input_lines.push_back(immB.output());
    _input_lines.push_back(immJ.output());
    mux_branchTargetSel.input(_input_lines);
    //done with mux

    //populating mux_op2select
    _input_lines.clear();
    _input_lines.push_back(regs.op2());
    _input_lines.push_back(imm.output());
    _input_lines.push_back(immS.output());
    mux_op2select.input(_input_lines);
    //done with mux

    //updating branch adder
    adder_branch.input(PC, mux_branchTargetSel.output());
    //updated branch adder

    //updating wb adder
    adder_wb.input(PC, immU.output());
    //updated wb adder

}

//executes the ALU operation based on ALUop
void execute() {
    //executing ALU unit
    alu.input(regs.op1(), mux_op2select.output());
    alu.process();
    //execution done.

    if (isBranchInst == 1) {
        //using BranchControl unit
        bcu.input(alu.output());
        bcu.input_ops(regs.op1(), regs.op2());
        mux_isbranch.select_line = bcu.output();
        //isBranch updated
    }

    //populating mux_isbranch
    vector<int> _input_lines;
    _input_lines.clear();
    _input_lines.push_back(alu.output());
    _input_lines.push_back(adder_branch.output());
    _input_lines.push_back(adder_pc.output());
    mux_isbranch.input(_input_lines);
    //done with mux
}

//perform the memory operation
void memory_access() {
    mem.iswrite = pipexecute.m["MemOp"];
    mem.mem_addr(pipexecute.aluout);
    mem.data_write(pipexecute.OP2);


    //updating mux_resultselect
    vector<int> _input_lines;
    _input_lines.push_back(pipexecute.pc + 4);
    _input_lines.push_back(pipexecute.immu);
    _input_lines.push_back(mem.output());
    _input_lines.push_back(pipexecute.aluout);
    _input_lines.push_back(pipexecute.wbadder_out);
    mux_resultselect.input(_input_lines);
    //updated mux
}

//writes the results back to register file
void write_back() {
    
    //taking input from pipeline registers

    int pc = pipmemory.pc;
    regs.rfwrite = pipmemory.wb["rfwrite"];

    //done taking input from pipeline registers


    //start
    if(regs.rfwrite){
        regs.write(pipmemory.resultselectmux_out);
    }
    //end

    //updating PC for next cycle
    PC = pipmemory.isbranch_out;
    //updated PC.
}