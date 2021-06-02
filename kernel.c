// #include "drivers/print_string.c"
#include "interrupts/interrupts.c"
#include "memory/memory.c"
#include "interrupts/floppy.c"
#include "file_system/fat12.c"

// void flpydsk_write_sector_imp (unsigned char head, unsigned char track, unsigned char sector) {

// 	unsigned int st0, cyl;

// 	//! initialize DMA
// 	dma_initialize_floppy ((unsigned char*) DMA_BUFFER, 512 );

// 	//! set the DMA for read transfer
// 	dma_set_write ( FDC_DMA_CHANNEL );

// 	//! read in a sector
// 	flpydsk_send_command (
// 				FDC_CMD_WRITE_SECT | FDC_CMD_EXT_MULTITRACK | FDC_CMD_EXT_DENSITY);
// 	flpydsk_send_command ( head << 2 | _CurrentDrive );
// 	flpydsk_send_command ( track);
// 	flpydsk_send_command ( head);
// 	flpydsk_send_command ( sector);
// 	flpydsk_send_command ( FLPYDSK_SECTOR_DTL_512 );
// 	flpydsk_send_command ( ( ( sector + 1 ) >= FLPY_SECTORS_PER_TRACK ) ? FLPY_SECTORS_PER_TRACK : sector + 1 );
// 	flpydsk_send_command ( FLPYDSK_GAP3_LENGTH_3_5 );
// 	flpydsk_send_command ( 0xff );

// 	//! wait for irq
// 	flpydsk_wait_irq ();

// 	//! read status info
// 	for (int j=0; j<7; j++)
// 		flpydsk_read_data ();

// 	//! let FDC know we handled interrupt
// 	flpydsk_check_int (&st0,&cyl);
// }

void write_512_bytes(char* address, char ch){
	char* ptr = address;
	while(ptr < (address + 512)){
		*ptr = ch;
		ptr++;
	}
}

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
	// flpydsk_set_working_drive (0);

// 	//! install floppy disk to IR 38, uses IRQ 6
	// print("here\n");
	floppy_install(38);

	// print("After install\n");

// 	//! set DMA buffer to 64k
	floppy_set_dma (0x1000);

	read_boot_sector();

	// unsigned char* sector = flpydsk_read_sector (0);
// //
// //
// //
// // 
// 	print(int_to_string(FDC_CMD_WRITE_SECT | FDC_CMD_EXT_MULTITRACK | FDC_CMD_EXT_DENSITY));
// 	print("\n");

// 	print(int_to_string(0 << 2 | _CurrentDrive));
// 	print("\n");

// 	print(int_to_string(0 << 2 | _CurrentDrive));
// 	print("\n");

	// write_512_bytes((unsigned char*)DMA_BUFFER, 'P');

// 	flpydsk_control_motor (1);
// 	if (flpydsk_seek ((unsigned char)0, (unsigned char)0) != 0)
// 		return 0;

// 	//! read sector and turn motor off
// 	flpydsk_write_sector_imp ((unsigned char)0, (unsigned char)0, (unsigned char)1);
// 	flpydsk_control_motor (0);
	// unsigned char write_status = floppy_write_sector(0);
	// write_512_bytes((unsigned char*)DMA_BUFFER, 'A');
// 	flpydsk_install (38);
// 	// flpydsk_read_sector_imp(0,0,1);
	// unsigned char* sector = (unsigned char*) DMA_BUFFER;
	// write_sector(0);
	// unsigned char* sector = floppy_read_sector (0);

	// print("after sector read\n");

// 	//! display sector
	// if (sector!=0) {

	// 	int i = 0;
	// 	for ( int c = 0; c < 4; c++ ) {

	// 		for (int j = 0; j < 128; j++){
	// 			print(hex_to_string(sector[i + j]));
	// 			print(" ");
	// 		}
	// 			// DebugPrintf ("0x%x ", sector[ i + j ]);
	// 		i += 128;

	// 		// DebugPrintf("\n\rPress any key to continue\n\r");
	// 		// getch ();
	// 	}
	// }
	// else{
	// 	print("error\n");
	// }
	// 	DebugPrintf ("\n\r*** Error reading sector from disk");

	// DebugPrintf ("\n\rDone.");

	print_help();
    print("Enter Command:          ");
}

int _start(){

}