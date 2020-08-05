#include "Parser.h"

// FIXME, implement this function.
// Here shows an example on how to translate "add x10, x10, x25"
void loadInstructions(Instruction_Memory *i_mem, const char *trace)
{
    printf("Loading trace file: %s\n", trace);

    FILE *fd = fopen(trace, "r");
    if (fd == NULL)
    {
        perror("Cannot open trace file. \n");
        exit(EXIT_FAILURE);
    }

    // Iterate all the assembly instructions
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    Addr PC = 0; // program counter points to the zeroth location initially.
    int IMEM_index = 0;
    while ((read = getline(&line, &len, fd)) != -1)
    {
        // Assign program counter
        i_mem->instructions[IMEM_index].addr = PC;

        // Extract operation
        char *raw_instr = strtok(line, " ");
        if (strcmp(raw_instr, "add") == 0 ||
            strcmp(raw_instr, "sub") == 0 ||
            strcmp(raw_instr, "sll") == 0 ||
            strcmp(raw_instr, "srl") == 0 ||
            strcmp(raw_instr, "xor") == 0 ||
            strcmp(raw_instr, "or")  == 0 ||
            strcmp(raw_instr, "and") == 0)
        {
            parseRType(raw_instr, &(i_mem->instructions[IMEM_index]));
            i_mem->last = &(i_mem->instructions[IMEM_index]);
	}

         if (strcmp(raw_instr, "ld") == 0   ||
             strcmp(raw_instr, "addi") == 0 ||
             strcmp(raw_instr, "slli") == 0)
        {
            parseIType(raw_instr, &(i_mem->instructions[IMEM_index]));
            i_mem->last = &(i_mem->instructions[IMEM_index]);
        }

        if (strcmp(raw_instr, "bne") == 0)
        {
            parseSBType(raw_instr, &(i_mem->instructions[IMEM_index]));
            i_mem->last = &(i_mem->instructions[IMEM_index]);
        }


        IMEM_index++;
        PC += 4;
    }

    fclose(fd);
}

void parseRType(char *opr, Instruction *instr)
{
    instr->instruction = 0;
    unsigned opcode = 0;
    unsigned funct3 = 0;
    unsigned funct7 = 0;

    if (strcmp(opr, "add") == 0)
    {
        opcode = 51;
        funct3 = 0;
        funct7 = 0;
    }

    char *reg = strtok(NULL, ", ");
    unsigned rd = regIndex(reg);

    reg = strtok(NULL, ", ");
    unsigned rs_1 = regIndex(reg);

    reg = strtok(NULL, ", ");
    reg[strlen(reg)-1] = '\0';
    unsigned rs_2 = regIndex(reg);

    // Contruct instruction
    instr->instruction |= opcode;
    instr->instruction |= (rd << 7);
    instr->instruction |= (funct3 << (7 + 5));
    instr->instruction |= (rs_1 << (7 + 5 + 3));
    instr->instruction |= (rs_2 << (7 + 5 + 3 + 5));
    instr->instruction |= (funct7 << (7 + 5 + 3 + 5 + 5));
}

