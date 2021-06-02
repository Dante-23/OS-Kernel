#pragma once

#include "../drivers/IO.c"
#include "dma.c"
#include "interrupt_handlers.c"

void floppy_set_dma (int addr);

//! install floppy driver
void floppy_install (int irq);

//! set current working drive
void floppy_set_working_drive (unsigned char drive);

//! get current working drive
unsigned char floppy_get_working_drive ();

//! read a sector
unsigned char* floppy_read_sector (int sectorLBA);

//! converts an LBA address to CHS
void floppy_lba_to_chs (int lba,int *head,int *track,int *sector);


enum floppy_IO {

	floppy_DOR		=	0x3f2,
	floppy_MSR		=	0x3f4,
	floppy_FIFO	=	0x3f5,
	floppy_CTRL	=	0x3f7
};

/**
*	Bits 0-4 of command byte. Please see chapter for additional commands
*/

enum floppy_CMD {

	FDC_CMD_READ_TRACK	=	2,
	FDC_CMD_SPECIFY		=	3,
	FDC_CMD_CHECK_STAT	=	4,
	FDC_CMD_WRITE_SECT	=	5,
	FDC_CMD_READ_SECT	=	6,
	FDC_CMD_CALIBRATE	=	7,
	FDC_CMD_CHECK_INT	=	8,
	FDC_CMD_FORMAT_TRACK=	0xd,
	FDC_CMD_SEEK		=	0xf
};

/**
*	Additional command masks. Can be masked with above commands
*/

enum floppy_CMD_EXT {

	FDC_CMD_EXT_SKIP		=	0x20,	//00100000
	FDC_CMD_EXT_DENSITY		=	0x40,	//01000000
	FDC_CMD_EXT_MULTITRACK	=	0x80	//10000000
};

/*
**	Digital Output Register
*/

enum floppy_DOR_MASK {

	floppy_DOR_MASK_DRIVE0			=	0,	//00000000	= here for completeness sake
	floppy_DOR_MASK_DRIVE1			=	1,	//00000001
	floppy_DOR_MASK_DRIVE2			=	2,	//00000010
	floppy_DOR_MASK_DRIVE3			=	3,	//00000011
	floppy_DOR_MASK_RESET			=	4,	//00000100
	floppy_DOR_MASK_DMA			=	8,	//00001000
	floppy_DOR_MASK_DRIVE0_MOTOR	=	16,	//00010000
	floppy_DOR_MASK_DRIVE1_MOTOR	=	32,	//00100000
	floppy_DOR_MASK_DRIVE2_MOTOR	=	64,	//01000000
	floppy_DOR_MASK_DRIVE3_MOTOR	=	128	//10000000
};

/**
*	Main Status Register
*/

enum floppy_MSR_MASK {

	floppy_MSR_MASK_DRIVE1_POS_MODE	=	1,	//00000001
	floppy_MSR_MASK_DRIVE2_POS_MODE	=	2,	//00000010
	floppy_MSR_MASK_DRIVE3_POS_MODE	=	4,	//00000100
	floppy_MSR_MASK_DRIVE4_POS_MODE	=	8,	//00001000
	floppy_MSR_MASK_BUSY				=	16,	//00010000
	floppy_MSR_MASK_DMA				=	32,	//00100000
	floppy_MSR_MASK_DATAIO				=	64, //01000000
	floppy_MSR_MASK_DATAREG			=	128	//10000000
};

/**
*	Controller Status Port 0
*/

enum floppy_ST0_MASK {

	floppy_ST0_MASK_DRIVE0		=	0,		//00000000	=	for completness sake
	floppy_ST0_MASK_DRIVE1		=	1,		//00000001
	floppy_ST0_MASK_DRIVE2		=	2,		//00000010
	floppy_ST0_MASK_DRIVE3		=	3,		//00000011
	floppy_ST0_MASK_HEADACTIVE	=	4,		//00000100
	floppy_ST0_MASK_NOTREADY	=	8,		//00001000
	floppy_ST0_MASK_UNITCHECK	=	16,		//00010000
	floppy_ST0_MASK_SEEKEND	=	32,		//00100000
	floppy_ST0_MASK_INTCODE	=	64		//11000000
};

/*
** LPYDSK_ST0_MASK_INTCODE types
*/

enum floppy_ST0_INTCODE_TYP {

	floppy_ST0_TYP_NORMAL		=	0,
	floppy_ST0_TYP_ABNORMAL_ERR=	1,
	floppy_ST0_TYP_INVALID_ERR	=	2,
	floppy_ST0_TYP_NOTREADY	=	3
};

