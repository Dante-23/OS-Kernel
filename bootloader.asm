[org 0x7c00]
STAGE2_LOAD_LOCATION equ 0x7e00

	ignore db "ig", 0                               ; Ignore first 3 bytes
	bpbOEM			db "NANO-OS ", 0				; This member must be exactally 8 bytes. It is just; the name of your OS :) Everything else remains the same.

	bpbBytesPerSector  	dw 512
	bpbSectorsPerCluster 	db 1
	bpbReservedSectors	dw 1
	bpbNumberOfFATs	    db 2
	bpbRootEntries 	    dw 224
	bpbTotalSectors 	    dw 2880
	bpbMedia 	            db 0xF0
	bpbSectorsPerFAT 	    dw 10
	bpbSectorsPerTrack	 	dw 18
	bpbHeadsPerCylinder 	dw 2
	bpbHiddenSectors 	    dd 0
	bpbTotalSectorsBig      dd 0

	bsDriveNumber 	        db 0
	bsUnused 	            db 0
	bsExtBootSignature 	db 0x29
	bsSerialNumber	        dd 0xa0a1a2a3
	bsVolumeLabel 	        db "MOS FLOPPY ", 0
	bsFileSystem 	        db "FAT12   ", 0

	mov bp, 0x9000								; Update stack pointer and base pointer. Not Needed
	mov sp, bp

	; mov bx, KERNEL_LOAD_SUCCESS_MESSAGE
	; call Print
	; call Print_nl
	
	call Load_Kernel

	call STAGE2_LOAD_LOCATION

	; mov bx, STAGE2_LOAD_LOCATION
	; ; add bx, STAGE2_LOAD_LOCATION
	; call Print
	; call Print_nl
	 
	; call Enter_Protected_Mode
	jmp $										; jmp to current address... Same as while(1);

; Includes start

; %include "memory/memory.asm"
%include "asm_essentials/essentials.asm"
%include "asm_essentials/print_string.asm"
%include "asm_essentials/disk_load.asm"
; %include "asm_essentials/gdt.asm"
; %include "asm_essentials/print_protected_mode.asm"
; %include "asm_essentials/enter_protected_mode.asm"

; Includes end

[bits 16]
Load_Kernel:
	mov bx, STAGE2_LOAD_LOCATION			; memory location where kernel will be loaded
	mov al, 0x1								; number of sectors requested to read
	mov cl, 0x2	                         ; sector number from where reading will start
    mov ch, 0x0
	call disk_read							; call disk_load defined in disk_load.asm
	ret

; [bits 32]
; Begin_PM:
; 	call KERNEL_MEMORY_LOCATION				; give control to the kernel.
; 	jmp $									; loop if the control is returned by the kernel

; CONTANTS and MESSAGES START

; DISK_LOADING_MESSAGE db "Loading from disk", 0
; REAL_MODE_MESSAGE db "Loading in real mode", 0
;KERNEL_LOAD_MESSAGE db "Loading kernel in real mode", 0
; KERNEL_LOAD_SUCCESS_MESSAGE db "Successfully loaded kernel", 0
;PROTECTED_MODE_MESSAGE db "Going to protected mode", 0
; PROTECTED_MODE_SUCCESS_MESSAGE db "Successfully entered protected mode", 0
; CONTANTS and MESSAGES END

times 510-($-$$) db 0
dw 0xaa55

; ; STAGE2:
; 	mov bx, STAGE2_MESSAGE
; 	call Print
; 	call Print_nl
; 	ret

; STAGE2_MESSAGE db "hello", 0