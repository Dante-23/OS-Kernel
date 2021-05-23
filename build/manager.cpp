#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

// gcc -ffreestanding -c kernel.c -o bin/kernel.o
// nasm kernel_entry.asm -f elf -o bin/kernel_entry.o
// nasm interrupts/interrupts.asm -f elf -o bin/interruptsasm.o
// nasm asm_essentials/memory.asm -f elf -o bin/memory.o
// ld -o bin/kernel.bin -Ttext 0x1000 bin/memory.o bin/kernel_entry.o bin/interruptsasm.o bin/kernel.o --oformat binary
// nasm -fbin bootloader.asm -o bin/bootloader.bin
// cat bin/bootloader.bin bin/kernel.bin > bin/os.bin
// qemu-system-i386 -fda bin/os.bin


// x86_64-elf-gcc -ffreestanding -c kernel.c -o bin/kernel.o
// nasm kernel_entry.asm -f elf -o bin/kernel_entry.o
// nasm interrupts/interrupts.asm -f elf -o bin/interruptsasm.o
// nasm asm_essentials/memory.asm -f elf -o bin/memory.o
// x86_64-elf-ld -o bin/kernel.bin -Ttext 0x1000 bin/memory.o bin/kernel_entry.o bin/interruptsasm.o bin/kernel.o --oformat binary
// nasm -fbin bootloader.asm -o bin/bootloader.bin
// cat bin/bootloader.bin bin/kernel.bin > bin/os.bin
// qemu-system-i386 -fda bin/os.bin

int main(){
	vector<string> commands;
	commands.push_back("x86_64-elf-gcc -m32 -ffreestanding -c kernel.c -o bin/kernel.o");
	commands.push_back("nasm asm_essentials/kernel_entry.asm -f elf -o bin/kernel_entry.o");
	commands.push_back("nasm interrupts/interrupts.asm -f elf -o bin/interruptsasm.o");
	commands.push_back("nasm memory/memory.asm -f elf -o bin/memory.o");
	commands.push_back("x86_64-elf-ld -m elf_i386 -s -o bin/kernel.bin -Ttext 0x9001 bin/kernel_entry.o bin/interruptsasm.o bin/memory.o bin/kernel.o --oformat binary");
	commands.push_back("nasm -fbin bootloader.asm -o bin/bootloader.bin");
	commands.push_back("cat bin/bootloader.bin bin/kernel.bin > bin/os.bin");
	commands.push_back("cp bin/os.bin /mnt/c/Users/dante23/Desktop/\"8th Semester OS Project\"/myos");
	
	for(string command: commands){
		system(command.c_str());
	}
	cout << "BUILD COMPLETE" << endl;
	char ch;
	cout << "Run setup? (y/n): ";
	cin >> ch;
	if(ch == 'y' || ch == 'Y'){
		system("/mnt/c/Users/dante23/Desktop/\"8th Semester OS Project\"/myos/qemu/qemu-system-i386 -fda /mnt/c/Users/dante23/Desktop/\"8th Semester OS Project\"/myos/bin/os.bin");
	}
	else{
		cout << "qemu\\qemu-system-i386 -fda os.bin" << endl;
	}
	return 0;
}
