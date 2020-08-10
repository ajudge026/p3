#include "Core.h"

Core *initCore(Instruction_Memory *i_mem)
{
    Core *core = (Core *)malloc(sizeof(Core));
    core->clk = 0;
    core->PC = 0;
    core->instr_mem = i_mem;
    core->tick = tickFunc;

    // FIXME, initialize register file here.
    // core->data_mem[8*0] = ...
    
    //data memory setting for uint64_t arr[] = {16, 128, 8, 4}

    //uint64_t arr[] = {16, 128, 8, 4};


    
	for (int i = 0; i <(1024);i++)
	{
		core->data_mem[i] = 0;
		//printf("data_mem[8*%d] = %d\n", i, i);
	}
	
	for (int i = 0; i <(16);i++)
	{
		core->data_mem[8*i] = i;
		//printf("data_mem[8*%d] = %d\n", i, i);
	}
	


	

    // FIXME, initialize data memory here.
    // core->reg_file[0] = ...

    //set the reg_files for holding the offset
	 /* core->reg_file[20] = 4;
	 core->reg_file[21] = 0;
	 core->reg_file[22] = 0; 
	 core->reg_file[27] = 1; 	
    core->reg_file[26] = 4; 
	core->reg_file[30] = 0;  */
	
	core->reg_file[25] = 4;	 
	 core->reg_file[0] = 0; 
	 core->reg_file[27] = 50; //outbase
	 core->reg_file[30] = 1; 
	 core->reg_file[31] = 2; 
	 core->reg_file[31] = 2; 
	 
    
    //score->reg_file[25] = 0; // offset
    

    return core;
}

