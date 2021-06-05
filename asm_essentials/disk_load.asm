disk_read:
    mov ah, 0x2                         ; tells BIOS it is read disk
    mov dh, 0x1                         ; head number

    mov [SECTORS_READ_REQUESTED], al    ; Store number of sectors requested in a variable so that we can verify whether number of sector
                                        ; requested is same as number of sectors actually read

    int 0x13                            ; BIOS interrupt

    jc disk_error                       ; if disk read failed, then carry bit will be set

    cmp al, [SECTORS_READ_REQUESTED]    ; compare number of sectors requested and number of sectors
                                        ; actually read
    jne sector_error                    ; if(al != SECTORS_READ_REQUESTED) jump to sector_error segment
    ret                                 ; else return

disk_error:
    mov bx, DISK_ERROR_MESSAGE          ; bx = DISK_ERROR_MESSAGE
    call Print
    call Print_nl
    jmp $

sector_error:
    mov bx, SECTOR_ERROR_MESSAGE
    call Print
    call Print_nl
    jmp $

SECTORS_READ_REQUESTED db 0
DISK_ERROR_MESSAGE db "Error reading disk", 0
SECTOR_ERROR_MESSAGE db "Sector error", 0
REBOOTING_MESSAGE db "Rebooting", 0