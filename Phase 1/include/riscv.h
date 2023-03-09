void run_riscvsim();
void reset_proc();
void load_program_memory(char* file_name);
void write_data_memory();
void swi_exit();


int read_word(unsigned char *mem, unsigned int address);
void write_word(unsigned char *mem, unsigned int address, unsigned int data);