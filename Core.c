#include "Core.h"

Core *initCore(Instruction_Memory *i_mem)
{
    Core *core = (Core *)malloc(sizeof(Core));
    core->clk = 0;
    core->PC = 0;
    core->instr_mem = i_mem;
    core->tick = tickFunc;

    // FIXME, initialize register file here.
    // core->data_mem[0] = ...
    
    //data memory setting for uint64_t arr[] = {16, 128, 8, 4}

    //uint64_t arr[] = {16, 128, 8, 4};


    core->data_mem[0] = 16;
    core->data_mem[1] = 0;
    core->data_mem[2] = 0;
    core->data_mem[3] = 0;
    core->data_mem[4] = 0;
    core->data_mem[5] = 0;
    core->data_mem[6] = 0;
    core->data_mem[7] = 0;


    core->data_mem[8] = 128;
    core->data_mem[9] = 0;
    core->data_mem[10] = 0;
    core->data_mem[11] = 0;
    core->data_mem[12] = 0;
    core->data_mem[13] = 0;
    core->data_mem[14] = 0;
    core->data_mem[15] = 0;

    core->data_mem[16] = 8;
    core->data_mem[17] = 0;
    core->data_mem[18] = 0;
    core->data_mem[19] = 0;
    core->data_mem[20] = 0;
    core->data_mem[21] = 0;
    core->data_mem[22] = 0; 
    core->data_mem[23] = 0;

    core->data_mem[24] = 4;
    core->data_mem[25] = 0;
    core->data_mem[26] = 0;
    core->data_mem[27] = 0;
    core->data_mem[28] = 0;
    core->data_mem[29] = 0;
    core->data_mem[30] = 0;
    core->data_mem[31] = 0;



    // FIXME, initialize data memory here.
    // core->reg_file[0] = ...

    //set the reg_file
    core->reg_file[25] = 4;
    core->reg_file[10] = 4;
    core->reg_file[22] = 1;

    return core;
}

// FIXME, implement this function
bool tickFunc(Core *core)
{
    // Steps may include
    // (Step 1) Reading instruction from instruction memory
    unsigned instruction = core->instr_mem->instructions[core->PC / 4].instruction;
    
	// (Step 2) ...
	// prints instructions in decimal
	printf("Instruction: %u\n", instruction);
    

    Signal input = (instruction & 127);
	// prints opcode in decimal
	
    printf("Opcode: %ld\n", input); 
	
	//holds signals from the controller
    ControlSignals signals;
    ControlUnit(input, &signals);

    Signal func3 =( (instruction >> (7 + 5)) & 7);
    
	Signal func7 = ((instruction >> (7 + 5 + 3 + 5 + 5)) & 127);
    Signal ALU_ctrl_signal = ALUControlUnit(signals.ALUOp, func7, func3);

    Register reg_1 = (instruction >> (7 + 5 + 3)) & 31;
    
	Register reg_2 = (instruction >> (7 + 5 + 3 + 5)) & 31;

    //create signal input to ALU from read data 1 output
    Signal alu_in_0;
    alu_in_0 = core->reg_file[reg_1];

    Signal alu_in_1 = MUX(signals.ALUSrc,core->reg_file[reg_2],ImmeGen(instruction));
    Signal ALU_output;
	
    Signal zero_alu_input;

    ALU(alu_in_0, alu_in_1, ALU_ctrl_signal, &ALU_output, &zero_alu_input);
    printf("ALU out: %ld\n", ALU_output);

    Register write_reg = (instruction >> 7) & 31;

    if(signals.MemWrite)
    {
        core->data_mem[ALU_output] = alu_in_1;
    }
	
	// core outputs of memory 
    Signal mem_result= 0;
    
    // (Step N) Increment PC. FIXME, is it correct to always increment PC by 4?!
    // use mux to choose branch or incremented pc values   
    mem_result|= core->data_mem[ALU_output + 7];
    mem_result= mem_result<< 8 | core->data_mem[ALU_output + 6];
    mem_result= mem_result<< 16 | core->data_mem[ALU_output + 5];
    mem_result= mem_result<< 24 | core->data_mem[ALU_output + 4];
    mem_result= mem_result<< 32 | core->data_mem[ALU_output + 3];
    mem_result= mem_result<< 40 | core->data_mem[ALU_output + 2];
    mem_result= mem_result<< 48 | core->data_mem[ALU_output + 1];
    mem_result= mem_result<< 56 | core->data_mem[ALU_output + 0];
    printf("%ld\n", mem_result);

    if(signals.RegWrite)
    {
        core->reg_file[write_reg] = MUX(signals.MemtoReg, ALU_output, mem_result);
    }



    printf("Register x9 -  %ld\n", core->reg_file[9]); 
    printf("Register x11 -  %ld\n", core->reg_file[11]);

    Signal shifted_immediate = ShiftLeft1(ImmeGen(input));
	
    core->PC = Add(core->PC, MUX((zero_alu_input & signals.Branch), 4, (signed int)shifted_immediate));
	
    printf("ending Program Counter: %ld\n", core->PC);



    ++core->clk;
    // Are we reaching the final instruction?
    if (core->PC > core->instr_mem->last->addr)
    {
        return false;
    }
    return true;
}

