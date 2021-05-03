[bits 32]                                   ; operating in 32 bit protected mode

VIDEO_MEMORY equ 0xb8000                    ; address of video memory. This is memory mapped IO
WHITE_ON_BLACK equ 0x0f                     ; default colour scheme

                                            ; register ebx contains the address of the byte which is to be
                                            ; printed
Print_Protected_Mode:
    pusha
    mov edx, VIDEO_MEMORY                   ; put address of video memory in register edx
    jmp Print_Protected_Mode_Loop

Print_Protected_Mode_Loop:
    mov al, [ebx]                           ; al = main_memory[ebx]
    mov ah, WHITE_ON_BLACK                  ; ah = 0xef

    cmp al, 0                               ; if(al == 0) i.e reached the end of string
    je Print_Protected_Mode_Return          ; return

    mov [edx], ax                           ; else video_memory[edx] = ax
    add ebx, 1                              ; ebx = ebx + 1
    add edx, 2                              ; edx = edx + 1
    jmp Print_Protected_Mode_Loop           ; Recursion

Print_Protected_Mode_Return:
    popa
    ret