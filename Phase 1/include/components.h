// .h file for components.cpp

#ifndef COMPONENTS_H
#define COMPONENTS_H

// reg is the 32 registers
class mux {
    int select;
    int input[8];
    // variables
    public:
        int output();
};

#endif