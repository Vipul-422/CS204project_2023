# Single-Cycle Processor Design

The simulator flow follows a single-cycle processor design. During each clock cycle, the following steps are executed:


*Fetch*:
The instruction pointed by the Program Counter (PC) is fetched from the Instruction Memory.

*Decode*:
The fetched instruction is decoded to determine the operation to be performed, the operands involved, and the destination register.

*Execute*:
The ALU (Arithmetic Logic Unit) operates on the operands.

*Memory Access*:
If the instruction involves a memory operation (e.g. load/store), the memory is accessed to read/write data.

*Write Back*:
The result of the execution is written back to the destination register.

## *Usage*


### **How to Use**
In the CLI version, three options are available: run, step, and reset. The run option allows the simulator to execute all instructions until the program is complete. The step option allows the user to execute one instruction at a time, and the reset option resets the simulator to its initial state.

### **Input**
The CLI of the single-cycle RISC-V processor requires a machine code from venus as input.

### **Output**

The simulator generates output messages for each stage of execution in the output.txt file.
Once the execution of the program is complete, the simulator prints the state of the register file and memory.


For the machine instruction: **00A00093**, the output format would like:



>
    Value of PC is: 0
    Fetch:- Instruction fetched is - 00A00093
    Decode:- Instruction's mnemonic - ADDI, x0 = rs1 = 0, imm = 10
    Execute:- ALU output = 10
    Memory_access:- Nothing changed after memory_access!
    Write_back:- rd = x1 = 10
    PC changed to 4



## *Directory Structure*

```
|---CS204project_2023
    |---.mc files
        |---input.mc
        |---bubble_sort.mc
        |---fibonacci_number.mc
        |---sum_of_array_of_n_elements.mc
        |---merge.sort.mc
    |---Phase 1(single_cycle)
        |---include
            |---riscv.h
            |---functions.h
            |---components.h
        |---src
            |---main.cpp
            |---riscv.cpp
            |---functions.cpp
            |---components.cpp
```
## *How To Run*

### Dependencies
>mingw g++ compiler

- Clone the repository to your local machine.
- Change working directory to src.
- Run the following command:
> 
    mingw32-make
    ../bin/myRISCVSim.exe (filepath to .mc file)









