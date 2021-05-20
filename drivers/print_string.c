#pragma once
#include "IO.c"
#include "../memory/buffers.c"
#include "essentials.c"

#define VGA_MEMORY (unsigned char*)0xb8000
#define ROWS 25
#define COLUMNS 80

unsigned short Cursor_Position = 0;

void Set_Cursor_Position(unsigned short position){
    outputbyte(0x3D4, 0x0F);
    outputbyte(0x3D5, (unsigned char)(position & 0xFF));
    outputbyte(0x3D4, 0x0E);
    outputbyte(0x3D5, (unsigned char)((position >> 8) & 0xFF));
    Cursor_Position = position;
}

unsigned short Get_Cursor_Position_From_Coord(unsigned char x, unsigned char y){
	return x * COLUMNS + y;
}

void print_string(unsigned char* str){
	unsigned char* ptr = (unsigned char*)str;
	unsigned short index = Cursor_Position;
	while(*ptr != 0){
		if(*ptr == '\n'){
			index += COLUMNS;
			index -= (index % COLUMNS);
			ptr++;
			continue;
		}
		*(VGA_MEMORY + index * 2) = *ptr;
		ptr++;
		index++;
	}
	Cursor_Position = index;
}

void clear_screen(){
	Set_Cursor_Position(0);
	for(unsigned short i = 0; i < ROWS * COLUMNS; i++){
		*(VGA_MEMORY + i * 2) = ' ';
	}
}

char* hex_to_string(int number){
    int mask = 0x0000000f;
    int index = 0;
    for(int i = 0; i <= 28; i += 4){
        int value = (number & mask);
        value = (value >> i);
        if(value >= 0 && value <= 9) value = (value + 48);
        else value += 55;
        mask = (mask << 4);
        hex_string[index++] = (char)value;
    }
    hex_string[index++] = 'x';
    hex_string[index++] = '0';
    int size = strlen(hex_string);
    reverse(hex_string, size);
    hex_string[index++] = '\0';
    return hex_string;
}

char* int_to_string(int number){
    int index = 0;
    while(number > 0){
        int value = number % 10;
        int_string[index++] = (char)(value + 48);
        number = number / 10;
    }
    int size = strlen(int_string);
    reverse(int_string, size);
    int_string[index++] = '\0';
    return int_string;
}