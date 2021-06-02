#include "../drivers/IO.c"

unsigned short ADDRESS_REGISTERS[8] = { 0,2,4,6,0xc0,0xc4,0xc8,0xcc };
unsigned short COUNTER_REGISTERS[8] = { 1,3,5,7,0xc2,0xc6,0xca,0xce };
unsigned short PAGE_REGISTERS[8] = { 0x87, 0x83, 0x81, 0x82, 0x8f, 0x8b, 0x89, 0x8a };

unsigned short COMMAND_REGISTERS[2] = { 0x08, 0xd0 };
unsigned short MODE_REGISTERS[2] = { 0x0b, 0xd6 };
unsigned short MASK_REGISTERS[2] = { 0x0f, 0xde };

void dma_set_address(unsigned char channel, unsigned char low, unsigned char high){
    if(channel >= 8 || channel < 0) return;
    unsigned short port = ADDRESS_REGISTERS[channel];
    outputbyte(port, low);
    outputbyte(port, high);
}

void dma_set_count(unsigned char channel, unsigned char low, unsigned char high){
    if(channel >= 8 || channel < 0) return;
    unsigned short port = COUNTER_REGISTERS[channel];
    outputbyte(port, low);
    outputbyte(port, high);
}

void dma_set_external_page_register (unsigned char channel, unsigned char val){
    if(channel >= 8 || channel < 0) return;
    unsigned short port = PAGE_REGISTERS[channel];
    outputbyte(port, val);
}

void enable_controller(unsigned char channel){
    unsigned short port = (channel < 4) ? COMMAND_REGISTERS[0] : COMMAND_REGISTERS[1];
    outputbyte(port, 4);
}

void disable_controller(unsigned char channel){
    unsigned short port = (channel < 4) ? COMMAND_REGISTERS[0] : COMMAND_REGISTERS[1];
    outputbyte(port, 0);
}

void dma_mask_channel(unsigned char channel){
    unsigned short port;
	if (channel <= 4) port = MASK_REGISTERS[0];
	else port = MASK_REGISTERS[1];
    outputbyte(port, (1 << (channel-1)));
}

void dma_unmask_channel(unsigned char channel){
    unsigned short port;
	if (channel <= 4) port = MASK_REGISTERS[0];
	else port = MASK_REGISTERS[1];
    outputbyte(port, channel);
}

void dma_unmask_all(int dma){
    outputbyte(0xdc, 0xff);
}

void dma_set_mode (unsigned char channel, unsigned char mode){
    unsigned short port;
    if(channel < 4) port = MODE_REGISTERS[0];
    else port = MODE_REGISTERS[1];
    dma_mask_channel(channel);
    outputbyte(port, mode);
    dma_unmask_channel(0);
}

void dma_set_read(unsigned char channel){
    dma_set_mode(channel, (4 | 0x40 | 0x10));
}

void dma_set_write(unsigned char channel){
    dma_set_mode(channel, (8 | 0x40 | 0x10));
}

void dma_reset_flipflop(int dma){
	if (dma < 2) return;
	outputbyte( (dma==0) ? 0x0c : 0xd8, 0xff);
}

void dma_reset (int dma){
	outputbyte(0x0d, 0xff);
}