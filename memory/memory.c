#pragma once
#include "../drivers/print_string.c"
#include "buffers.c"

void print_memory_map(struct Memory_Map* memory_map, unsigned short position){
    // Set_Cursor_Position(position);

    print("Memory Base: ");
    char a[10];
    int_to_ascii(memory_map->base_address, a);
    print(a);

    print("\n");

    // Set_Cursor_Position(position + 80);

    print("Region Length: ");
    char b[10];
    int_to_ascii(memory_map->region_length, b);
    print(b);

    print("\n");

    // Set_Cursor_Position(position + 160);

    print("Region Type: ");
    char c[10];
    int_to_ascii(memory_map->region_type, c);
    print(c);

    print("\n\n");

    // Set_Cursor_Position(position + 320);
}