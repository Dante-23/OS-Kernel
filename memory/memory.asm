memory_region_count:
	db 0
	GLOBAL memory_region_count


detect_memory:                    ; try pushing registers on stack
	mov ax, 0
	mov es, ax
	mov di, 0x5000                ; memory location where memory map will be stored
	mov edx, 0x534D4150
	xor ebx, ebx

loop:
	mov eax, 0xE820
	mov ecx, 24
	int 0x15

	cmp ebx, 0
	je finished

	add di, 24
	inc byte [memory_region_count]
	jmp loop

finished:                            ; try poping registers off stack
	ret