#include <stdio.h>

#include "Core.h"
#include "Parser.h"

int main(int argc, const char *argv[])
{	
    if (argc != 2)
    {
        printf("Usage: %s %s\n", argv[0], "<trace-file>");

        return 0;
    }

    /* Task One */
    // TODO, (1) parse and translate all the assembly instructions into binary format;
    // (2) store the translated binary instructions into instruction memory.
    Instruction_Memory instr_mem;
    instr_mem.last = NULL;
    //printf("before the ld instr\n");
    loadInstructions(&instr_mem, argv[1]);
    //printf("after the ld instr\n");
    /* Task Two */
    // TODO, implement Core.{h,c}
    Core *core = initCore(&instr_mem);
    //printf("after init core\n");
    /* Task Three - Simulation */
    while (core->tick(core));
	printf("-----------------The original; matirix -------------------\n" );
	
	
	for(int i =0; i <128; i++)
	{		
		if (i == 0 || core->data_mem[i] != 0)
		{
				printf("data_mem[%d] - %d\n", i, core->data_mem[i]);
		}		
	}
	printf("-----------------The out matrix  -------------------\n" );
	for(int i =128; i <1024; i++)
	{		
		if (i == 0 || core->data_mem[i] != 0)
		{
				printf("data_mem[%d] - %d\n", i, core->data_mem[i]);
		}		
	}


    printf("Simulation is finished.\n");

    free(core);    
}
