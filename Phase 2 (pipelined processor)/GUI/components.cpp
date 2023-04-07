// contains classes variable that represent various units like alu, memory, register file, etc

#include "components.h"
#include <bitset>
#include <string>
#include <map>
#include <vector>
#include <iostream>





// ALU start
void ALU::input(int _op1, int _op2) {
    op1 = _op1;
    op2 = _op2;
}

void ALU::process() {

    switch(operation) {
        case 1: {
            // add
            out = op1 + op2;
            break;
        }
        case 2: {
            // subtract
            out = op1 - op2;
            break;
        }
        case 3: {
            // and
            out = op1 & op2;
            break;
        }
        case 4: {
            // or
            out = op1 | op2;
            break;
        }
        case 5: {
            // sll
            out = op1 << op2;
            break;
        }
        case 6: {
            // slt
            out = (op1 < op2)?1:0;
            break;
        }
        case 7: {
            // sra
            out = op1 >> op2;
            break;
        }
        case 8: {
            // srl
            out = (unsigned)op1 >> op2;
            break;
        }
        case 9: {
            // xor
            out = op1 ^ op2;
            break;
        }
    }


}

int ALU::output() {
    return out;
}
// ALU end


// regfile start
Regfile::Regfile() {
    regs["x0"] = 0;
    for(int i=1; i<32; i++) {
        string temp = "x" + to_string(i);
        regs[temp] = 0;
    }
}

void Regfile::input(vector<int> _rs1, vector<int> _rs2, vector<int> _rd) {
    
    int ird=0, irs1=0, irs2=0;
    bitset<5> bits;
    
    for(int i=0; i<5; i++) {bits[i] = _rd[4-i];}
    ird = (int)bits.to_ulong();
    rd = "x"+to_string(ird);

    for(int i=0; i<5; i++) {bits[i] = _rs1[4-i];}
    irs1 = (int)bits.to_ulong();
    rs1 = "x"+to_string(irs1);

    for(int i=0; i<5; i++) {bits[i] = _rs2[4-i];}
    irs2 = (int)bits.to_ulong();
    rs2 = "x"+to_string(irs2);
}

void Regfile::write(int data) {
    if(rfwrite && rd!="x0") {
        regs[rd] = data;
    }
}

int Regfile::op1() {
    return regs[rs1];
}
int Regfile::op2() {
    return regs[rs2];
}
// regfile end

//following part is for mux, adder, sign ext, memory used in processor(contributed by Vipul)
//mux start
void Mux::input(vector<int> _input_lines) //function declaration of input for mux
{
    input_lines.clear();
    for(int i=0;i<_input_lines.size();i++){
        input_lines.push_back(_input_lines[i]);
    }
}
int Mux::output() //function declaration of output for mux
{
    return input_lines[select_line];
}
//mux end

//memory start
Memory::Memory() {
    for(int i=0; i<100000; i++) {
        mem[i] = 0;
    }
}
void Memory::mem_addr(int _address) {
    address = _address-4096;
}
void Memory::data_write(int _op2) {
    op2 = _op2;
    

    if (iswrite) {

        bitset<32> b_op2(op2);

        bitset<8> b1, b2, b3, b4;    // b1 contains least significant part, b4 contains most significant part
        // for ex: if number is 0x12345678 then
        // b1: 78
        // b2: 56
        // b3: 34
        // b4: 12
        
        if (sltype == 0) {
            for(int i=0; i<8; i++) {b1[i] = b_op2[i];}
        }
        else if (sltype == 1) {
            for(int i=0; i<8; i++) {b1[i] = b_op2[i];}
            for(int i=8; i<16; i++) {b2[i-8] = b_op2[i];}
        }
        else {
            for(int i=0; i<8; i++) {b1[i] = b_op2[i];}
            for(int i=8; i<16; i++) {b2[i-8] = b_op2[i];}
            for(int i=16; i<24; i++) {b3[i-16] = b_op2[i];}
            for(int i=24; i<32; i++) {b4[i-24] = b_op2[i];}
        }

        mem[address] = (char)b1.to_ulong();
        mem[address+1] = (char)b2.to_ulong();
        mem[address+2] = (char)b3.to_ulong();
        mem[address+3] = (char)b4.to_ulong();

    }

}
int Memory::output() {

    if(address < 0) {
        return -1;
    }
    else if (address >= 100000) {
        return -1;
    }
    else {

        bitset<32> output;

        bitset<8> b1((int)mem[address]), b2((int)mem[address+1]), b3((int)mem[address+2]), b4((int)mem[address+3]);

        if (sltype == 0) {
            for(int i=0; i<8; i++) {output[i] = b1[i];}
        }
        else if (sltype == 1) {
            for(int i=0; i<8; i++) {output[i] = b1[i];}
            for(int i=8; i<16; i++) {output[i] = b2[i-8];}
        }
        else {
            for(int i=0; i<8; i++) {output[i] = b1[i];}
            for(int i=8; i<16; i++) {output[i] = b2[i-8];}
            for(int i=16; i<24; i++) {output[i] = b3[i-16];}
            for(int i=24; i<32; i++) {output[i] = b4[i-24];}
        }

        out = (int)output.to_ulong();
        return out;
    }

}
//memory end

//adder start
void Adder::input(int _i1, int _i2){
    i1=_i1;
    i2=_i2;
}
int Adder::output(){
    return i1+i2;
}
//adder end

//sign ext start
void Sign_ext::input(vector<int> _num){

    int k=_num.size();
    bitset<32> bits;

    for(int i=31; i>=0; i--) {

        bits[i] = _num[0];
    }
    for(int i=0; i<k; i++) {
        bits[i] = _num[k-i-1];
    }

    num = (int)bits.to_ulong();


}
int Sign_ext::output(){
    return num;
}
//sign ext ends


//BranchControl unit starts
void BranchControl::input_func3(int _func3){
    func3 = _func3;
}

void BranchControl::input(int _alu_out){
    alu_out = _alu_out;
}

void BranchControl::input_ops(int _op1, int _op2){
    op1 = _op1;
    op2 = _op2;
}

int BranchControl::output(){
    out = 2;
    switch (func3) {
    case 0: {
        //beq
        if(alu_out==0){
            out = 1;
        }
        break;
    }

    case 1: {
        //bne
        if(alu_out!=0){
            out = 1;
        }
        break;
    }
    
    case 4: {
        //blt
        if (op1>0 && op2<0) {
            out = 2;
            break;
        }
        else if (op1<0 && op2>0) {
            out = 1;
            break;
        }
        else if(alu_out<0){
            out = 1;
        }
        break;
    }

    case 5: {
        //bge
        if (op1<0 && op2>0) {
            out = 2;
            break;
        }
        else if (op1>0 && op2<0) {
            out = 1;
            break;
        }
        else if(alu_out>=0){
            out = 1;
        }
        break;
    }
    default:
        break;
    }
    return out;
}
//BranchControl unit ends

/* DON'T TOUCH */

int PC; //global PC(program counter)
map <int, string> inst_mem;
ALU alu;
Regfile regs;
Memory mem;
Mux mux_op2select, mux_resultselect, mux_branchTargetSel, mux_isbranch;
Adder adder_pc, adder_branch, adder_wb;
Sign_ext immB, immJ, imm, immS, immU;
BranchControl bcu;


/* DON'T TOUCH ENDS */