// FIXME (1). Control Unit. Refer to Figure 4.18.
void ControlUnit(Signal input,
                 ControlSignals *signals)
{
    // For R-type
    if (input == 51) {
        signals->ALUSrc = 0;
        signals->MemtoReg = 0;
        signals->RegWrite = 1;
        signals->MemRead = 0;
        signals->MemWrite = 0;
        signals->Branch = 0;
        signals->ALUOp = 2;
    }
    // For ld 
    if (input == 3) { //opcode
        signals->ALUSrc = 1;
        signals->MemtoReg = 1;
        signals->RegWrite = 1;
        signals->MemRead = 1;
        signals->MemWrite = 0;
        signals->Branch = 0;
        signals->ALUOp = 0;
    }
    // For addi , slli 
    if (input == 19){
        signals->ALUSrc = 1;
        signals->MemtoReg = 1;
        signals->RegWrite = 1;
        signals->MemRead = 1;
        signals->MemWrite = 0;
        signals->Branch = 0;
        signals->ALUOp = 0;
    }
	
    // For sd (S-type)
    if (input == 35){
        signals->ALUSrc = 1;
        signals->MemtoReg = 0; 
        signals->RegWrite = 0;
        signals->MemRead = 0;
        signals->MemWrite = 1;
        signals->Branch = 0;
        signals->ALUOp = 0;
    }
    // For beq (SB-type)
    if (input == 99){ //opcode
        signals->ALUSrc = 0;
        signals->MemtoReg = 0; 
        signals->RegWrite = 0;
        signals->MemRead = 0;
        signals->MemWrite = 0;
        signals->Branch = 1;
        signals->ALUOp = 1;
    }
}

// FIXME (2). ALU Control Unit. Refer to Figure 4.12.
Signal ALUControlUnit(Signal ALUOp,
                      Signal Funct7,
                      Signal Funct3)
{
    //  add
    if (ALUOp == 2 && Funct7 == 0 && Funct3 == 0)
    {
        return 2;
    }

    

    // For subtract 
    if (ALUOp == 2 && Funct7 == 32 && Funct3 == 0)
    {
        return 6;
    }
    //and
    if (ALUOp == 2 && Funct7 == 0 && Funct3 == 7)
    {
        return 0;
    }
    //  or 
    if (ALUOp == 2 && Funct7 == 0 && Funct3 == 6)
    {
        return 1;
    }

    // ld 
    if (ALUOp == 0)
    {
        return 2;
    }
    //  sd
    if (ALUOp == 0)
    {
        return 2;
    }
    //  beq 
    if (ALUOp == 1)
    {
        return 6;
    }
    // slli
    if (ALUOp == 0 && Funct7 == 0 && Funct3 == 1)
    {
        return 3;
    }
}

// FIXME (3). Imme. Generator
Signal ImmeGen(Signal input)
{
    signed int immediate = 0;

    //ld
    if (input == 3){
        // 000000000000;
        immediate = 0;
    }
    //addi
    if (input == 19){
        //  000000000001;
        immediate = 1;
    }
    //slli
    if (input == 14)    {
        //  000000000011;
        immediate = 3;
    }
    //bne
    if (input == 99)    {
        //  111111111110;
        immediate = -4;
    }

    return immediate;

} 

// FIXME (4). ALU
void ALU(Signal input_0,
         Signal input_1,
         Signal ALU_ctrl_signal,
         Signal *ALU_result,
         Signal *zero)
{
    // For addition
    if (ALU_ctrl_signal == 2)
    {
        *ALU_result = (input_0 + input_1);
        if (*ALU_result == 0) { *zero = 1; } else { *zero = 0; }
    }
    // For and
    if (ALU_ctrl_signal == 0)
    {
        *ALU_result = (input_0 & input_1);
        if (*ALU_result == 0) { *zero = 1; } else { *zero = 0; }
    }
    // For or
    if (ALU_ctrl_signal == 1)
    {
        *ALU_result = (input_0 | input_1);
        if (*ALU_result == 0) { *zero = 1; } else { *zero = 0; }
    }
    // For subtraction
    if (ALU_ctrl_signal == 6)
    {
        *ALU_result = (input_0 - input_1);
        if (*ALU_result != 0) { *zero = 1; } else { *zero = 0; }
    }
    // For shift left
    if (ALU_ctrl_signal == 3)
    {
        *ALU_result = (input_0 << input_1);
        if (*ALU_result == 0) { *zero = 1; } else { *zero = 0; }
    }

}

// (4). MUX
Signal MUX(Signal sel,
           Signal input_0,
           Signal input_1)
{
    if (sel == 0) { return input_0; } else { return input_1; }
}

// (5). Add
Signal Add(Signal input_0,
           Signal input_1)
{
    return (input_0 + input_1);
}

// (6). ShiftLeft
Signal ShiftLeft1(Signal input)
{
    return input << 1;
}
