Read_Byte:
	mov ah, 0x00
	int 0x16
	ret

Write_Byte:
	pusha
	mov ah, 0x0e
	int 0x10
	popa
	ret

Clear_Screen:
	pusha
	mov ah, 0x0
	mov al, 0x3
	int 0x10
	popa
	ret

Reboot:
	int 0x19
