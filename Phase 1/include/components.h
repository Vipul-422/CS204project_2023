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

#ifndef COMPONENTS_H
#define COMPONENTS_H

class ALU {

    int op1, op2;
    int process();

    public:
        int operation;
        void input(int _op1, int _op2);
        int output();

};
ALU alu;


class Regfile {

    map<string, int> regs;
    string rd, rs1, rs2;

    public:
        Regfile();
        void input(vector<int> _rs1, vector<int> _rs2, vector<int> _rd={0,0,0,0,0});
        void write(int data, bool rfwrite=0);
        int op1();
        int op2();

};
Regfile regs;

//instruction memory start
class Memory
{
    bool iswrite; //0 for memory read and 1 for memory write
    int address;
    static unsigned int mem[100000];
    public:
        void input(int _address, bool _iswrite=0, int data=0);
        int read(int _address);
        int output();
};

Memory mem;
//instruction memory ends


//mux starts
class Mux
{
    vector<int> input_lines; //this vector will store values of all input values coming in the mux
    public:
        int select_line; //use to select which input_line will be selected
        void input(vector<int> _input_lines); //update vector<int> input_lines with the help of select_line
        int output(); //mux_name.output() will directly give us value of input
};

Mux mux_op2select;
Mux mux_resultselect;
Mux mux_branchTargetSel;
Mux mux_isbranch;
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

Adder adder_pc;
Adder adder_branch;
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


#endif