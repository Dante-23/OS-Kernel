// #include "drivers/print_string.c"
#include "interrupts/interrupts.c"

int _main(){
	isr_install();
	unsigned short position = Get_Cursor_Position_From_Coord(0,0);
    Set_Cursor_Position(position);
    clear_screen();
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
	asm volatile("sti");
	init_timer(50);
	// print_string("hsdkashdkashdkajshdkajsdhkjashdkjashdkjashdjkahdkjahsdkjashdjkashdthethehehehhetiyoyoyouoouooyoyoyoyo", position);
}

int _start(){
	unsigned char* vga = (unsigned char*)0xb8000;
	*vga = 'Q';
}
