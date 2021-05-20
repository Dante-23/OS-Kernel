[bits 16]
Enter_Protected_Mode:
	call detect_memory
	call copy
	mov cx, [0x6000]
	call print_hex
	;call function
	;mov cx, [memory]
	;call print_hex
	call EnableA20
    cli                                 ; disable interrupts
    lgdt [gdt_descriptor]               ; load the gdt descriptor
    mov eax, cr0                        ; cr0 = 1
    or eax, 0x1                         ; cr0 is a flag which needs to be set when in protected mode
    mov cr0, eax
    jmp CODE_SEG:init_pm                ; Jump to address gdt[CODE_SEG] + init_pm

; CODE_SEG is the index in the gdt.

EnableA20:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

; operating in 32 bit protected mode
[bits 32]
init_pm:
    mov ax, DATA_SEG                    ; Again DATA_SEG is the index in the gdt
    mov ds, ax                          ; Actual address of DATA_SEG is
    mov ss, ax                          ; Base address of gdt + DATA_SEG
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call Begin_PM

