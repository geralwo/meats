#include <stdio.h>
#include <stdint.h>
#include "vm.h"
#include "vm/instruction_set.h"
#include "util.h"

// Function to set a register's value
void vm_set_register(MeatsVM *vm, uint8_t reg, uint64_t value)
{
    if (reg < VM_REGISTER_COUNT)
    {
        vm->Registers[reg] = value;
    }
}

// Function to get a register's value

uint64_t vm_get_register(MeatsVM *vm, uint8_t reg)
{
    if (reg < VM_REGISTER_COUNT)
    {
        return vm->Registers[reg];
    }
    return 0;
}

void vm_set_flag(MeatsVM *vm, uint64_t flag)
{
    vm->Registers[31] |= flag;
}

// Unset (clear) a flag inside register 31
void vm_unset_flag(MeatsVM *vm, uint64_t flag)
{
    vm->Registers[31] &= ~flag;
}

// Check if a flag is set
int vm_flag_is_set(MeatsVM *vm, uint64_t flag)
{
    return (vm->Registers[31] & flag) != 0;
}

void meats_vm_dump_bytecode(MeatsVM *vm)
{
    if (vm->ProgramLength == 0)
    {
        printf(":: VM has no bytecode program\n");
    }
    else
    {
        printf("VM Program Size: %ld\n", vm->ProgramLength);
        for (size_t i = 0; i < vm->ProgramLength; i++)
        {
            printf("  %02x ", vm->Program[i]);
            if ((i + 1) % 8 == 0)
                printf("\n");
        }
        printf("\n");
    }
}

void meats_vm_print_stats(MeatsVM *vm)
{
    printf("Bytecode Size: %ld\tR31: %ld\nHeapPtr: %p\tHeapSize: %ld\n", vm->ProgramLength, vm_get_register(vm, 31), (void *)vm->Heap, vm->HeapSize);
}

void meats_vm_print_asm(MeatsVM *vm)
{
    printf("::: ASM BEGIN :::\n");
    vm->PC = 0;
    while (vm->PC < vm->ProgramLength)
    {
        uint8_t opcode = fetch(vm);
        if (disasm_table[opcode])
            disasm_table[opcode](vm);
        else
        {
            printf("0x%.2X\n", opcode);
        }
    }
    printf("::: ASM END :::\n");
    vm->PC = 0;
}

void meats_vm_dump_mem(MeatsVM *vm)
{
    for (int i = 0; i < (int)vm->HeapSize; i++)
    {
        if (i % 8 == 0)
            printf("\n");
        printf("m%u", vm->Heap[i]);
    }
    printf("\n");
    for (int i = 0; i < VM_STACK_SIZE; i++)
    {
        if (i % 8 == 0)
            printf("\n");
        printf("%lu", vm->Stack[i]);
    }
    printf("\n");
}

void meats_vm_dump_registers(MeatsVM *vm)
{
    for (int i = 0; i < VM_REGISTER_COUNT; i++)
    {
        printf("  [  r%.02d: ", i);
        print_bits(vm->Registers[i]);
        printf(" = %lu  ", vm->Registers[i]);
        printf("] \n");
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
}

size_t vm_bytes_to_size_t(const uint8_t bytes[], size_t byte_count)
{
    size_t value = 0;

    // Ensure we do not exceed the size of `size_t`
    if (byte_count > sizeof(size_t))
    {
        return 0; // Return 0 if invalid byte count
    }

    // Construct the value from bytes (big-endian order)
    for (size_t i = 0; i < byte_count; i++)
    {
        value |= (size_t)bytes[i] << ((byte_count - 1 - i) * 8);
    }
    return value;
}

// Function to convert bytes to an integer, considering the byte order (endianness)
uint16_t vm_bytes_to_uint16(const uint8_t bytes[2])
{
    uint16_t value = 0;
    value = (bytes[0] << 8) | bytes[1];
    return value;
}

// Function to convert bytes to an integer, considering the byte order (endianness)
uint32_t vm_bytes_to_uint32(const uint8_t bytes[4])
{
    uint32_t value = 0;
    // Handle endianness (assuming the byte order in the input is reversed)
    value = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    return value;
}

uint64_t vm_bytes_to_uint64(const uint8_t bytes[8])
{
    uint64_t value = 0;
    // Handle endianness for uint64_t (reversed order)
    value = ((uint64_t)bytes[0] << 56) | ((uint64_t)bytes[1] << 48) |
            ((uint64_t)bytes[2] << 40) | ((uint64_t)bytes[3] << 32) |
            ((uint64_t)bytes[4] << 24) | ((uint64_t)bytes[5] << 16) |
            ((uint64_t)bytes[6] << 8) | (uint64_t)bytes[7];
    return value;
}

uint8_t *vm_size_t_to_bytes(size_t value)
{
    static uint8_t bytes[sizeof(size_t)];
    for (size_t i = 0; i < sizeof(size_t); i++)
    {
        bytes[i] = (value >> ((sizeof(size_t) - 1 - i) * 8)) & 0xFF;
    }
    return bytes;
}

uint8_t *vm_uint64_to_bytes(uint64_t value)
{
    static uint8_t bytes[sizeof(uint64_t)];

    bytes[0] = (value >> 56) & 0xFF;
    bytes[1] = (value >> 48) & 0xFF;
    bytes[2] = (value >> 40) & 0xFF;
    bytes[3] = (value >> 32) & 0xFF;
    bytes[4] = (value >> 24) & 0xFF;
    bytes[5] = (value >> 16) & 0xFF;
    bytes[6] = (value >> 8) & 0xFF;
    bytes[7] = value & 0xFF;

    return bytes;
}

uint8_t *vm_uint32_to_bytes(uint32_t value)
{
    static uint8_t bytes[4]; // Static array to store 4 bytes

    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;

    return bytes;
}

uint8_t *vm_uint16_to_bytes(uint16_t value)
{
    static uint8_t bytes[2]; // Static array to store 2 bytes

    bytes[0] = (value >> 8) & 0xFF;
    bytes[1] = value & 0xFF;

    return bytes;
}
