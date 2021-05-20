// #include "drivers/print_string.c"
#include "interrupts/interrupts.c"
#include "memory/memory.c"

int _main(){
	isr_install();
	unsigned short position = Get_Cursor_Position_From_Coord(0,0);
    Set_Cursor_Position(position);
    clear_screen();
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
	asm volatile("sti");
	init_timer(50);
	char s[3];
	int_to_ascii(*memory_regions, s);
	print_string(s);
	print_string("\n");
	print_string("Testing");
	print_string("\n");

	int number = 0x512;
	print_string(hex_to_string(number));
	print_string("\n");

	print_string(int_to_string(number));
	print_string("\n");
	
	for(unsigned char i = 0; i < *memory_regions; i++){
		struct Memory_Map* mp = (struct Memory_Map*)0x5000;
		mp += i;
		print_memory_map(mp, Cursor_Position);
	}
	// print_string("hsdkashdkashdkajshdkajsdhkjashdkjashdkjashdjkahdkjahsdkjashdjkashdthethehehehhetiyoyoyouoouooyoyoyoyo", position);
}

int _start(){
	unsigned char* vga = (unsigned char*)0xb8000;
	*vga = 'Q';
}
