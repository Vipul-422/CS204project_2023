/* DON'T TOUCH */


#include "../include/riscv.h"
#include "../include/functions.h"
#include <bitset>
#include "../include/components.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


extern int PC; //global PC(program counter)
extern map <int, string> inst_mem;
extern ALU alu;
extern Regfile regs;
extern Memory mem;
extern Mux mux_op2select, mux_resultselect, mux_branchTargetSel, mux_isbranch, mux1_alu, mux2_alu;
extern Adder adder_pc, adder_branch, adder_wb;
extern Sign_ext immB, immJ, imm, immS, immU;
extern BranchControl bcu;
extern Pipfetch pipfetch;
extern Pipdecode pipdecode;
extern Pipexecute pipexecute;
extern Pipmemory pipmemory;
extern Cache inst_cache, cache;
extern int forwarding;
extern int temp_pc;
string inst_type;
int description = 0;
bool is_stall;
bool branchjump_stall;
// fstream file_ptr;
// file_ptr.open("temp.txt", ios::out);

/* DON'T TOUCH ENDS */



map<string, string> util;
map<string, int> utilint;



//reads from the instruction memory and updates the instruction register
vector<int> fetch() {
    vector<int> bin_string; //lsb at bin_string[0]
    inst_cache.iswrite = 0;
    inst_cache.sltype = 2;
    inst_cache.cache_addr(PC);
    int inst = inst_cache.output();
    bitset<32> binary_form(inst);
    for(int i=0;i<32;i++){
        bin_string.push_back(binary_form[i]);
    }
    //updating PC adder
    adder_pc.input(PC, 4);
    //done with adder

    return bin_string;
}




// reads the instruction register, reads operand1, operand2 from register file, decides the operation to be performed in execute stage
void decode() {

    vector<int> inst = pipfetch.instruction;

    alu.operation = 1;
    mux_op2select.select_line = 0;
    mux_branchTargetSel.select_line = 0;
    mux_isbranch.select_line = 2;
    mux_resultselect.select_line = 0;
    cache.iswrite = false;
    regs.rfwrite = false;
    cache.sltype = 2;

    int isBranchInst = 0;
    
    
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
    immJ.input(immj);  // imxmJ is now live

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
        cache.iswrite = false;
        mux_op2select.select_line = 1;
        alu.operation = 1;
        mux_resultselect.select_line = 2;
        cache.sltype = func3;   // 0 for b, 1 for h, 2 for w 
        if(cache.sltype==0){
            inst_type = "LB";
        }
        else if(cache.sltype==1){
            inst_type = "LH";
        }
        else if(cache.sltype==2){
            inst_type = "LW";
        }
    }
    else if (opcode == "1100111") {
        // jalr
        description = 4;
        regs.rfwrite = true;
        cache.iswrite = false;
        mux_op2select.select_line = 1;
        alu.operation = 1;
        mux_resultselect.select_line = 3;
        mux_isbranch.select_line = 0;
        inst_type = "JALR";
        isBranchInst = 3;
    }
    else if (opcode == "0100011") {
        // sb, sh, sw
        description = 5;
        regs.rfwrite = false;
        cache.iswrite = true;
        alu.operation = 1;
        mux_op2select.select_line = 2;
        cache.sltype = func3;
        if(cache.sltype==0){
            inst_type = "SB";
        }
        else if(cache.sltype==1){
            inst_type = "SH";
        }
        else if(cache.sltype==2){
            inst_type = "SW";
        }
    }
    else if (opcode == "1100011") {
        // branching
        description = 6;
        isBranchInst = 1;
        regs.rfwrite = false;
        cache.iswrite = false;
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
        cache.iswrite = false;
        mux_resultselect.select_line = 1;
        inst_type = "LUI";
    }
    else if (opcode == "0010111") {
        // auipc
        // rd = pc + imm << 12
        description = 8;
        regs.rfwrite = true;
        cache.iswrite = false;
        mux_resultselect.select_line = 4;
        inst_type = "AUIPC";
    }
    else if (opcode == "1101111") {
        // jal
        description = 9;
        regs.rfwrite = true;
        cache.iswrite = false;
        mux_resultselect.select_line = 0;
        mux_branchTargetSel.select_line = 1;
        mux_isbranch.select_line = 1;
        inst_type = "JAL";
        isBranchInst = 2;
    }
    else {
        //for any wrong instruction
        cout<<"Wrong instruction!!! instruction: "<<inst_mem[pipfetch.pc]<<" at PC = "<<pipfetch.pc<<"\n";
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
    adder_branch.input(pipfetch.pc, mux_branchTargetSel.output());
    //updated branch adder

    //updating wb adder
    adder_wb.input(pipfetch.pc, immU.output());
    //updated wb adder

    pipdecode.isBranchInst = isBranchInst;
    pipdecode.func3 = func3;

}

