//****************************************************************************
//**
//**	dma.h
//**		8237 ISA Direct Memory Access Controller (DMAC)
//**
//****************************************************************************

//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

/**
*	2 DMACs, 32 bit master & 16bit slave each having 8 channels
*/
#pragma once

#define DMA_MAX_CHANNELS 16
#define DMA_CHANNELS_PER_DMAC 8

/**
*	DMA0 address/count registers
*/
enum DMA0_CHANNEL_IO {

	DMA0_CHAN0_ADDR_REG = 0, //! Thats right, i/o port 0
	DMA0_CHAN0_COUNT_REG = 1,
	DMA0_CHAN1_ADDR_REG = 2,
	DMA0_CHAN1_COUNT_REG = 3,
	DMA0_CHAN2_ADDR_REG = 4,
	DMA0_CHAN2_COUNT_REG = 5,
	DMA0_CHAN3_ADDR_REG = 6,
	DMA0_CHAN3_COUNT_REG = 7,
};

/**
*	Generic DMA0 registers
*/
enum DMA0_IO {

	DMA0_STATUS_REG = 0x08,
	DMA0_COMMAND_REG = 0x08,
	DMA0_REQUEST_REG = 0x09,
	DMA0_CHANMASK_REG = 0x0a,
	DMA0_MODE_REG = 0x0b,
	DMA0_CLEARBYTE_FLIPFLOP_REG = 0x0c,
	DMA0_TEMP_REG = 0x0d,
	DMA0_MASTER_CLEAR_REG = 0x0d,
	DMA0_CLEAR_MASK_REG = 0x0e,
	DMA0_MASK_REG = 0x0f
};

/**
*	DMA1 address/count registers
*/
enum DMA1_CHANNEL_IO {

	DMA1_CHAN4_ADDR_REG = 0xc0,
	DMA1_CHAN4_COUNT_REG = 0xc2,
	DMA1_CHAN5_ADDR_REG = 0xc4,
	DMA1_CHAN5_COUNT_REG = 0xc6,
	DMA1_CHAN6_ADDR_REG = 0xc8,
	DMA1_CHAN6_COUNT_REG = 0xca,
	DMA1_CHAN7_ADDR_REG = 0xcc,
	DMA1_CHAN7_COUNT_REG = 0xce,
};

/**
*	DMA External Page Registers
*/
enum DMA0_PAGE_REG {

	DMA_PAGE_EXTRA0 = 0x80, //! Also diagnostics port
	DMA_PAGE_CHAN2_ADDRBYTE2 = 0x81,
	DMA_PAGE_CHAN3_ADDRBYTE2 = 0x82,
	DMA_PAGE_CHAN1_ADDRBYTE2 = 0x83,
	DMA_PAGE_EXTRA1 = 0x84,
	DMA_PAGE_EXTRA2 = 0x85,
	DMA_PAGE_EXTRA3 = 0x86,
	DMA_PAGE_CHAN6_ADDRBYTE2 = 0x87,
	DMA_PAGE_CHAN7_ADDRBYTE2 = 0x88,
	DMA_PAGE_CHAN5_ADDRBYTE2 = 0x89,
	DMA_PAGE_EXTRA4 = 0x8c,
	DMA_PAGE_EXTRA5 = 0x8d,
	DMA_PAGE_EXTRA6 = 0x8e,
	DMA_PAGE_DRAM_REFRESH = 0x8f //!no longer used in new PCs
};

/**
*	Generic DMA1 registers
*/
enum DMA1_IO {

	DMA1_STATUS_REG = 0xd0,
	DMA1_COMMAND_REG = 0xd0,
	DMA1_REQUEST_REG = 0xd2,
	DMA1_CHANMASK_REG = 0xd4,
	DMA1_MODE_REG = 0xd6,
	DMA1_CLEARBYTE_FLIPFLOP_REG = 0xd8,
	DMA1_INTER_REG = 0xda,
	DMA1_UNMASK_ALL_REG = 0xdc,
	DMA1_MASK_REG = 0xde
};

/**
*	DMA mode bit mask (Non MASK Values can be ORed together)
*/

enum DMA_MODE_REG_MASK {

	DMA_MODE_MASK_SEL = 3,

	DMA_MODE_MASK_TRA = 0xc,
	DMA_MODE_SELF_TEST = 0,
	DMA_MODE_READ_TRANSFER =4,
	DMA_MODE_WRITE_TRANSFER = 8,

	DMA_MODE_MASK_AUTO = 0x10,
	DMA_MODE_MASK_IDEC = 0x20,

	DMA_MODE_MASK = 0xc0,
	DMA_MODE_TRANSFER_ON_DEMAND= 0,
	DMA_MODE_TRANSFER_SINGLE = 0x40,
	DMA_MODE_TRANSFER_BLOCK = 0x80,
	DMA_MODE_TRANSFER_CASCADE = 0xC0
};

enum DMA_CMD_REG_MASK {

	DMA_CMD_MASK_MEMTOMEM = 1,
	DMA_CMD_MASK_CHAN0ADDRHOLD = 2,
	DMA_CMD_MASK_ENABLE = 4,
	DMA_CMD_MASK_TIMING = 8,
	DMA_CMD_MASK_PRIORITY = 0x10,
	DMA_CMD_MASK_WRITESEL = 0x20,
	DMA_CMD_MASK_DREQ = 0x40,
	DMA_CMD_MASK_DACK = 0x80
};

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

//! sets the mode of a channel
void dma_set_mode (unsigned char channel, unsigned char mode);

//! prepares for generic channel read
void dma_set_read (unsigned char channel);

