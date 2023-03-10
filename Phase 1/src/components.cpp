// contains classes variable that represent various units like alu, memory, register file, etc

#include "components.h"


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

void Regfile::input(vector<int> _rs1, vector<int> _rs2, vector<int> _rd={0,0,0,0,0}) {
    
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

void Regfile::write(int data, bool rfwrite=0) {
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
void Memory::input(int _address, bool _iswrite, int data=0){
    mem[address]=data;
    address=_address;
    iswrite=_iswrite;
}
int Memory::read(int _address){
    return mem[_address];
}
int Memory::output(){
    return mem[address];
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