//executes the ALU operation based on ALUop
void execute() {
    
    //updating mux1_alu
    vector<int> _input_lines;
    _input_lines.push_back(pipdecode.RS1);
    // _input_lines.push_back(pipexecute.aluout);
    _input_lines.push_back(pipmemory.resultselectmux_out);
    _input_lines.push_back(utilint["pipmemrd"]);
    mux1_alu.input(_input_lines);
    mux1_alu.select_line = 0;
    //updated mux

    //updating mux2_alu
    _input_lines.clear();
    _input_lines.push_back(pipdecode.op2mux_out);
    // _input_lines.push_back(pipexecute.aluout);
    _input_lines.push_back(pipmemory.resultselectmux_out);
    mux2_alu.input_lines.push_back(utilint["pipmemrd"]);
    mux2_alu.input(_input_lines);
    mux2_alu.select_line = 0;
    //updated mux

    //executing ALU unit
    if(forwarding == 1) {
        int temp = PC;
        PC = pipdecode.pc;
        vector<int> temp_inst = fetch();
        PC = temp;
        string opcode="";
        for(int i=6; i>=0; i--) {
            opcode += to_string(temp_inst[i]);
        }
        temp = PC;
        PC = pipexecute.pc;
        temp_inst = fetch();
        PC = temp;
        string opcode1="";
        for(int i=6; i>=0; i--) {
            opcode1 += to_string(temp_inst[i]);
        }
        temp = PC;
        PC = temp_pc;
        temp_inst = fetch();
        PC = temp;
        string opcode2="";
        for(int i=6; i>=0; i--) {
            opcode2 += to_string(temp_inst[i]);
        }
        // cout<<"opcode2 = "<<opcode2<<"\n";
        if((pipdecode.rs1 == pipexecute.rd || pipdecode.rs2 == pipexecute.rd)&&!pipexecute.isEmpty) {
            if (pipexecute.rd!="x0" && opcode1 == "0000011") {
                is_stall = true;
                return;
            }
        }
        if((pipdecode.rs1 == util["pipmemrd"])&&!utilint["pipmemisempty"]) {
            // cout<<"here1\n";
            // cout<<util["pipmemrd"]<<" = "<<utilint["pipmemrd"]<<"\n";
            // mux1_alu.input_lines.pop_back();
            // int opcode = pipdecode.pc;
            mux1_alu.select_line = 2;
            if(opcode == "0110111" || opcode == "0010111" || opcode == "1101111") {
                mux1_alu.select_line = 0;
            }
            if(opcode2 == "0100011" || opcode2 == "1100011") {
                mux1_alu.select_line = 0;
            }
        }
        if((pipdecode.rs2 == util["pipmemrd"])&&!utilint["pipmemisempty"]) {
            // cout<<"here2\n";
            // mux2_alu.input_lines.pop_back();
            mux2_alu.select_line = 2;
            if(opcode == "0100011" || opcode == "0010011" || opcode == "0000011" || opcode == "0110111" || opcode == "0010111" || opcode == "1101111") {
                // cout<<"in if\n";
                mux2_alu.select_line = 0;
            }
            if(opcode == "1100111") {
                mux2_alu.select_line = 0;
            }
        }
        if((pipdecode.rs1 == pipexecute.rd)&&!pipexecute.isEmpty) {
            // cout<<"herere1\n";
            mux1_alu.select_line = 1;
            if(opcode == "0110111" || opcode == "0010111" || opcode == "1101111") {
                mux1_alu.select_line = 0;
            }
            if(opcode1 == "0100011" || opcode1 == "1100011") {
                mux1_alu.select_line = 0;
            }
            // mux2_alu.select_line = 0;
        }
        if((pipdecode.rs2 == pipexecute.rd)&&!pipexecute.isEmpty) {
            // cout<<"hererere2\n";
            // cout<<pipdecode.rs2<<" = "<<mux_resultselect.output()<<"\n";
            mux2_alu.select_line = 1;
            if(opcode == "0100011" || opcode == "0010011" || opcode == "0000011" || opcode == "0110111" || opcode == "0010111" || opcode == "1101111") {
                // cout<<"in if\n";
                mux2_alu.select_line = 0;
            }
        }   
        util.clear();
        utilint.clear();
    }
    else if(forwarding == 0) {
        if((pipdecode.rs1 == pipexecute.rd || pipdecode.rs2 == pipexecute.rd)&&!pipexecute.isEmpty) {
            if (pipexecute.rd!="x0") {
                is_stall = true;
                return;
            }
        }
        else if((pipdecode.rs1 == util["pipmemrd"] || pipdecode.rs2 == util["pipmemrd"])&&!utilint["pipmemisempty"]) {
            util.clear();
            utilint.clear();
            if(pipmemory.rd!="x0") {
                is_stall = true;
                return;
            }
        }
    }
    alu.operation = pipdecode.ex["AluOperation"];
    mux_isbranch.select_line = pipdecode.ex["isBranch"];
    // cout<<"mux1_selectline: "<<mux1_alu.select_line<<" mux2_selectline: "<<mux2_alu.select_line<<"\n";
    // cout<<"mux1_aluout: "<<mux1_alu.output()<<" mux2_aluout: "<<mux2_alu.output()<<"\n";
    alu.input(mux1_alu.output(), mux2_alu.output());
    alu.process();
    // cout<<"aluout: "<<alu.output()<<"\n";
    //populating mux_isbranch
    _input_lines.clear();
    _input_lines.push_back(alu.output());
    _input_lines.push_back(pipdecode.branchadder_out);
    _input_lines.push_back(pipdecode.pc+4);
    mux_isbranch.input(_input_lines);
    //done with mux

    //execution done.

    bcu.input_func3(pipdecode.ex["func3"]);

    if(pipdecode.isBranchInst == 3) {
        branchjump_stall = true;
        mux_isbranch.select_line = 0;
    }
    else if(pipdecode.isBranchInst == 2) {
        branchjump_stall = true;
        mux_isbranch.select_line = 1;
    }
    else if(pipdecode.isBranchInst == 1) {
        bcu.input(alu.output());
        bcu.input_ops(pipdecode.RS1, pipdecode.op2mux_out);
        mux_isbranch.select_line = bcu.output();
        branchjump_stall = true;
    }

   
}

