#include "components.h"
#include <iostream>

// contains classes variable that represent various units like alu, memory, register file, etc

int mux::output() {
    return input[select];
}