/**
*	GAP 3 sizes
*/

enum floppy_GAP3_LENGTH {

	floppy_GAP3_LENGTH_STD = 42,
	floppy_GAP3_LENGTH_5_14= 32,
	floppy_GAP3_LENGTH_3_5= 27
};

/*
**	Formula: 2^sector_number * 128, where ^ denotes "to the power of"
*/

enum floppy_SECTOR_DTL {

	floppy_SECTOR_DTL_128	=	0,
	floppy_SECTOR_DTL_256	=	1,
	floppy_SECTOR_DTL_512	=	2,
	floppy_SECTOR_DTL_1024	=	4
};

/**
*	Constants
*/

//! floppy irq
const int FLOPPY_IRQ = 6;

//! sectors per track
const int FLPY_SECTORS_PER_TRACK = 18;

//! dma tranfer buffer starts here and ends at 0x1000+64k
//! You can change this as needed. It must be below 16MB and in idenitity mapped memory!
int DMA_BUFFER = 0x1000;

//! FDC uses DMA channel 2
const int FDC_DMA_CHANNEL = 2;

//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================

//! used to wait in miliseconds

//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================

//! current working drive. Defaults to 0 which should be fine on most systems
static unsigned char	_CurrentDrive = 0;

//! set when IRQ fires
unsigned char _FloppyDiskIRQ = 0;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

/**
*	DMA Routines.
*/

unsigned char dma_initialize_floppy(unsigned char* buffer, unsigned length){
	// print(int_to_string((int)buffer));
	// print("\n");
   union{
      unsigned char byte[4];//Lo[0], Mid[1], Hi[2]
      unsigned long l;
   }a, c;

   a.l=(unsigned)buffer;
   c.l=(unsigned)length-1;

   //Check for buffer issues
   if ((a.l >> 24) || (c.l >> 16) || (((a.l & 0xffff)+c.l) >> 16)){
#ifdef _DEBUG
      _asm{
         mov      eax, 0x1337
         cli
         hlt
      }
#endif
      return 0;
   }

   dma_reset (1);
   dma_mask_channel( FDC_DMA_CHANNEL );//Mask channel 2
   dma_reset_flipflop ( 1 );//Flipflop reset on DMA 1

   dma_set_address( FDC_DMA_CHANNEL, a.byte[0],a.byte[1]);//Buffer address
   dma_reset_flipflop( 1 );//Flipflop reset on DMA 1

   dma_set_count( FDC_DMA_CHANNEL, c.byte[0],c.byte[1]);//Set count
   dma_set_read ( FDC_DMA_CHANNEL );

   dma_unmask_all( 1 );//Unmask channel 2

   return 1;
}

//! sets DMA base address
void floppy_set_dma (int addr) {

	DMA_BUFFER = addr;
}

/**
*	Basic Controller I/O Routines
*/

//! return fdc status
unsigned char floppy_read_status () {

	//! just return main status register
	return inputbyte (floppy_MSR);
}

//! write to the fdc dor
void floppy_write_dor (unsigned char val ) {

	//! write the digital output register
	outputbyte (floppy_DOR, val);
}

//! send command byte to fdc
void floppy_send_command (unsigned char cmd) {

	//! wait until data register is ready. We send commands to the data register
	for (int i = 0; i < 500; i++ )
		if ( floppy_read_status () & floppy_MSR_MASK_DATAREG )
			return outputbyte (floppy_FIFO, cmd);
}

//! get data from fdc
unsigned char floppy_read_data () {

	//! same as above function but returns data register for reading
	for (int i = 0; i < 500; i++ )
		if ( floppy_read_status () & floppy_MSR_MASK_DATAREG )
			return inputbyte (floppy_FIFO);

	return 0;
}

//! write to the configuation control register
void floppy_write_ccr (unsigned char val) {

	//! write the configuation control
	outputbyte (floppy_CTRL, val);
}

/**
*	Interrupt Handling Routines
*/

//! wait for irq to fire
void floppy_wait_irq () {

	//! wait for irq to fire
	while ( _FloppyDiskIRQ == 0);
	_FloppyDiskIRQ = 0;
}


//!	floppy disk irq handler
void i86_flpy_irq () {

	// _asm add esp, 12
	// _asm pushad
	// _asm cli

	//! irq fired
	_FloppyDiskIRQ = 1;

	//! tell hal we are done
	// interruptdone( FLOPPY_IRQ );

	// _asm sti
	// _asm popad
	// _asm iretd
}

/**
*	Controller Command Routines
*/

