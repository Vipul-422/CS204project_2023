#include "../include/riscv.h"
#include "../include/functions.h"
#include <bitset>

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

    // Basic setup for decode
    string opcode="";
    for(int i=6; i>=0; i--) {
        opcode += to_string(inst[i]);
    }


    // Immediates

    vector<int> immb = {inst[31], inst[7]};
    for(int i=30; i>=25; i--) { immb.push_back(inst[i]); }
    for(int i=11; i>=8; i--) { immb.push_back(inst[i]); }
    immb.push_back(inst[0]);
    immB.input(immb);  // immB is now live

    vector<int> immj = {inst[31]};
    for(int i=19; i>=12; i--) { immj.push_back(inst[i]); }
    immj.push_back(inst[20]);
    for(int i=30; i>=21; i--) { immj.push_back(inst[i]); }
    immj.push_back(inst[0]);
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


    // Basic setup end

    if (opcode == "0110011") {
        // arithmetic register
        regs.rfwrite = true;
        switch(func3) {
            case 0: {
                if(func7 == 0)
                    alu.operation = 1;
                else
                    alu.operation = 2;
                break;
            }
            case 4: {alu.operation = 9; break;}
            case 6: {alu.operation = 4; break;}
            case 7: {alu.operation = 3; break;}
            case 1: {alu.operation = 5; break;}
            case 5: {
                if(func7 == 0)
                    alu.operation = 8;
                else
                    alu.operation = 7;
                break;
            }
            case 2: {alu.operation = 6; break;}
        }
        mux_op2select.select_line = 0;
        mux_resultselect.select_line = 3;
    }
    else if (opcode == "0010011") {
        // arithmetic immediate
        regs.rfwrite = true;
        switch(func3) {
            case 0: {alu.operation = 1; break;}
            case 6: {alu.operation = 4; break;}
            case 7: {alu.operation = 3; break;}
        }
        mux_op2select.select_line = 1;
        mux_resultselect.select_line = 3;
    }
    else if (opcode == "0000011") {
        // lb, lh, lw
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_op2select.select_line = 1;
        alu.operation = 1;
        mux_resultselect.select_line = 2;
        mem.sltype = func3;   // 0 for b, 1 for h, 2 for w 
    }
    else if (opcode == "1100111") {
        // jalr
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_op2select.select_line = 1;
        alu.operation = 1;
        mux_resultselect.select_line = 3;
        mux_isbranch.select_line = 0;
    }
    else if (opcode == "0100011") {
        // sb, sh, sw
        regs.rfwrite = false;
        mem.iswrite = true;
        alu.operation = 1;
        mux_op2select.select_line = 2;
        mem.sltype = func3;
    }
    else if (opcode == "1100011") {
        // branching
        regs.rfwrite = false;
        mem.iswrite = false;
        mux_op2select.select_line = 0;
        mux_branchTargetSel.select_line = 0;
        alu.operation = 2;
        // !! NOTE - BRANCH SELECTION SELECTION TO BE DONE AFTER EXECUTION !!
        // mux_isbranch.select_line = 1    if condition is true
        // mux_isbranch.select_line = 2    else
    }
    else if (opcode == "0110111") {
        // lui
        // rd = imm << 12
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_resultselect.select_line = 1;
    }
    else if (opcode == "0010111") {
        // auipc
        // rd = pc + imm << 12
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_resultselect.select_line = 4;

    }
    else if (opcode == "1101111") {
        // jal
        regs.rfwrite = true;
        mem.iswrite = false;
        mux_resultselect.select_line = 0;
        mux_branchTargetSel.select_line = 1;
        mux_isbranch.select_line = 1;
    }
    else {
        // cout << "Wrong instruction at " << PC << endl;
    }


}




//executes the ALU operation based on ALUop
void execute() {
}
//perform the memory operation
void memory_op() {
}
//writes the results back to register file
void write_back() {
}
