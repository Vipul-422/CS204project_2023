// contains classes variable that represent various units like alu, memory, register file, etc

#include "../include/components.h"
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
void Memory::data_write(int b1, int b2, int b3, int b4) {
    
    mem[address] = b1;
    mem[address+1] = b2;
    mem[address+2] = b3;
    mem[address+3] = b4;

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
Memory mem;
//memory end

//cache start

void Cache::initialise(int cachesize, int blocksize, string _type, string _policy="", int saways=1) {
    cache_size = cachesize;
    block_size = blocksize;
    type = _type;
    sa_ways = saways;
    policy = _policy;

    lines = cache_size/(block_size*sa_ways);

    if(type == "DM") {
        for(int i=0; i<lines; i++) {
            vector<char> temp;
            for(int j=0; j<block_size; j++) {
                temp.push_back('0');
            }
            dm.push_back(make_pair(-1, temp));
        }
    }
    else if(type == "SA") {
        for(int i=0; i<lines; i++) {
            vector<pair<int,vector<char>>> tempout;
            for(int j=0; j<sa_ways; j++) {
                vector<char> temp;
                for(int k=0; k<block_size; k++) {
                    temp.push_back('0');
                }
                tempout.push_back(make_pair(-1, temp));
            }
            sa.push_back(tempout);
        }
    }


}

void Cache::cache_addr(int _address) {
    address = _address - 4096;
}

void Cache::cache_write(int _op2) {
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

        int bit1 = (char)b1.to_ulong();
        int bit2 = (char)b2.to_ulong();
        int bit3 = (char)b3.to_ulong();
        int bit4 = (char)b4.to_ulong();

        mem.data_write(bit1, bit2, bit3, bit4);

        int tag = address - (address%block_size);

        if(type == "DM") {
            int index = (address/block_size)%lines;
            if(dm[index].first == tag) {
                int diff = address-tag;
                dm[index].second[diff] = bit1;
                dm[index].second[diff+1] = bit2;
                dm[index].second[diff+2] = bit3;
                dm[index].second[diff+3] = bit4;
            }
        }
        else if(type == "SA") {
            int index = (address/block_size)%lines;
            for(int i=0; i<sa_ways; i++) {
                if (sa[index][i].first == tag) {
                    int diff = address-tag;
                    sa[index][i].second[diff] = bit1;
                    sa[index][i].second[diff+1] = bit2;
                    sa[index][i].second[diff+2] = bit3;
                    sa[index][i].second[diff+3] = bit4;
                }
            }
        }
        else if(type == "FA") {
            if(fa[tag].first==1) {
                int diff = address-tag;
                fa[tag].second[diff] = bit1;
                fa[tag].second[diff+1] = bit2;
                fa[tag].second[diff+2] = bit3;
                fa[tag].second[diff+3] = bit4;
            }
        }

    }

}


//cache end

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

//Pipelined register fetch starts
Pipfetch::Pipfetch() {
    isEmpty = true;
}
void Pipfetch::input(vector<int> _instruction, int _pc){
    instruction.clear();
    for(int i = 0; i < _instruction.size(); i++){
        instruction.push_back(_instruction[i]);
    }
    pc = _pc;
}
//Pipelined register fetch ends

//Pipelined register decode starts
Pipdecode::Pipdecode() {
    isEmpty = true;
}
void Pipdecode::input_vars(string _rs1, string _rs2, string _rd, int _RS1, int _OP2, int _pc, int _op2mux_out, int _branchadder_out, int _immu){
    rs1 = _rs1;             // rs1, rs2 and rd required for forwarding
    rs2 = _rs2;
    rd = _rd;
    RS1 = _RS1;             // default input 1 of alu
    OP2 = _OP2;             // input for data write
    pc = _pc;               
    op2mux_out = _op2mux_out;                    // default input 2 for alu
    branchadder_out = _branchadder_out;
    immu = _immu;
}
void Pipdecode::input_controls(map<string, int> _ex, map<string, int> _m, map<string, int> _wb) {
    ex = _ex;
    m = _m;
    wb = _wb;
}
//Pipelined register decode ends


//Pipelined register execute starts
Pipexecute::Pipexecute() {
    isEmpty = true;
}
void Pipexecute::input_vars(string _rs1, string _rs2, string _rd, int _OP2, int _pc, int _aluout, int _immu, int _wbadder_out){
    rs1 = _rs1;
    rs2 = _rs2;
    rd = _rd;
    OP2 = _OP2;
    pc = _pc;
    aluout = _aluout;
    immu = _immu;
    wbadder_out = _wbadder_out;
}
void Pipexecute::input_controls(map<string, int> _m, map<string, int> _wb) {
    m = _m;
    wb = _wb;
}
//Pipelined register execute ends


//Pipelined register memory starts
Pipmemory::Pipmemory() {
    isEmpty = true;
}
void Pipmemory::input_vars(string _rd, int _pc, int _isbranchmux_out, int _resultselectmux_out, int _aluout, int _memout){
    rd = _rd;
    pc = _pc;
    isbranchmux_out = _isbranchmux_out;
    resultselectmux_out = _resultselectmux_out;
    aluout = _aluout;
    memout = _memout;
}
void Pipmemory::input_controls(map<string, int> _wb) {
    wb = _wb;
}
//Pipelined register memory ends



/* DON'T TOUCH */

int PC; //global PC(program counter)
map <int, string> inst_mem;
ALU alu;
Regfile regs;
Mux mux_op2select, mux_resultselect, mux_branchTargetSel, mux_isbranch, mux1_alu, mux2_alu;
Adder adder_pc, adder_branch, adder_wb;
Sign_ext immB, immJ, imm, immS, immU;
BranchControl bcu;
Pipfetch pipfetch;
Pipdecode pipdecode;
Pipexecute pipexecute;
Pipmemory pipmemory;
Cache cache;

/* DON'T TOUCH ENDS */