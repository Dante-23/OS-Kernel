#pragma once

#define VIDEO_MEMORY 0xb8000

void print_char(unsigned char character){
	unsigned short* ptr = (unsigned short*)VIDEO_MEMORY;
	*ptr = character;
}

void print_string(unsigned char* str, unsigned short cursor_position){
    //unsigned short screen_index = cursor_position;
    //while(*str != 0){
      //  *(VIDEO_MEMORY + screen_index * 2) = *str;
        //str++;
        //screen_index++;
    //}
}
