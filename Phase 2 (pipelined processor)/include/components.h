#ifndef COMPONENTS_H
#define COMPONENTS_H

// .h file for components.cpp

#include <string>
#include <map>
#include <vector>

using namespace std;
/*
1) instruction memory's black box
2) register file's black box
3) ALU black box
4) Data memory Black box
5) Muxes (done)
6) Sign Ext
7) Adder
*/


class ALU {

    int op1, op2, out;

    public:
        int operation;
        void input(int _op1, int _op2);
        void process();
        int output();

};


class Regfile {

    public:
        string rd, rs1, rs2;
        map<string, int> regs;
        bool rfwrite;
        Regfile();
        void input(vector<int> _rs1, vector<int> _rs2, vector<int> _rd);
        void write(int data);
        int op1();
        int op2();

};

// Data Memory
class Memory
{
    int op2;
    public:
        int address;
        int out;
        char mem[100000];
        bool iswrite; //0 for memory read and 1 for memory write
        int sltype; //0 for b, 1 for h, 2 for w
        Memory();
        void mem_addr(int _address);
        void data_write(int _op2);
        int output();
};
// Data memory ends

//mux starts
class Mux
{
    vector<int> input_lines; //this vector will store values of all input values coming in the mux
    public:
        int select_line; //use to select which input_line will be selected
        void input(vector<int> _input_lines); //update vector<int> input_lines with the help of select_line
        int output(); //mux_name.output() will directly give us value of input
};


//mux ends

//adder start
class Adder
{
    int i1;
    int i2;
    public:
        void input(int _i1, int _i2);
        int output();
};

//adder ends

//sign ext starts
class Sign_ext
{
    int num;
    public:
        void input(vector<int> num);
        int output();
};
//sign ext ends

//BranchControl starts

class BranchControl
{
    int func3;
    int alu_out;
    int op1, op2;
    int out;
    public:
        void input_func3(int _func3);
        void input_ops(int _op1, int _op2);
        void input(int _alu_out);
        int output();
};

//BranchControl ends


//Pipelined register fetch starts

class Pipfetch
{
    public:
        bool isEmpty = 0;
        vector<int> instruction;
        int pc;
        void input(vector<int> _instruction, int pc);
};

//Pipelined register fetch ends

//Pipelined register decode starts

class Pipdecode
{
    public:
        bool isEmpty = 0;
        string rs1, rs2, rd;
        int RS1 = 0, OP2 = 0, RD = 0, pc, op2select = 0, branc_adder = 0;
        map<string, int> ex, wb, m;
        void input(string _rs1, string _rs2, string _rd, int _RS1, int _OP2, int _RD, int _pc, int _op2select, int _branch_adder, map<string, int> _ex, map<string, int> _m, map<string, int> _wb);
};

//Pipelined register decode ends


//Pipelined register execute starts

class Pipexecute
{
    public:
        bool isEmpty = 0;
        string rs2, rd;
        int OP2, RD, pc, aluout;
        
        map<string, int> wb, m;
        void input(string _rs2, string _rd, int _OP2, int _RD, int _pc, int _aluout, map<string, int> _m, map<string, int> _wb);
};

//Pipelined register execute ends

//Pipelined register execute starts

class Pipmemory
{
    public:
        bool isEmpty = 0;
        string rd;
        int RD, pc, aluout, memout;
        map<string, int> wb;
        void input(string _rd, int _RD, int _pc, int _aluout, map<string, int> _wb);
};

//Pipelined register execute ends



#endif
