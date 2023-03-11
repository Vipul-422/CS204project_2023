#include "../include/components.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;


//reads from the instruction memory and updates the instruction register
vector<int> fetch();
//reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode(vector<int> inst);
//executes the ALU operation based on ALUop
void execute();
//perform the memory operation
void memory_op();
//writes the results back to register file
void write_back();
