// contains classes variable that represent various units like alu, memory, register file, etc

#include "components.h"


// ALU start
void ALU::input(int _op1, int _op2, int _operation) {
    op1 = _op1;
    op2 = _op2;
    operation = _operation;
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
regfile::regfile() {
    for(int i=0; i<32; i++) {
        string temp = "x" + to_string(i);
        regs[temp] = 0;
    }
}

void regfile::input(vector<int> _rs1, vector<int> _rs2, vector<int> _rd={0,0,0,0,0}) {
    
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

void regfile::write(int data, bool rfwrite) {
    if(rfwrite && rd!="x0") {
        regs[rd] = data;
    }
}

int regfile::op1() {
    return regs[rs1];
}
int regfile::op2() {
    return regs[rs2];
}
// regfile end