#ifndef RISCV_H
#define RISCV_H


void run_riscvsim(char * forwardd,char* branch_pred);
void reset_proc();
void load_program_memory(char* filename);
void write_data_memory();
void swi_exit();


#endif
