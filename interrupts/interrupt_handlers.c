#pragma once

#include "../drivers/print_string.c"
#include "../drivers/essentials.c"
#include "../memory/memory.c"
#include "../file_system/fat12.c"

void print_sector(unsigned char* sector){
	int i = 0;
	for ( int c = 0; c < 4; c++ ) {

		for (int j = 0; j < 128; j++){
			print(hex_to_string(sector[i + j]));
			print(" ");
		}
			// DebugPrintf ("0x%x ", sector[ i + j ]);
		i += 128;
		// DebugPrintf("\n\rPress any key to continue\n\r");
		// getch ();
	}
}

void print_help(){
    print("\n");
    print("----- Supported commands -----\n");
    print("(1) help\n");
    print("(2) timer\n");
    print("(3) umemory map\n");
    print("(4) memory size\n");
    print("(5) current blocks\n");
    print("(6) allocate {bytes}\n");
    print("(7) deallocate {address}\n");
    print("(8) bpb\n");
    print("(9) ls\n");
    print("(10) memory map\n\n");
}

void handle_backspace(){
    if(keyboard_index > 0){
        keyboard_index--;
        Cursor_Position--;
        *(VGA_MEMORY + Cursor_Position * 2) = ' ';
    }
}

void handle_space(){
    keyboard_input[keyboard_index++] = ' ';
    *(VGA_MEMORY + Cursor_Position * 2) = ' ';
    Cursor_Position++;
}

void handle_tab(){
    handle_space();
    handle_space();
    handle_space();
}

void handle_enter(){
    keyboard_input[keyboard_index++] = '\0';
    keyboard_index = 0;
    print("\n");
    if(strcmp(keyboard_input, (unsigned char*)"memory map")){
        for(unsigned char i = 0; i < *memory_regions; i++){
            struct Memory_Map* mp = (struct Memory_Map*)0x5000;
            mp += i;
            print_memory_map(mp, Cursor_Position);
        }
    }
    else if(strcmp(keyboard_input, (unsigned char*)"umemory map")){
        get_type_1_memory_map();
        for(unsigned char index = 0; index < type_1_index; index++){
            print_memory_map(type_1_memory_map[index], Cursor_Position);
        }
    }
    else if(strcmp(keyboard_input, (unsigned char*)"memory size")){
        print("\n");
        print("Actual memory size:     ");
        print(int_to_string(actual_memory_size));
        print("\n");
        print("memory size:            ");
        print(int_to_string(memory_size));
        print("\n\n");
    }
    else if(strcmp(keyboard_input, (unsigned char*)"help")){
        print_help();
    }
    else if(strcmp(keyboard_input, (unsigned char*)"timer")){
        print("\n");
        print("Current timer value:    ");
        print(int_to_string(timer_counter));
        print("\n\n");
    }
    else if(strncmp(keyboard_input, (unsigned char*)"allocate", 0, 7)){
        unsigned short len = strlen(keyboard_input);
        char* bytes = substr(keyboard_input, 9, len);
        unsigned int number = mystoi(bytes);
        void* address = allocate(number);
        print("Allocated start address: ");
        print(int_to_string((unsigned int)address));
        print("\n\n");
    }
    else if(strncmp(keyboard_input, (unsigned char*)"deallocate", 0, 9)){
        unsigned short len = strlen(keyboard_input);
        char* address = substr(keyboard_input, 11, len);
        unsigned int number = mystoi(address);
        deallocate((void*)number);
        print("deallocated\n\n");
    }
    else if(strcmp(keyboard_input, (unsigned char*)"address")){
        print("\n");
        print(int_to_string((unsigned int)bitmap));
        print("\n");
        print(int_to_string((unsigned int)(bitmap + BITMAP_SIZE)));
        print("\n\n");
    }
    else if(strcmp(keyboard_input, (unsigned char*)"ls")){
        print("\n");
        // floppy_read_sector(0);
        outputbyte(0x20, 0x20); // EOI command to primary controller
        outputbyte(0xa0, 0x20);
        print_root_directory_table();
        print("\n\n");
    }
    else if(strcmp(keyboard_input, (unsigned char*)"bpb")){
        print("\n");
        outputbyte(0x20, 0x20); // EOI command to primary controller
        outputbyte(0xa0, 0x20);
        read_boot_sector();
        print("\n\n");
    }
    else if(strncmp(keyboard_input, (unsigned char*)"read", 0, 3)){
        unsigned short len = strlen(keyboard_input);
        char* filename = substr(keyboard_input, 5, len - 1);
        filename[len] = '\0';
        print("Filename:               ");
        print(filename);
        print("\n");
        outputbyte(0x20, 0x20); // EOI command to primary controller
        outputbyte(0xa0, 0x20);
        unsigned char* file_contents = (unsigned char*)allocate(1024);
        read_file(filename, file_contents);
        print_sector(file_contents);
        deallocate((void*)file_contents);
        print("\n\n");
    }
    else if(strcmp(keyboard_input, (unsigned char*)"current blocks")){
        unsigned int free_blocks = get_number_of_free_blocks();
        unsigned int alloc_blocks = get_number_of_alloc_blocks();
        print("\n");
        
        print("Bit Map size:           ");
        print(int_to_string(BITMAP_ACTUAL_SIZE));
        print("\n");

        print("Total blocks:           ");
        print(int_to_string(free_blocks + alloc_blocks));
        print("\n");
        print("Free blocks:            ");
        print(int_to_string(free_blocks));
        print("\n");

        print("Allocated blocks:       ");
        print(int_to_string(alloc_blocks));
        print("\n");
        print("\n\n");
    }
    else{
        print("Unknown input\n\n");
    }
    print("Enter Command:          ");
}

