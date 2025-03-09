#include "vm.h"
#include "vm/instruction_set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute_JMPE(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t reg2 = fetch(vm);
	size_t address = vm_get_register(vm, 8);
	if (vm_get_register(vm, reg1) == vm_get_register(vm, reg2))
	{
		vm->PC = address;
	}
}

void disasm_JMPE(MeatsVM *vm)
{
	uint8_t reg1 = fetch(vm);
	uint8_t reg2 = fetch(vm);
	printf("JMPE r%u == r%u\n", reg1, reg2);
}

uint8_t *bytecode_JMPE(uint8_t reg1, uint8_t reg2)
{
	uint8_t *bytecode = malloc(JMPE_INSTR_SIZE);
	if (bytecode == NULL)
		exit(123);
	bytecode[0] = 0xFF;
	bytecode[1] = reg1;
	bytecode[2] = reg2;

	return bytecode;
}
