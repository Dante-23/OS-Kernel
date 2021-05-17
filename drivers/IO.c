#pragma once

void outputbyte(unsigned short port, unsigned char val){
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inputbyte(unsigned short port){
    unsigned char returnValue;
    asm volatile("inb %1, %0" : "=a"(returnValue) : "Nd"(port));
    return returnValue;
}