// FIXME, implement this function
bool tickFunc(Core *core)
{
	//printf("==============  new tick =====================");
    // Steps may include
    // (Step 1) Reading instruction from instruction memory
    unsigned instruction = core->instr_mem->instructions[core->PC / 4].instruction;
    
	// (Step 2) ...
	// prints instructions in decimal
	//printf("Instruction: %u\n", instruction);
    

    Signal input = (instruction & 127);
	// prints opcode in decimal
	
    //printf("Opcode: %ld\n", input); 
	
	//holds signals from the controller
    ControlSignals signals;
    ControlUnit(instruction, input, &signals);
	//read the memory 
	

    Signal func3 =( (instruction >> (7 + 5)) & 7);
    //printf("func3 - %ld\n", func3);
	Signal func7 = ((instruction >> (7 + 5 + 3 + 5 + 5)) & 127);
	//printf("func7 in tickfunction - %ld\n", func7);
    Signal ALU_ctrl_signal = ALUControlUnit(signals.ALUOp, func7, func3);

    Register read_reg_1 = (instruction >> (7 + 5 + 3)) & 31;
    
	Register read_reg_2 = (instruction >> (7 + 5 + 3 + 5)) & 31;
	
	Register write_reg = (instruction >> 7) & 31;

    //create signal input to ALU from read data 1 output
    Signal alu_in_0;    
	alu_in_0 = core->reg_file[read_reg_1];
	//printf("the alu mux control is %ld\n",signals.ALUSrc );
	Signal read_reg_2_value =core->reg_file[read_reg_2];
	Signal read_reg_1_value =core->reg_file[read_reg_1];
	//printf("reg1 - %ld, reg1val - %ld\n", read_reg_1, read_reg_1_value);
	//printf("reg2 - %ld, reg2val - %ld\n", read_reg_2, read_reg_2_value);
	
    Signal alu_in_1 = MUX(signals.ALUSrc,core->reg_file[read_reg_2],ImmeGen( input,instruction));
    Signal ALU_output;
	
    Signal zero_alu_input;
	//printf("alu input 0 -  %ld\n", alu_in_0);
	//printf("alu input 1 - %ld\n", alu_in_1);
	
    ALU(alu_in_0, alu_in_1, ALU_ctrl_signal, &ALU_output, &zero_alu_input); // 0 is offset shuold change to imm val 
	
		
	/*  if (core->PC == 24)
	{
		printf("+++++++++++++<<<x26>>>+++++++++++++++++++++++++++++ the value of x%ld aftershifting  %ld by  %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		printf("+++++++++++++<<<x26>>>+++++++++++++++++++++++++++++ the instruction is -  %d\n", instruction);

	}
	 if (core->PC == 36)
	{
		printf("+++++++++++++<<<x20>>>+++++++++++++++++++++++++++++ the value of x%ld storing  %ld by  %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		printf("+++++++++++++<<<x20>>>+++++++++++++++++++++++++++++ the instruction is -  %d\n", instruction);

	} */
		/* 
	if (core->PC == 20)
	{
		printf("+++++++++++++<<<x24>>>+++++++++++++++++++++++++++++ the value of x%ld after adding to  %ld and %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		printf("+++++++++++++<<<x24>>>+++++++++++++++++++++++++++++ the instruction is -  %d\n", instruction);

	} */
	
	/* if(signals.MemRead == 1)
	{
		
		core->reg_file[write_reg] = core->data_mem[8*ALU_output];
		printf(":::::::::::::::: ld :::::::::::::the store register index - %ld\n",write_reg );
		printf("::::::::::::::::::: the read mem index - %ld\n",ALU_output);
		printf("::::::::::::::::::: the read mem val - %ld\n",(core->reg_file[write_reg]));
	}
	//printf("the alu control signa; is - %ld\n",ALU_ctrl_signal );
	//printf("the instruction  is - %d\n",instruction );
	//printf("the opcode is - %d\n",(instruction & 127));
	
	 if (core->PC == 28)
	{
		printf("+++++++++++++<<<x20>>>+++++++++++++++++++++++++++++ the value of x%ld after adding to  %ld and %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		printf("+++++++++++++<<<x20>>>+++++++++++++++++++++++++++++ the instruction is -  %d\n", instruction);

	}
	if (core->PC == 12)
	{
		printf("+++++++++++++<<<sll - x24>>>+++++++++++++++++++++++++++++ the value of x%ld after adding to  %ld and %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		printf("+++++++++++++<<<sll - x24>>>+++++++++++++++++++++++++++++ the instruction is -  %d\n", instruction);

	}	 */
	/* if (core->PC == 0)
	{
		printf("+++++++++++++<<<x0>>>+++++++++++++++++++++++++++++ the value of x%ld after adding to  %ld and %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		printf("+++++++++++++<<<x0>>>+++++++++++++++++++++++++++++ the instruction is -  %d\n", instruction);

	} */
	/* if (core->PC == 32)
	{
		printf("+++++++++++++<<<x28>>>+++++++++++++++++++++++++++++ the value of x%ld after adding to  %ld and %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		printf("+++++++++++++<<<x28>>>+++++++++++++++++++++++++++++ the instruction is -  %d\n", instruction);

	} 
	if (core->PC == 16)
	{
		printf("+++++++++++++<<<x24>>>+++++++++++++++++++++++++++++ the value of x%ld after adding to  %ld and %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		printf("+++++++++++++<<<x24>>>+++++++++++++++++++++++++++++ the instruction is -  %d\n", instruction);

	} 
	if ((instruction & 127) == 51 && (func3 == 0 ))
	{
		//printf("the instruction is  add\n"); //printing adding operands b4 and after
		//printf("the operands were %ld - %ld and ", read_reg_1, alu_in_0); //printing adding operands b4 and after
		//printf("the operands were %ld - %ld\n", read_reg_2, alu_in_1); //printing adding operands b4 and after
		//printf("the result is %ld\n", ALU_output); //printing adding operands b4 and after
		/*if(read_reg_1 == 32 && read_reg_2 == 26)
		{
			
			printf("++++++++++++++++++++++++++++++++++++++++++ the value of x%ld after adding to  %ld and %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		}
	} */
	/* if ((instruction & 127) == 35)
	{
		//printf("the instruction is  store (sw)\n"); //printing adding operands b4 and after
		//printf("the operands were %ld - %ld and ", read_reg_1, alu_in_0); //printing adding operands b4 and after
		//printf("the operands were %ld - %ld\n", read_reg_2, alu_in_1); //printing adding operands b4 and after
		//printf("the result is %ld\n", ALU_output); //printing adding operands b4 and after
	}
	if ((instruction & 127) == 51 && (func3 == 1 ))
	{
		if (read_reg_1 == 26)
		{
			printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<the value of the matrix left shifted  by i is - %ld\n",ALU_output );
		}
		//printf("the instruction is  sll\n"); //printing sll  operands b4 and after
		//printf("the operands were %ld - %ld and ", read_reg_1, alu_in_0); //printing sll operands b4 and after
		//printf("the operands were %ld - %ld\n", read_reg_2, alu_in_1); //printing sll  operands b4 and after
		//printf("the result is %ld\n", ALU_output); //printing sll  operands b4 and after
	}
	if ((instruction & 127) == 99 )
	{
		printf("the instruction is  bne\n"); //printing sll  operands b4 and after
		printf("============================== bne =====================the operands were %ld - %ld and ", alu_in_1, alu_in_0); //printing sll operands b4 and after
		//printf("the operands were %ld - %ld\n", read_reg_2, alu_in_1); //printing sll  operands b4 and after
		printf("the result is %ld\n", ALU_output); //printing sll  operands b4 and after
	} */

    //printf("ALU out: %ld\n", ALU_output);

    //Register write_reg = (instruction >> 7) & 31;	

    //printf("alu output should be destination address - %lu\n", ALU_output);
	//printf("reg_2_value should be data to write - %lu\n", reg_2_value);
	if(signals.MemWrite)
    {
       //printf("############################the datamem write address is -  %lu\n",  ALU_output);
		//printf("###########################the data being written is#################### -  %lu\n",  read_reg_2_value);
		core->data_mem[8*ALU_output] = read_reg_2_value;
		//printf("the data at the mem address is %u\n",   core->data_mem[8*ALU_output]);
    }
	
	// core outputs of memory 
    Signal mem_result= 0;
	 mem_result = core->data_mem[8*ALU_output];
	 /* 	if (core->PC == 20)
	{
		printf("+++++++++++++<<<x26>>>+++++++++++++++++++++++++++++ the value of x%ld after adding to  %ld and %ld is - %ld\n", write_reg, read_reg_1_value, read_reg_2_value, ALU_output );
		printf("+++++++++++++<<<x26>>>+++++++++++++++++++++++++++++ the value of the load register is -  %ld \n", mem_result );
		printf("+++++++++++++<<<x26>>>+++++++++++++++++++++++++++++ the instruction is -  %d\n", instruction);

	} */
    
    // (Step N) Increment PC. FIXME, is it correct to always increment PC by 4?!
    // use mux to choose branch or incremented pc values   
    /* mem_result|= core->data_mem[8*ALU_output + 7];
    mem_result= mem_result<< 8 | core->data_mem[8*ALU_output + 6];
    mem_result= mem_result<< 16 | core->data_mem[8*ALU_output + 5];
    mem_result= mem_result<< 24 | core->data_mem[8*ALU_output + 4];
    mem_result= mem_result<< 32 | core->data_mem[8*ALU_output + 3];
    mem_result= mem_result<< 40 | core->data_mem[8*ALU_output + 2];
    mem_result= mem_result<< 48 | core->data_mem[8*ALU_output + 1];
    mem_result= mem_result<< 56 | core->data_mem[8*ALU_output + 0]; */ // <-------------------------------- might need to change how addresses
    //printf("mem result - %ld\n", mem_result);

	Signal write_reg_val =  core->reg_file[write_reg];
	//printf("write reg - %ld, write reg val - %ld \n",write_reg, write_reg_val);
    if(signals.RegWrite)
    {
		/*if ((instruction & 127) == 51)
		{
			printf("the reigister being written to is %ld\n", write_reg	);
			printf("the  value being written  is %ld\n", MUX(signals.MemtoReg, ALU_output, mem_result));
		}
		*/
		//printf("the reigister being written to is %ld\n", write_reg	);
			//printf("the  value being written  is %ld\n", MUX(signals.MemtoReg, ALU_output, mem_result));
        core->reg_file[write_reg] = MUX(signals.MemtoReg, ALU_output, mem_result);
    }



    ////printf("Register x9 -  %ld\n", core->reg_file[9]); 
    ////printf("Register x11 -  %ld\n", core->reg_file[11]);

    Signal shifted_immediate = ShiftLeft1(ImmeGen(input, instruction));	
	if((instruction& 127 )== 99)
	{
		//printf("the instruction is  %ld\n", ALU_output); //printing adding operands b4 and after
		//printf("the comparing operands were %ld and %ld\n", alu_in_0,alu_in_1); //printing adding operands b4 and after
		//printf("the shifted imm is  %ld \n", shifted_immediate); //printing adding operands b4 and after
	}
	//printf("the mux control signal is - %d\n", (zero_alu_input && signals.Branch));
	//printf("the zero_alu_input is - %ld\n", zero_alu_input );
	//printf("the alu control is - %ld\n",ALU_ctrl_signal  );
	//printf("signals.Branch is - %ld\n",  signals.Branch);
	//printf("the non shifted immediate is - %ld\n", ImmeGen(Signal input, Signal instruction));
	//printf("the shifted immediate is  - %ld\n", shifted_immediate);
	
	Signal mux_output = MUX((zero_alu_input & signals.Branch), 4, (signed int)shifted_immediate);
    core->PC = Add(core->PC, mux_output);
	
    //printf(" Program Counter after add: %ld\n", core->PC);


	
		
	
    ++core->clk;
    // Are we reaching the final instruction?
    if (core->PC > core->instr_mem->last->addr)
    {
		//printf("the datamem stored is - %d ", core->data_mem[8*0]);
        return false;
    }
    return true;
}

