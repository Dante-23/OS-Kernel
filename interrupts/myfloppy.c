#pragma once

#include "../drivers/IO.c"
#include "dma.c"
#include "interrupts.c"

#define DOR 0x3f2
#define MSR 0x3f4
#define FIFO 0x3f5
#define CTRL 0x3f7

#define READ_TRACK 2
#define SPECIFY 3
#define CHECK_STAT 4
#define WRITE_SECT 5
#define READ_SECT 6
#define CALIBRATE 7
#define CHECK_INT 8
#define WRITE_DEL_S 9
#define READ_ID_S 0xa
#define READ_DEL_S 0xc
#define FORMAT_TRACK 0xd
#define SEEK 0xf

#define SKIP 0x20
#define DENSITY 0x40
#define MULTITRACK 0x80

#define GAP3_LENGTH_STD 42
#define GAP3_LENGTH_5_14 32
#define GAP3_LENGTH_3_5 27

#define SECTOR_128 0
#define SECTOR_256 1
#define SECTOR_512 2
#define SECTOR_1024 4

#define SECTORS_PER_TRACK 18

unsigned int DMA_BUFFER = 0x1000;

#define FLOPPY_CHANNEL 2

const unsigned char current_drive = 0;
unsigned char floppy_interrupt = 0;

unsigned char* read_sector (int sectorLBA);
void lba_to_chs (int lba,int *head,int *track,int *sector);
void floppy_install ();
void floppy_reset ();
void floppy_wait();
unsigned char dma_initialize_floppy(unsigned short buffer, unsigned short counter);
int floppy_seek ( unsigned int cyl, unsigned int head );
int floppy_calibrate (unsigned int drive);
void floppy_check_int (unsigned int* st0, unsigned int* cyl);
void floppy_control_motor (unsigned char b);
void floppy_drive_data (unsigned int stepr, unsigned int loadt, unsigned int unloadt, unsigned char dma );
void read_sector_imp (unsigned char head, unsigned char track, unsigned char sector);
void floppy_write_ccr(unsigned char value);
unsigned char floppy_read_data();
void floppy_send_command(unsigned char command);
unsigned char floppy_read_status();
void floppy_write_dor(unsigned char value);
void handle_floppy_interrupt();
void write_sector_imp (unsigned char head, unsigned char track, unsigned char sector);
unsigned char write_sector (int sectorLBA);
void floppy_set_dma (int addr);

//! disable controller
void floppy_disable_controller () {
	floppy_write_dor (0);
}

//! enable controller
void floppy_enable_controller () {
	floppy_write_dor ( (4 | 8) );
}

void floppy_write_dor(unsigned char value){
    outputbyte(DOR, value);
}

unsigned char floppy_read_status(){
    return inputbyte(MSR);
}

void floppy_send_command(unsigned char command){
    // while((floppy_read_status() & 128) == 0);
    // outputbyte(FIFO, command);
	for (int i = 0; i < 500; i++ )
		if ( floppy_read_status () & 128)
			return outputbyte(FIFO, command);
}

unsigned char floppy_read_data(){
    // while(!(floppy_read_status() & 128));
    // return inputbyte(FIFO);
	for (int i = 0; i < 500; i++ )
		if ( floppy_read_status () & 128)
			return inputbyte(FIFO);
}

void floppy_write_ccr(unsigned char value){
    outputbyte(CTRL, value);
}

