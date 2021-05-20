#pragma once
#include "../drivers/print_string.c"

struct Memory_Map{
    unsigned long long base_address;
    unsigned long long region_length;
    unsigned int region_type;
    unsigned int extended_attributes;
};

unsigned char* memory_regions = (unsigned char*)(0x6000);

void print_memory_map(struct Memory_Map* memory_map, unsigned short position){
    Set_Cursor_Position(position);

    print_string("Memory Base: ");
    char a[10];
    int_to_ascii(memory_map->base_address, a);
    print_string(a);

    Set_Cursor_Position(position + 80);

    print_string("Region Length: ");
    char b[10];
    int_to_ascii(memory_map->region_length, b);
    print_string(b);

    Set_Cursor_Position(position + 160);

    print_string("Region Type: ");
    char c[10];
    int_to_ascii(memory_map->region_type, c);
    print_string(c);

    print_string("\n");

    Set_Cursor_Position(position + 320);
}