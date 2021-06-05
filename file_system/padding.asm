; 1 = boot Loader / Reserved Sector
; 2 - 11 = FAT
; 12 - 16 = Root directory table
; 17 = stage2 kernel
; 18 - 
; We have to pad 15 sectors which is 7680 bytes


times 5120 - ($-$$) db 0

; The following are entries on root directory table

boot_loader db "boot.bin   ", 0
boot_loader_size dw 1
boot_loader_start_sector dw 0

fat db "fat.ft     ", 0
fat_size dw 2
fat_start_sector dw 1

rdt db "rdt.rt     ", 0
rdt_size dw 2
rdt_start_sector dw 3

stage2_kernel db "stage2.bin ", 0
stage2_kernel_size dw 1
stage2_kernel_start_sector dw 5

kernel db "kernel.bin ", 0
kernel_size dw 42
kernel_start_sector dw 6

times 7680 - ($-$$) db 0
