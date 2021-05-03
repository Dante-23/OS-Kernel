Print:
	mov ah, 0x0e
	mov al, [bx]
	cmp al, 0
	je Return
	int 0x10
	add bx, 1
	jmp Print

Print_nl:
    pusha
    
    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10
    
    popa
    ret

Return:
    ret