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
    
print_hex:
	pusha
	mov ah, 0xe
	mov al, 'x'
	int 0x10
	
	mov ax, cx
	and ax, 0xf000
	shr ax, 12
	add al, 48
	call Write_Byte
	
	mov ax, cx
	and ax, 0x0f00
	shr ax, 8
	add al, 48
	call Write_Byte
	
	mov ax, cx
	and ax, 0x00f0
	shr ax, 4
	add al, 48
	call Write_Byte
	
	mov ax, cx
	and ax, 0x000f
	add al, 48
	call Write_Byte
	
	popa
	ret
