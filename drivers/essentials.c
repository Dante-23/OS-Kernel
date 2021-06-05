#pragma once

#include "../memory/buffers.c"

#define get_low_16(address) (unsigned short)((address) & 0xFFFF)
#define get_high_16(address) (unsigned short)(((address) >> 16) & 0xFFFF)

void memory_copy(unsigned char* a, unsigned char* b, unsigned short bytes){
    // copy byte by byte from a to b
    for(unsigned short i = 0; i < bytes; i++)
        b[i] = a[i];
}

int strlen(char s[]){
    char *ptr = s;
    while(*ptr != '\0') ptr++;
    return ptr - s;
}

void reverse(char s[], int size){
    unsigned int i = 0, j = size - 1;
    while(i < j){
        char ch = s[i];
        s[i] = s[j];
        s[j] = ch;
        i++;
        j--;
    }
}

void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
    reverse(str, strlen(str));
}

char* get_screen_row_data(unsigned char row){
    unsigned short position = COLUMNS * row, index = position;
    unsigned char i = 0;
    while(index < position + COLUMNS){
        vga_row_data[i++] = *(VGA_MEMORY + index * 2);
        index++;
    }
    vga_row_data[i++] = '\0';
    return vga_row_data;
}

unsigned char strcmp(char* a, char* b){
    unsigned char ok = 0;
    while(!ok){
        if(*a != *b) return ok;
        else if(*a == '\0' || *b == '\0') ok = 1;
        a++;
        b++;
    }
    return ok;
}

char* substr(char* str, unsigned int start_index, unsigned int end_index){
    // stores data in hex_string.
    // Doing this to save space.
    unsigned int index = 0;
    while(start_index <= end_index) hex_string[index++] = str[start_index++];
    hex_string[index++] = '\0';
    return hex_string;
}

unsigned char strncmp(char* str, char* param, unsigned int start_index, unsigned int end_index){
    while(start_index <= end_index){
        if(str[start_index] == param[start_index]) start_index++;
        else break;
    }
    return (start_index > end_index);
}

unsigned int mystoi(char* str){
    unsigned int number = 0;
    while(*str != '\0'){
        unsigned int digit = (unsigned int)(*str - 48);
        number = (number * 10) + digit;
        str++;
    }
    return number;
}

void scroll_screen(){
    if(Cursor_Position < ROWS * COLUMNS) return;
    unsigned short index = Cursor_Position;
    Cursor_Position = 0;
    for(unsigned char row = 1; row < ROWS; row++){
        char* first_row_data = get_screen_row_data(row);
        while(*first_row_data != 0){
            if(*first_row_data == '\n'){
                Cursor_Position += COLUMNS;
                Cursor_Position -= (Cursor_Position % COLUMNS);
                first_row_data++;
                continue;
            }
            *(VGA_MEMORY + Cursor_Position * 2) = *first_row_data;
            Cursor_Position++;
            first_row_data++;
        }
    }
    Cursor_Position = index - COLUMNS;
    Cursor_Position -= Cursor_Position % COLUMNS;
    index = Cursor_Position;
    while(index < Cursor_Position + COLUMNS){
        *(VGA_MEMORY + index * 2) = ' ';
        index++;
    }
}