void parseIType(char *opr, Instruction *instr)
{
    instr->instruction = 0;
    unsigned opcode = 0;
    unsigned funct3 = 0;
    

    if (strcmp(opr, "ld") == 0)
    {
        opcode = 3;
        funct3 = 3;

        char *reg = strtok(NULL, ", ");
        unsigned rd = regIndex(reg);

        reg = strtok(NULL, "( ");
        unsigned immediate = atoi(reg);
        
        reg = strtok(NULL, ")");
        // reg[strlen(reg)-1] = '\0';
        unsigned rs_1 = regIndex(reg);

        if (immediate < 0) {
            // Contruct instruction
            immediate = abs(immediate);
            instr->instruction |= opcode;
            instr->instruction |= (rd << 7);
            instr->instruction |= (funct3 << (7 + 5));
            instr->instruction |= (rs_1  << (7 + 5 + 3)); 
            // instr->instruction |= (rs_2 << (7 + 5 + 3 + 5));
            instr->instruction |= ((~immediate + 1) << (7 + 5 + 3 + 5)); 
        }
        else{
            // Contruct instruction
            instr->instruction |= opcode;
            instr->instruction |= (rd << 7);
            instr->instruction |= (funct3 << (7 + 5));
            instr->instruction |= (rs_1 << (7 + 5 + 3));
            // instr->instruction |= (rs_2 << (7 + 5 + 3 + 5));
            instr->instruction |= (immediate << (7 + 5 + 3 + 5));
        }
    }

    if (strcmp(opr, "addi") == 0)
    {
        opcode = 19;
        funct3 = 0;
            
        char *reg = strtok(NULL, ", ");
        unsigned rd = regIndex(reg);

        reg = strtok(NULL, ", ");
        unsigned rs_1 = regIndex(reg);
        
        reg = strtok(NULL, ", ");
        // reg[strlen(reg)-1] = '\0';
        unsigned immediate = atoi(reg);
     
        if (immediate < 0) {
            immediate = abs(immediate); 
            // Contruct instruction
            instr->instruction |= opcode;
            instr->instruction |= (rd << 7);
            instr->instruction |= (funct3 << (7 + 5));
            instr->instruction |= (rs_1 << (7 + 5 + 3));
            // instr->instruction |= (rs_2 << (7 + 5 + 3 + 5));
            instr->instruction |= ((~immediate + 1) << (7 + 5 + 3 + 5));
        }

        else{
            // Contruct instruction
            instr->instruction |= opcode;
            instr->instruction |= (rd << 7);
            instr->instruction |= (funct3 << (7 + 5));
            instr->instruction |= (rs_1 << (7 + 5 + 3));
            // instr->instruction |= (rs_2 << (7 + 5 + 3 + 5));
            instr->instruction |= (immediate << (7 + 5 + 3 + 5));
        }
        
    }

    if (strcmp(opr, "slli") == 0)
    {
        opcode = 19;
        funct3 = 1;
            
        char *reg = strtok(NULL, ", ");
        unsigned rd = regIndex(reg);

        reg = strtok(NULL, ", ");
        unsigned rs_1 = regIndex(reg);
        
        reg = strtok(NULL, ", ");
        // reg[strlen(reg)-1] = '\0';
        unsigned immediate = atoi(reg);

        // Contruct instruction
     instr->instruction |= opcode;
     instr->instruction |= (rd << 7);
     instr->instruction |= (funct3 << (7 + 5));
     instr->instruction |= (rs_1 << (7 + 5 + 3));
     // instr->instruction |= (rs_2 << (7 + 5 + 3 + 5));
     instr->instruction |= (immediate << (7 + 5 + 3 + 5));
        
    }
 
    
    // unsigned immediate = regIndex(reg);
    
    // reg = strtok(NULL, ", ");
    // reg[strlen(reg)-1] = '\0';
    // unsigned immediate = regIndex(reg);

}

void parseSBType(char *opr, Instruction *instr)
{
    instr->instruction = 0;
    unsigned opcode = 0;
    unsigned funct3 = 0;
    unsigned funct7 = 0;

    if (strcmp(opr, "bne") == 0)
    {
        opcode = 99;
        funct3 = 1;
    }

    char *reg = strtok(NULL, ", ");
    unsigned rs_1 = regIndex(reg);

    reg = strtok(NULL, ", ");
    unsigned rs_2 = regIndex(reg);

    reg = strtok(NULL, ", ");
    reg[strlen(reg)-1] = '\0';
    unsigned immediate = atoi(reg);  
    
    if (immediate < 0){
        immediate = abs(immediate);
         // Contruct instruction
        instr->instruction |= opcode;
        instr->instruction |= ((((~immediate + 1) & (1 << 11)) >> 11) << 7);            //2's complement embeded in the shifting (negate/invert then add 1)
        instr->instruction |= ((((1 << 4) - 1) & ((~immediate + 1) >> (2 - 1))) << (7 + 1));
        instr->instruction |= (funct3 << (7 + 1 + 4));
        instr->instruction |= (rs_1 << (7 + 1 + 4 + 3));
        instr->instruction |= (rs_2 << (7 + 1 + 4 + 3 + 5));
        instr->instruction |= ((((1 << 9) - 1) & ((~immediate + 1) >> (6 - 1))) << (7 + 1 + 4 + 3 + 5 + 5));
        instr->instruction |= ((((~immediate + 1) & (1 << 12)) >> 12) << (7 + 1 + 4 + 3 + 5 + 5 + 6));

    }

    else{
        // Contruct instruction
        instr->instruction |= opcode;
        instr->instruction |= (((immediate  & (1 << 11)) >> 11) << 7);            //2's complement embeded in the shifting (negate/invert then add 1)
        instr->instruction |= ((((1 << 4) - 1) & (immediate >> (2 - 1))) << (7 + 1));
        instr->instruction |= (funct3 << (7 + 1 + 4));
        instr->instruction |= (rs_1 << (7 + 1 + 4 + 3));
        instr->instruction |= (rs_2 << (7 + 1 + 4 + 3 + 5));
        instr->instruction |= ((((1 << 9) - 1) & (immediate >> (6 - 1))) << (7 + 1 + 4 + 3 + 5 + 5));
        instr->instruction |= (((immediate & (1 << 12)) >> 12) << (7 + 1 + 4 + 3 + 5 + 5 + 6));
    }
}

int regIndex(char *reg)
{
    unsigned i = 0;
    for (i; i < NUM_OF_REGS; i++)
    {
        if (strcmp(REGISTER_NAME[i], reg) == 0)
        {
            break;
        }
    }

    return i;
}