//! check interrupt status command
void floppy_check_int (unsigned int* st0, unsigned int* cyl) {

	floppy_send_command (FDC_CMD_CHECK_INT);

	*st0 = floppy_read_data ();
	*cyl = floppy_read_data ();
}

//! turns the current floppy drives motor on/off
void floppy_control_motor (unsigned char b) {

	//! sanity check: invalid drive
	if (_CurrentDrive > 3)
		return;

	unsigned char motor = 0;

	//! select the correct mask based on current drive
	switch (_CurrentDrive) {

		case 0:
			motor = floppy_DOR_MASK_DRIVE0_MOTOR;
			break;
		case 1:
			motor = floppy_DOR_MASK_DRIVE1_MOTOR;
			break;
		case 2:
			motor = floppy_DOR_MASK_DRIVE2_MOTOR;
			break;
		case 3:
			motor = floppy_DOR_MASK_DRIVE3_MOTOR;
			break;
	}

	//! turn on or off the motor of that drive
	if (b)
		floppy_write_dor ((unsigned char)(_CurrentDrive | motor | floppy_DOR_MASK_RESET | floppy_DOR_MASK_DMA));
	else
		floppy_write_dor (floppy_DOR_MASK_RESET);

	//! in all cases; wait a little bit for the motor to spin up/turn off
	sleep (20);
}

//! configure drive
void floppy_drive_data (unsigned char stepr, unsigned char loadt, unsigned char unloadt, unsigned char dma ) {

	unsigned char data = 0;

	//! send command
	floppy_send_command (FDC_CMD_SPECIFY);
	data = ( (stepr & 0xf) << 4) | (unloadt & 0xf);
		floppy_send_command (data);
	data = (( loadt << 1 ) | ( (dma) ? 0 : 1 ) );
		floppy_send_command (data);
}

