// contains classes variable that represent various units like alu, memory, register file, etc

#include "../include/components.h"
#include <bitset>
#include <string>
#include <map>
#include <vector>





// ALU start
void ALU::input(int _op1, int _op2) {
    op1 = _op1;
    op2 = _op2;
}

int ALU::process() {
    int out;

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

    return out;

}

int ALU::output() {
    return process();
}
// ALU end


// regfile start
Regfile::Regfile() {
    for(int i=0; i<32; i++) {
        string temp = "x" + to_string(i);
        regs[temp] = 0;
    }
}

void Regfile::input(vector<int> _rs1, vector<int> _rs2, vector<int> _rd) {
    
    int ird=0, irs1=0, irs2=0;
    for(int i=0; i<_rd.size(); i--) {
        ird = ird*2 + _rd[i];
        irs1 = irs1*2 + _rs1[i];
        irs2 = ird*2 + _rs2[i];
    }
    rd = to_string(ird);
    rs1 = to_string(irs1);
    rs2 = to_string(irs2);
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
void Memory::mem_addr(int _address) {
    address = (unsigned)_address - 268435456;
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

    bitset<32> output;

    bitset<8> b1(mem[address]), b2(mem[address+1]), b3(mem[address+2]), b4(mem[address+3]);

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

    int out = (int)output.to_ulong();
    return out;

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
    int k=_num.size()-1;
    num=0;
    for(int i=0;i<k;i++){
        if(i==0){
            num-=_num[i]*(1<<k);
        }
        else{
            num+=_num[i]*(1<<k);
        }
        k--;
    }
}
int Sign_ext::output(){
    return num;
}
//sign ext ends

/* DON'T TOUCH */

int PC; //global PC(program counter)
map <int, string> inst_mem;
ALU alu;
Regfile regs;
Memory mem;
Mux mux_op2select, mux_resultselect, mux_branchTargetSel, mux_isbranch;
Adder adder_pc, adder_branch;
Sign_ext immB, immJ, imm, immS, immU;


/* DON'T TOUCH ENDS */