void read_sector_imp (unsigned char head, unsigned char track, unsigned char sector){
		unsigned int st0, cyl;

	//! initialize DMA
	dma_initialize_floppy ((unsigned short) DMA_BUFFER, 512 );

	//! set the DMA for read transfer
	dma_set_read ( FLOPPY_CHANNEL );

	//! read in a sector
	floppy_send_command (READ_SECT | MULTITRACK | SKIP | DENSITY);
	floppy_send_command ( head << 2 | current_drive );
	floppy_send_command ( track);
	floppy_send_command ( head);
	floppy_send_command ( sector);
	floppy_send_command ( SECTOR_512 );
	floppy_send_command ( ( ( sector + 1 ) >= SECTORS_PER_TRACK ) ? SECTORS_PER_TRACK : sector + 1 );
	floppy_send_command ( GAP3_LENGTH_3_5 );
	floppy_send_command ( 0xff );

	//! wait for irq
	print("waiting for interrupt\n");
	floppy_wait ();

	//! read status info
	for (int j=0; j<7; j++)
		floppy_read_data ();

	//! let FDC know we handled interrupt
	floppy_check_int (&st0,&cyl);	
}

void write_sector_imp (unsigned char head, unsigned char track, unsigned char sector){
		unsigned int st0, cyl;

	//! initialize DMA
	dma_initialize_floppy ((unsigned short) DMA_BUFFER, 512 );

	//! set the DMA for write transfer
	dma_set_write ( FLOPPY_CHANNEL );

	//! write on a sector
	floppy_send_command (WRITE_SECT | MULTITRACK | DENSITY);
	floppy_send_command ( head << 2 | current_drive );
	floppy_send_command ( track);
	floppy_send_command ( head);
	floppy_send_command ( sector);
	floppy_send_command ( SECTOR_512 );
	floppy_send_command ( ( ( sector + 1 ) >= SECTORS_PER_TRACK ) ? SECTORS_PER_TRACK : sector + 1 );
	floppy_send_command ( GAP3_LENGTH_3_5 );
	floppy_send_command ( 0xff );

	//! wait for irq
	floppy_wait ();

	//! read status info
	for (int j=0; j<7; j++)
		floppy_read_data ();

	//! let FDC know we handled interrupt
	floppy_check_int (&st0,&cyl);	
}

void floppy_drive_data (unsigned int stepr, unsigned int loadt, unsigned int unloadt, unsigned char dma ) {
	unsigned int data = 0;
	floppy_send_command (SPECIFY);
	data = ( (stepr & 0xf) << 4) | (unloadt & 0xf);
	floppy_send_command (data);
	data = (loadt) << 1 | (dma==1) ? 1 : 0;
	floppy_send_command (data);
}

void floppy_control_motor (unsigned char b) {
	//! sanity check: invalid drive
	if (current_drive > 3) return;
	unsigned char motor = 0;
	//! select the correct mask based on current drive
	switch (current_drive) {
		case 0:
			motor = 16;
			break;
		case 1:
			motor = 32;
			break;
		case 2:
			motor = 64;
			break;
		case 3:
			motor = 128;
			break;
	}
	//! turn on or off the motor of that drive
	if (b) floppy_write_dor ((unsigned char)(current_drive | motor | 4 | 8));
	else floppy_write_dor (4);
	//! in all cases; wait a little bit for the motor to spin up/turn off
	sleep(20);
}

void floppy_check_int (unsigned int* st0, unsigned int* cyl) {
	floppy_send_command (CHECK_INT);
	*st0 = floppy_read_data ();
	*cyl = floppy_read_data ();
}

int floppy_calibrate (unsigned int drive) {
	unsigned int st0, cyl;
	if (drive >= 4) return -2;
	//! turn on the motor
	floppy_control_motor (1);
	for (int i = 0; i < 10; i++) {
		//! send command
		floppy_send_command ( CALIBRATE );
		floppy_send_command ( drive );
		floppy_wait ();
		floppy_check_int ( &st0, &cyl);
		//! did we fine cylinder 0? if so, we are done
		if (!cyl) {
			floppy_control_motor (0);
			return 0;
		}
	}
	floppy_control_motor (0);
	return -1;
}

int floppy_seek ( unsigned int cyl, unsigned int head ) {
	unsigned int st0, cyl0;
	if (current_drive >= 4) return -1;
	for (int i = 0; i < 10; i++ ) {
		//! send the command
		floppy_send_command (SEEK);
		floppy_send_command ( (head) << 2 | current_drive);
		floppy_send_command (cyl);
		//! wait for the results phase IRQ
		floppy_wait ();
		floppy_check_int (&st0,&cyl0);
		//! found the cylinder?
		if ( cyl0 == cyl)
			return 0;
	}
	return -1;
}

