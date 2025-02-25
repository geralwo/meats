#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>

void execute_NOP()
{
	return;
}

void disasm_NOP()
{
	printf("NULL\n");
}

uint8_t* bytecode_NOP()
{
	static uint8_t bytecode[NOP_INSTR_SIZE];
	bytecode[0] = 0x00;
	return bytecode;
}
