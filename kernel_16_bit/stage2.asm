; This is the entry point of our kernel running in 16 bit real mode
; Currently we have assigned 1 sector to it
; First sector is boot sector, second is this.
; From third sector it is out 32 bit kernel

[org 0x7e00]
KERNEL_MEMORY_LOCATION equ 0x9001			; Main memory address where kernel will be loaded

call Clear_Screen

mov bx, HELLO
call Print
call Print_nl

call Load_Kernel

call Enter_Protected_Mode

jmp $

%include "memory/memory.asm"
%include "asm_essentials/essentials.asm"
%include "asm_essentials/print_string.asm"
%include "asm_essentials/disk_load.asm"
%include "asm_essentials/gdt.asm"
%include "asm_essentials/print_protected_mode.asm"
%include "asm_essentials/enter_protected_mode.asm"

[bits 16]
Load_Kernel:
	mov bx, KERNEL_MEMORY_LOCATION			; memory location where kernel will be loaded
	mov al, 0x32								; number of sectors requested to read
	mov cl, 0x3	                         ; sector number from where reading will start
    mov ch, 0x0
	call disk_read							; call disk_load defined in disk_load.asm
	ret

[bits 32]
Begin_PM:
	call KERNEL_MEMORY_LOCATION				; give control to the kernel.
	jmp $									; loop if the control is returned by the kernel

HELLO db "hello www F", 0
times 512-($-$$) db 0