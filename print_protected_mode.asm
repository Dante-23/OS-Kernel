[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

Print_Protected_Mode:
    pusha
    mov edx, VIDEO_MEMORY
    jmp Print_Protected_Mode_Loop

Print_Protected_Mode_Loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK

    cmp al, 0
    je Print_Protected_Mode_Return

    mov [edx], ax
    add ebx, 1
    add edx, 2
    jmp Print_Protected_Mode_Loop

Print_Protected_Mode_Return:
    popa
    ret