//perform the memory operation
void memory_access() {

    cache.sltype = pipexecute.m["sltype"];
    cache.iswrite = pipexecute.m["MemOp"];
    mux_resultselect.select_line = pipexecute.m["ResultSelect"];
    // cout<<"pipexecute.aluout = "<<pipexecute.aluout<<"\n";
    cache.cache_addr(pipexecute.aluout);
    // cout<<"pipexecute.OP2 = "<<pipexecute.OP2<<"\n";
    cache.cache_write(pipexecute.OP2);


    //updating mux_resultselect
    vector<int> _input_lines;
    _input_lines.push_back(pipexecute.pc + 4);
    _input_lines.push_back(pipexecute.immu);
    _input_lines.push_back(cache.output());
    _input_lines.push_back(pipexecute.aluout);
    _input_lines.push_back(pipexecute.wbadder_out);
    mux_resultselect.input(_input_lines);
    //updated mux
}

//writes the results back to register file
void write_back() {
    
    //taking input from pipeline registers

    int pc = pipmemory.pc;
    regs.rfwrite = pipmemory.wb["RFWrite"];

    //done taking input from pipeline registers

    regs.rd = pipmemory.rd;

    //start
    if(regs.rfwrite){
        regs.write(pipmemory.resultselectmux_out);
    }




    //end

    //updating PC for next cycle
    // PC = pipmemory.isbranchmux_out;
    //updated PC.
}