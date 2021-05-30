#pragma once

#define NUMBER_BUFFER_SIZE 32
#define KERNEL_CS 0x08
#define IDT_ENTRIES 256

#define VGA_MEMORY (unsigned char*)0xb8000
#define ROWS 25
#define COLUMNS 80

#define KEYBOARD_INPUT_SIZE 512

char hex_string[NUMBER_BUFFER_SIZE];
char int_string[NUMBER_BUFFER_SIZE];

unsigned char vga_row_data[COLUMNS + 1];
unsigned char keyboard_input[KEYBOARD_INPUT_SIZE];

unsigned long long timer_counter = 0;
unsigned short Cursor_Position = 0;
unsigned short keyboard_index = 0;

unsigned char UPPERCASE = 0;

//////////////////////////////////////////////////////////////////////////
//////// Memory Part /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define MEMORY_MAP_SIZE 10
#define BITMAP_SIZE 5000
#define BITMAP_BLOCK_SIZE 32
#define PAGE_SIZE 1024

unsigned int BITMAP_ACTUAL_SIZE = 0;
unsigned char last_block_bits = 0;

struct Memory_Map{
    unsigned long long base_address;
    unsigned long long region_length;
    unsigned int region_type;
    unsigned int extended_attributes;
};

unsigned int bitmap[BITMAP_SIZE];

unsigned long long memory_size = 0;
unsigned long long actual_memory_size = 0;

unsigned char* memory_regions = (unsigned char*)(0x6000);

struct Memory_Map* type_1_memory_map[MEMORY_MAP_SIZE];

unsigned char type_1_index = 0;

//////////////////////////////////////////////////////////////////////////
//////// Memory Part /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////