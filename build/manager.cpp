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

int main(){
	vector<string> commands;
	commands.push_back("gcc -ffreestanding -c kernel.c -o bin/kernel.o");
	commands.push_back("nasm asm_essentials/kernel_entry.asm -f elf -o bin/kernel_entry.o");
	commands.push_back("nasm interrupts/interrupts.asm -f elf -o bin/interruptsasm.o");
	commands.push_back("nasm memory/memory.asm -f elf -o bin/memory.o");
	commands.push_back("ld -o bin/kernel.bin -Ttext 0x1000 bin/kernel_entry.o bin/interruptsasm.o bin/memory.o bin/kernel.o --oformat binary");
	commands.push_back("nasm -fbin bootloader.asm -o bin/bootloader.bin");
	commands.push_back("cat bin/bootloader.bin bin/kernel.bin > bin/os.bin");
	
	for(string command: commands){
		system(command.c_str());
	}
	cout << "BUILD COMPLETE" << endl;
	char ch;
	cout << "Run setup? (y/n): ";
	cin >> ch;
	if(ch == 'y' || ch == 'Y'){
		system("qemu-system-i386 -fda bin/os.bin");
	}
	return 0;
}