void print_letter(unsigned char scancode) {
    switch (scancode) {
        case 0x0:
            // keyboard_input[keyboard_index++] = 
            // print("ERROR");
            break;
        case 0x1:
            // print("ESC");
            break;
        case 0x2:
            keyboard_input[keyboard_index++] = '1';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x3:
            // print("2");
            keyboard_input[keyboard_index++] = '2';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x4:
            // print("3");
            keyboard_input[keyboard_index++] = '3';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x5:
            // print("4");
            keyboard_input[keyboard_index++] = '4';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x6:
            // print("5");
            keyboard_input[keyboard_index++] = '5';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x7:
            // print("6");
            keyboard_input[keyboard_index++] = '6';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x8:
            // print("7");
            keyboard_input[keyboard_index++] = '7';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x9:
            // print("8");
            keyboard_input[keyboard_index++] = '8';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x0A:
            // print("9");
            keyboard_input[keyboard_index++] = '9';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x0B:
            // print("0");
            keyboard_input[keyboard_index++] = '0';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x0C:
            // print("-");
            keyboard_input[keyboard_index++] = '-';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x0D:
            // print("+");
            keyboard_input[keyboard_index++] = '+';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x0E:
            // print("Backspace");
            handle_backspace();
            break;
        case 0x0F:
            // print("Tab");
            handle_tab();
            break;
        case 0x10:
            // print("Q");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'Q' : 'q';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x11:
            // print("W");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'W' : 'w';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x12:
            // print("E");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'E' : 'e';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x13:
            // print("R");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'R' : 'r';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x14:
            // print("T");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'T' : 't';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x15:
            // print("Y");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'Y' : 'y';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x16:
            // print("U");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'U' : 'u';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x17:
            // print("I");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'I' : 'i';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x18:
            // print("O");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'O' : 'o';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x19:
            // print("P");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'P' : 'p';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
		case 0x1A:
			// print("[");
            keyboard_input[keyboard_index++] = UPPERCASE ? '[' : '[';
            print_char(keyboard_input[keyboard_index - 1]);
			break;
		case 0x1B:
			// print("]");
            keyboard_input[keyboard_index++] = UPPERCASE ? ']' : ']';
            print_char(keyboard_input[keyboard_index - 1]);
			break;
		case 0x1C:
			// print("ENTER");
            handle_enter();
			break;
		case 0x1D:
			// print("LCtrl");
			break;
		case 0x1E:
			// print("A");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'A' : 'a';
            print_char(keyboard_input[keyboard_index - 1]);
			break;
		case 0x1F:
			// print("S");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'S' : 's';
            print_char(keyboard_input[keyboard_index - 1]);
			break;
        case 0x20:
            // print("D");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'D' : 'd';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x21:
            // print("F");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'F' : 'f';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x22:
            // print("G");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'G' : 'g';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x23:
            // print("H");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'H' : 'h';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x24:
            // print("J");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'J' : 'j';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x25:
            // print("K");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'K' : 'k';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x26:
            // print("L");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'L' : 'l';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x27:
            // print(";");
            keyboard_input[keyboard_index++] = UPPERCASE ? ';' : ';';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x28:
            // print("'");
            keyboard_input[keyboard_index++] = UPPERCASE ? '\'' : '\'';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x29:
            // print("`");
            keyboard_input[keyboard_index++] = UPPERCASE ? '`' : '`';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
		case 0x2A:
			// print("LShift");
			break;
		case 0x2B:
			// print("\\");
            keyboard_input[keyboard_index++] = UPPERCASE ? '\\' : '\\';
            print_char(keyboard_input[keyboard_index - 1]);
			break;
		case 0x2C:
			// print("Z");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'Z' : 'z';
            print_char(keyboard_input[keyboard_index - 1]);
			break;
		case 0x2D:
			// print("X");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'X' : 'x';
            print_char(keyboard_input[keyboard_index - 1]);
			break;
		case 0x2E:
			// print("C");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'C' : 'c';
            print_char(keyboard_input[keyboard_index - 1]);
			break;
		case 0x2F:
			// print("V");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'V' : 'v';
            print_char(keyboard_input[keyboard_index - 1]);
			break;
        case 0x30:
            // print("B");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'B' : 'b';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x31:
            // print("N");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'N' : 'n';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x32:
            // print("M");
            keyboard_input[keyboard_index++] = UPPERCASE ? 'M' : 'm';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x33:
            // print(",");
            keyboard_input[keyboard_index++] = UPPERCASE ? ',' : ',';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x34:
            // print(".");
            keyboard_input[keyboard_index++] = UPPERCASE ? '.' : '.';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x35:
            // print("/");
            keyboard_input[keyboard_index++] = UPPERCASE ? '/' : '/';
            print_char(keyboard_input[keyboard_index - 1]);
            break;
        case 0x36:
            // print("Rshift");
            break;
        case 0x37:
            // print("Keypad *");
            break;
        case 0x38:
            // print("LAlt");
            break;
        case 0x39:
            // print("Spc");
            handle_space();
            break;

        case 0x3a:
            UPPERCASE = !UPPERCASE;
            break;
        case 0x41:
            UPPERCASE = !UPPERCASE;
            break;
        case 0x48:
            // Up arrow key
            print("up\n");
            break;
        case 0x50:
            // Down arrow key
            print("down\n");
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80 
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                // print("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                // print("key up ");
                // print_letter(scancode - 0x80);
            } else {
                // print("Unknown key up");
            }
            break;
    }
}

void handle_keyboard_interrupt(){
    unsigned char scancode = inputbyte(0x60);
    // print_(scancode);
    print_letter(scancode);

    // if(scancode < 0x3a){
    //     if(scancode == 28){
    //         print("Hello world");
    //         print("\n");
    //         // scroll_screen();
    //     }
    //     else{
    //         print("Current timer is written here: ");
    //         print(int_to_string(timer_counter));
    //         print("\n");
    //     }
    // }
    // else if(scancode == 65 || scancode == 58){
    //     UPPERCASE = !UPPERCASE;
    // }
    // else{
    //     // print(int_to_string(scancode));
    //     // print("\n");
    // }
}