//! calibrates the drive
int floppy_calibrate (unsigned char drive) {

	unsigned int st0, cyl;

	if (drive >= 4)
		return -2;

	//! turn on the motor
	floppy_control_motor (1);

	for (int i = 0; i < 10; i++) {

		//! send command
		floppy_send_command ( FDC_CMD_CALIBRATE );
		floppy_send_command ( drive );
		floppy_wait_irq ();
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

//! disable controller
void floppy_disable_controller () {

	floppy_write_dor (0);
}

//! enable controller
void floppy_enable_controller () {

	floppy_write_dor ( floppy_DOR_MASK_RESET | floppy_DOR_MASK_DMA);
}

//! reset controller
void floppy_reset () {

	unsigned int st0, cyl;

	//! reset the controller
	floppy_disable_controller ();
	floppy_enable_controller ();
	floppy_wait_irq ();

	//! send CHECK_INT/SENSE INTERRUPT command to all drives
	for (int i=0; i<4; i++)
		floppy_check_int (&st0,&cyl);

	//! transfer speed 500kb/s
	floppy_write_ccr (0);

	//! pass mechanical drive info. steprate=3ms, unload time=240ms, load time=16ms
	floppy_drive_data (3,16,240,1);

	//! calibrate the disk
	floppy_calibrate ( _CurrentDrive );
}

//! read a sector
void floppy_read_sector_imp (unsigned char head, unsigned char track, unsigned char sector) {

	unsigned int st0, cyl;

	//! initialize DMA
	dma_initialize_floppy ((unsigned char*) DMA_BUFFER, 512 );

	//! set the DMA for read transfer
	dma_set_read ( FDC_DMA_CHANNEL );

	//! read in a sector
	floppy_send_command (
				FDC_CMD_READ_SECT | FDC_CMD_EXT_MULTITRACK | FDC_CMD_EXT_SKIP | FDC_CMD_EXT_DENSITY);
	floppy_send_command ( head << 2 | _CurrentDrive );
	floppy_send_command ( track);
	floppy_send_command ( head);
	floppy_send_command ( sector);
	floppy_send_command ( floppy_SECTOR_DTL_512 );
	floppy_send_command ( ( ( sector + 1 ) >= FLPY_SECTORS_PER_TRACK ) ? FLPY_SECTORS_PER_TRACK : sector + 1 );
	floppy_send_command ( floppy_GAP3_LENGTH_3_5 );
	floppy_send_command ( 0xff );

	//! wait for irq
	floppy_wait_irq ();

	//! read status info
	for (int j=0; j<7; j++)
		floppy_read_data ();

	//! let FDC know we handled interrupt
	floppy_check_int (&st0,&cyl);
}

//! seek to given track/cylinder
int floppy_seek ( unsigned char cyl, unsigned char head ) {

	unsigned int st0, cyl0;

	if (_CurrentDrive >= 4)
		return -1;

	for (int i = 0; i < 10; i++ ) {

		//! send the command
		floppy_send_command (FDC_CMD_SEEK);
		floppy_send_command ( (head) << 2 | _CurrentDrive);
		floppy_send_command (cyl);

		//! wait for the results phase IRQ
		floppy_wait_irq ();
		floppy_check_int (&st0,&cyl0);

		//! found the cylinder?
		if ( cyl0 == cyl)
			return 0;
	}

	return -1;
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//! convert LBA to CHS
void floppy_lba_to_chs (int lba,int *head,int *track,int *sector) {

   *head = ( lba % ( FLPY_SECTORS_PER_TRACK * 2 ) ) / ( FLPY_SECTORS_PER_TRACK );
   *track = lba / ( FLPY_SECTORS_PER_TRACK * 2 );
   *sector = lba % FLPY_SECTORS_PER_TRACK + 1;
}

//! install floppy driver
void floppy_install (int irq) {

	//! install irq handler
	// setvect (irq, i86_flpy_irq);

	//! reset the fdc
	floppy_reset ();

	//! set drive information
	floppy_drive_data (13, 1, 0xf, 1);
}

//! set current working drive
void floppy_set_working_drive (unsigned char drive) {

	if (drive < 4)
		_CurrentDrive = drive;
}

//! get current working drive
unsigned char floppy_get_working_drive () {

	return _CurrentDrive;
}

//! read a sector
unsigned char* floppy_read_sector (int sectorLBA) {
	floppy_install(38);
	if (_CurrentDrive >= 4)
		return 0;

	//! convert LBA sector to CHS
	int head=0, track=0, sector=1;
	floppy_lba_to_chs (sectorLBA, &head, &track, &sector);

	//! turn motor on and seek to track
	// print(int_to_string(head));
	// print(" ");
	// print(int_to_string(track));
	// print(" ");
	// print(int_to_string(sector));
	// print("\n");
	// return (unsigned char*)DMA_BUFFER;
	floppy_control_motor (1);
	if (floppy_seek ((unsigned char)track, (unsigned char)head) != 0)
		return 0;

	//! read sector and turn motor off
	floppy_read_sector_imp ((unsigned char)head, (unsigned char)track, (unsigned char)sector);
	floppy_control_motor (0);

	//! warning: this is a bit hackish
	return (unsigned char*) DMA_BUFFER;
}

void floppy_write_sector_imp (unsigned char head, unsigned char track, unsigned char sector) {

	unsigned int st0, cyl;

	//! initialize DMA
	dma_initialize_floppy ((unsigned char*) DMA_BUFFER, 512 );

	//! set the DMA for write transfer
	dma_set_write ( FDC_DMA_CHANNEL );

	//! write on a sector
	floppy_send_command (
				FDC_CMD_WRITE_SECT | FDC_CMD_EXT_MULTITRACK | FDC_CMD_EXT_DENSITY);
	floppy_send_command ( head << 2 | _CurrentDrive );
	floppy_send_command ( track);
	floppy_send_command ( head);
	floppy_send_command ( sector);
	floppy_send_command ( floppy_SECTOR_DTL_512 );
	floppy_send_command ( ( ( sector + 1 ) >= FLPY_SECTORS_PER_TRACK ) ? FLPY_SECTORS_PER_TRACK : sector + 1 );
	floppy_send_command ( floppy_GAP3_LENGTH_3_5 );
	floppy_send_command ( 0xff );

	//! wait for irq
	floppy_wait_irq ();

	//! read status info
	for (int j=0; j<7; j++)
		floppy_read_data ();

	//! let FDC know we handled interrupt
	floppy_check_int (&st0,&cyl);
}

unsigned char floppy_write_sector (int sectorLBA) {
	floppy_install(38);
	if (_CurrentDrive >= 4)
		return 0;

	//! convert LBA sector to CHS
	int head=0, track=0, sector=1;
	floppy_lba_to_chs (sectorLBA, &head, &track, &sector);

	//! turn motor on and seek to track
	// print(int_to_string(head));
	// print(" ");
	// print(int_to_string(track));
	// print(" ");
	// print(int_to_string(sector));
	// print("\n");
	// return (unsigned char*)DMA_BUFFER;
	floppy_control_motor (1);
	if (floppy_seek ((unsigned char)track, (unsigned char)head) != 0)
		return 0;

	//! read sector and turn motor off
	floppy_write_sector_imp ((unsigned char)head, (unsigned char)track, (unsigned char)sector);
	floppy_control_motor (0);

	//! warning: this is a bit hackish
	return 1;
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[floppy.cpp]
//**
//****************************************************************************