// FIXME (1). Control Unit. Refer to Figure 4.18.
void ControlUnit(unsigned instruction, Signal input,
                 ControlSignals *signals)
{	
	Signal func3 = ( (instruction >> (7 + 5)) & 7);
    // For R-type - add
    if (input == 51 & (func3 == 0)) {
		//printf("RType\n"); 
        signals->ALUSrc = 0;
        signals->MemtoReg = 0;
        signals->RegWrite = 1;
        signals->MemRead = 0;
        signals->MemWrite = 0;
        signals->Branch = 0;
        signals->ALUOp = 2;
    }
	
	
	// For R-type - sll
    if (input == 51 & (func3 == 1)) {
		//printf("RType\n"); 
        signals->ALUSrc = 0;
        signals->MemtoReg = 0;
        signals->RegWrite = 1;
        signals->MemRead = 0;
        signals->MemWrite = 0;
        signals->Branch = 0;
        signals->ALUOp = 0;
    }
    // For ld 
    if (input == 3) { //opcode
	    //printf("ld\n"); 
        signals->ALUSrc = 1;
        signals->MemtoReg = 1;
        signals->RegWrite = 1;
        signals->MemRead = 1;
        signals->MemWrite = 0;
        signals->Branch = 0;
        signals->ALUOp = 0;
    }
    // For addi , slli 
    if (input == 19 ){
		//printf("slli\n"); 		
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
		//printf("sw\n"); 
        signals->ALUSrc = 1;
        signals->MemtoReg = 0; 
        signals->RegWrite = 0;
        signals->MemRead = 0;
        signals->MemWrite = 1;
        signals->Branch = 0;
        signals->ALUOp = 69;
    }
    // For beq (SB-type)
    if (input == 99){ //opcode
        //printf("bne\n"); 
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
	// slli
    if (ALUOp == 0 && Funct7 == 0 && Funct3 == 1)
    {
        return 3;
    }

	
    // ld 
    if (ALUOp == 0 && Funct3 == 3)
    {
        return 2;
    }
    //  sd
    if (ALUOp == 69)
    {
        return 2;
    }
    //  beq 
    if (ALUOp == 1)
    {
        return 6;
    }
	//printf("Funct3 - %ld\n", Funct3);
    //printf("Funct7 - %ld\n", Funct7);
    //printf("ALUOP - %ld\n", ALUOp);
    
}

// FIXME (3). Imme. Generator
Signal ImmeGen(Signal input, unsigned instruction)
{
     Signal immediate = 0;

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
			//imm goes from left to right 
			Signal immNeg = (instruction >> 31) & 1;
			Signal imm3 = (instruction >> 24) & 63 ;
			Signal imm2 = (instruction >> 8) &15;
			Signal imm1 = (instruction >> 7) &1;
			//printf("immneg = %d\n", immNeg);
			//printf("imm3 = %d\n", imm3);
			//printf("imm2 = %d\n", imm2);
			//printf("imm1 = %d\n", imm1);
			immediate = imm2;
			immediate |= imm3 <<4;
			immediate |= imm1 <<10;
			immediate |= immNeg << 11;
		/* if (immediate == 4062)
		{
									
		} */
		
		if (immediate == 4056)
		{
				immediate = -24;					
		}
		else
		{
			immediate = -16;
		}
		
		
		
		
			
			
		
		//printf("unshifted imm - %d\n", immediate);
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
	// For addition sd may need to change 
    if (ALU_ctrl_signal == 69)
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
		//printf("ALU RESULT - %ld", (input_0 - input_1));
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
    return input << 1; //<--------------------------------------------------- why? 
}

// regwrite 
 void regWrite(Signal MemWrite, Signal *data_mem, Signal data, Signal *address )
{
		
}