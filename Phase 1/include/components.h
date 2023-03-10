// .h file for components.cpp

#include <string>
#include <map>
#include <vector>

using namespace std;

#ifndef COMPONENTS_H
#define COMPONENTS_H

class ALU {

    int op1, op2, operation;
    int process();

    public:
        void input(int _op1, int _op2, int _operation);
        int output();


};
ALU alu;


class regfile {

    map<string, int> regs;
    string rd, rs1, rs2;

    public:
        regfile();
        void input(vector<int> _rs1, vector<int> _rs2, vector<int> _rd={0,0,0,0,0});
        void write(int data, bool rfwrite);
        int op1();
        int op2();

};
regfile regs;




#endif