unsigned char dma_initialize_floppy(unsigned short buffer, unsigned short counter){
	dma_reset (1);
	dma_mask_channel( FLOPPY_CHANNEL );//Mask channel 2
	dma_reset_flipflop ( 1 );//Flipflop reset on DMA 1

	unsigned char counter_low = (counter & 0xFF), counter_high = ((counter >> 8) & 0xFF);
	unsigned char buffer_low = (buffer & 0xFF), buffer_high = ((buffer >> 8) & 0xFF);

	dma_set_address( FLOPPY_CHANNEL, buffer_low, buffer_high);//Buffer address
	dma_reset_flipflop( 1 );//Flipflop reset on DMA 1

	dma_set_count( FLOPPY_CHANNEL, counter_low, counter_high);//Set count
	dma_set_read ( FLOPPY_CHANNEL );

	dma_unmask_all( 1 );//Unmask channel 2
	return 1;
}

void handle_floppy_interrupt(){
	floppy_interrupt = 1;
}

void floppy_wait(){
	while(floppy_interrupt == 0);
	floppy_interrupt = 0;
}

void floppy_reset () {
	unsigned int st0, cyl;
	//! reset the controller
	floppy_disable_controller();
	floppy_enable_controller();
	floppy_wait ();
	//! send CHECK_INT/SENSE INTERRUPT command to all drives
	for (int i=0; i<4; i++)
		floppy_check_int (&st0,&cyl);
	//! transfer speed 500kb/s
	floppy_write_ccr (0);
	//! pass mechanical drive info. steprate=3ms, unload time=240ms, load time=16ms
	floppy_drive_data (3,16,240,1);
	//! calibrate the disk
	int value = floppy_calibrate ( current_drive );
}

void floppy_install () {
 
	//! install irq handler
	//! initialize the DMA for FDC
	// flpydsk_initialize_dma ();
 
	//! reset the fdc
	floppy_reset ();
 
	//! set drive information
	floppy_drive_data (13, 1, 0xf, 1);
}

void lba_to_chs (int lba,int *head,int *track,int *sector) {
   *head = ( lba % ( SECTORS_PER_TRACK * 2 ) ) / ( SECTORS_PER_TRACK );
   *track = lba / ( SECTORS_PER_TRACK * 2 );
   *sector = lba % SECTORS_PER_TRACK + 1;
}

unsigned char* read_sector (int sectorLBA) {
	// floppy_install();
	if (current_drive >= 4)
		return 0;
 
	//! convert LBA sector to CHS
	int head=0, track=0, sector=1;
	lba_to_chs (sectorLBA, &head, &track, &sector);
 
	//! turn motor on and seek to track
	floppy_control_motor (1);
	if (floppy_seek (track, head) != 0)
		return 0;
 
	//! read sector and turn motor off
	print("before read sector imp\n");
	read_sector_imp (head, track, sector);
	print("after read sector imp\n");
	floppy_control_motor (0);
 
	//! warning: this is a bit hackish
	return (unsigned char*) DMA_BUFFER;
}

void floppy_set_dma (int addr){
	DMA_BUFFER = addr;
}

unsigned char write_sector(int sectorLBA) {
	floppy_install();
	if (current_drive >= 4)
		return 0;
 
	//! convert LBA sector to CHS
	int head=0, track=0, sector=1;
	lba_to_chs (sectorLBA, &head, &track, &sector);
 
	//! turn motor on and seek to track
	floppy_control_motor (1);
	if (floppy_seek (track, head) != 0)
		return 0;
 
	//! read sector and turn motor off
	write_sector_imp (head, track, sector);
	floppy_control_motor (0);

	return 1;
	//! warning: this is a bit hackish
}