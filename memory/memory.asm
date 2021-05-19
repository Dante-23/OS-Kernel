detect_memory:                    ; try pushing registers on stack
	pusha
	mov ax, 0
	mov es, ax
	mov di, 0x5000                ; memory location where memory map will be stored
	mov edx, 0x534D4150
	xor ebx, ebx

loop_in_memory:
;pusha
;mov bx, testing
;call print
;popa
		mov eax, 0xe820
		mov ecx, 24
		int 0x15

		cmp ebx, 0
		je loop_finished
		;jmp loop_finished

		add di, 24
		inc word [memory]
		jmp loop_in_memory

loop_finished:
		popa                            ; try poping registers off stack
		ret

EnableA20:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

copy:
	pusha
	mov bx, [memory]
	mov word [0x6000], bx
	popa
	ret

memory dw 0x0
testing db "ZZ", 0
location dw 0x6000