//! prepares for generic channel write
void dma_set_write (unsigned char channel);

//! sets the address of a channel
void dma_set_address(unsigned char channel, unsigned char low, unsigned char high);

//! sets the counter of a channel
void dma_set_count(unsigned char channel, unsigned char low, unsigned char high);

//! masks a channel
void dma_mask_channel (unsigned char channel);

//! unmasks a channel
void dma_unmask_channel (unsigned char channel);

//! resets a flipflop
void dma_reset_flipflop (int dma);

//! reset the dma to defaults
void dma_reset (int dma);

//! sets an external page register
void dma_set_external_page_register (unsigned char reg, unsigned char val);

//! unmasks all registers
void dma_unmask_all (int dma);

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END [dma.h]
//**
//****************************************************************************


//****************************************************************************
//**
//**	dma.cpp
//**		8237 ISA Direct Memory Access Controller (DMAC)
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================
//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//! masks a channel
void dma_mask_channel(unsigned char channel){

	if (channel <= 4)
		outputbyte(DMA0_CHANMASK_REG, (1 << (channel-1)));
	else
		outputbyte(DMA1_CHANMASK_REG, (1 << (channel-5)));
}

//! unmasks a channel
void dma_unmask_channel (unsigned char channel) {

	if (channel <= 4)
		outputbyte(DMA0_CHANMASK_REG, channel);
	else
		outputbyte(DMA1_CHANMASK_REG, channel);
}

//! unmasks all channels
void dma_unmask_all (int dma){

	//! it doesnt matter what is written to this register
	outputbyte(DMA1_UNMASK_ALL_REG, 0xff);
}

//! resets controller to defaults
void dma_reset (int dma){

	//! it doesnt matter what is written to this register
	outputbyte(DMA0_TEMP_REG, 0xff);
}

//! resets flipflop
void dma_reset_flipflop(int dma){

	if (dma < 2)
		return;

	//! it doesnt matter what is written to this register
	outputbyte( (dma==0) ? DMA0_CLEARBYTE_FLIPFLOP_REG : DMA1_CLEARBYTE_FLIPFLOP_REG, 0xff);
}

//! sets the address of a channel
void dma_set_address(unsigned char channel, unsigned char low, unsigned char high) {

	if ( channel > 8 )
		return;

	unsigned short port = 0;
	switch ( channel ) {

		case 0: {port = DMA0_CHAN0_ADDR_REG; break;}
		case 1: {port = DMA0_CHAN1_ADDR_REG; break;}
		case 2: {port = DMA0_CHAN2_ADDR_REG; break;}
		case 3: {port = DMA0_CHAN3_ADDR_REG; break;}
		case 4: {port = DMA1_CHAN4_ADDR_REG; break;}
		case 5: {port = DMA1_CHAN5_ADDR_REG; break;}
		case 6: {port = DMA1_CHAN6_ADDR_REG; break;}
		case 7: {port = DMA1_CHAN7_ADDR_REG; break;}
	}

	outputbyte(port, low);
	outputbyte(port, high);
}

//! sets the counter of a channel
void dma_set_count(unsigned char channel, unsigned char low, unsigned char high) {

	if ( channel > 8 )
		return;

	unsigned short port = 0;
	switch ( channel ) {

		case 0: {port = DMA0_CHAN0_COUNT_REG; break;}
		case 1: {port = DMA0_CHAN1_COUNT_REG; break;}
		case 2: {port = DMA0_CHAN2_COUNT_REG; break;}
		case 3: {port = DMA0_CHAN3_COUNT_REG; break;}
		case 4: {port = DMA1_CHAN4_COUNT_REG; break;}
		case 5: {port = DMA1_CHAN5_COUNT_REG; break;}
		case 6: {port = DMA1_CHAN6_COUNT_REG; break;}
		case 7: {port = DMA1_CHAN7_COUNT_REG; break;}
	}

	outputbyte(port, low);
	outputbyte(port, high);
}

void dma_set_mode (unsigned char channel, unsigned char mode) {

	int dma = (channel < 4) ? 0 : 1;
	int chan = (dma==0) ? channel : channel-4;

	dma_mask_channel (channel);
	outputbyte ( (channel < 4) ? (DMA0_MODE_REG) : DMA1_MODE_REG, chan | (mode) );
	dma_unmask_all ( dma );
}

//! prepares channel for read
void dma_set_read (unsigned char channel) {

	dma_set_mode (channel,	DMA_MODE_READ_TRANSFER | DMA_MODE_TRANSFER_SINGLE);
}

//! prepares channel for write
void dma_set_write (unsigned char channel) {

	dma_set_mode (channel,
		DMA_MODE_WRITE_TRANSFER | DMA_MODE_TRANSFER_SINGLE);
}

//! writes to an external page register
void dma_set_external_page_register (unsigned char reg, unsigned char val) {

	if (reg > 14)
		return;

	unsigned short port = 0;
	switch ( reg ) {

		case 1: {port = DMA_PAGE_CHAN1_ADDRBYTE2; break;}
		case 2: {port = DMA_PAGE_CHAN2_ADDRBYTE2; break;}
		case 3: {port = DMA_PAGE_CHAN3_ADDRBYTE2; break;}
		case 4: {return;}//! nothing should ever write to register 4
		case 5: {port = DMA_PAGE_CHAN5_ADDRBYTE2; break;}
		case 6: {port = DMA_PAGE_CHAN6_ADDRBYTE2; break;}
		case 7: {port = DMA_PAGE_CHAN7_ADDRBYTE2; break;}
	}

	outputbyte(port, val);
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[dma.cpp]
//**
//****************************************************************************
