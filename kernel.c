// #include "drivers/print_string.c"
#include "interrupts/interrupts.c"
#include "memory/memory.c"
#include "interrupts/floppy_disk.c"

int _main(){
	isr_install();
	unsigned short position = Get_Cursor_Position_From_Coord(0,0);
    Set_Cursor_Position(position);
    clear_screen();
    // __asm__ __volatile__("int $2");
    // __asm__ __volatile__("int $3");
	asm volatile("sti");
	init_timer(50);
	// unsigned char msr = inputbyte(0x3f4);
	// print("Master Status Register value: ");
	// print(int_to_string(msr));
	// print("\n");

	initialize_main_memory_map();
	unsigned int address = (unsigned int)allocate(25);
	print("address: ");
	print(int_to_string(address));
	print("\n");
	deallocate((void*)address);
	unsigned int address1 = (unsigned int)allocate(500);
	print("address1: ");
	print(int_to_string(address1));
	print("\n");

	print("Supported commands\n");
	print("help\n");
	print("timer\n");
	print("umemory map\n");
	print("memory size\n");
	print("current blocks\n");
	print("memory map\n\n");
	print("Enter Command: ");
}

int _start(){

}