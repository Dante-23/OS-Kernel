[bits 32]
[extern _main] ; Define calling point. Must have same name as kernel.c 'main' function
call _main ; Calls the C function. The linker will know where it is placed in memory
jmp $