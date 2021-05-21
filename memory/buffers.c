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

int timer_counter = 0;
unsigned short Cursor_Position = 0;
unsigned short keyboard_index = 0;

unsigned char UPPERCASE = 0;

struct Memory_Map{
    unsigned long long base_address;
    unsigned long long region_length;
    unsigned int region_type;
    unsigned int extended_attributes;
};

unsigned char* memory_regions = (unsigned char*)(0x6000);