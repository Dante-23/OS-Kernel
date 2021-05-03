[org 0x7c00]
KERNEL_MEMORY_LOCATION equ 0x1000			; Main memory address where kernel will be loaded

mov bp, 0x9000
mov sp, bp

mov bx, REAL_MODE_MESSAGE
call Print
call Print_nl

mov bx, KERNEL_LOAD_MESSAGE
call Print
call Print_nl

call Load_Kernel

mov bx, KERNEL_LOAD_SUCCESS_MESSAGE
call Print
call Print_nl

mov bx, PROTECTED_MODE_MESSAGE
call Print
call Print_nl

call Enter_Protected_Mode
jmp $										; jmp to current address... Same as while(1);

; Includes start

%include "essentials.asm"
%include "print_string.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "print_protected_mode.asm"
%include "enter_protected_mode.asm"

; Includes end

[bits 16]
Load_Kernel:
	mov bx, KERNEL_MEMORY_LOCATION			; memory location where kernel will be loaded
	mov al, 0x2								; number of sectors requested to read
	call disk_read							; call disk_load defined in disk_load.asm
	ret

[bits 32]
Begin_PM:
	mov ebx, PROTECTED_MODE_SUCCESS_MESSAGE
	call Print_Protected_Mode
	; call Print_nl
	jmp $

; CONTANTS and MESSAGES START

DISK_LOADING_MESSAGE db "Loading from disk", 0
REAL_MODE_MESSAGE db "Loading in real mode", 0
KERNEL_LOAD_MESSAGE db "Loading kernel in real mode", 0
KERNEL_LOAD_SUCCESS_MESSAGE db "Successfully loaded kernel", 0
PROTECTED_MODE_MESSAGE db "Going to protected mode", 0
PROTECTED_MODE_SUCCESS_MESSAGE db "Successfully entered protected mode", 0

; CONTANTS and MESSAGES END

times 510-($-$$) db 0
dw 0xaa55
