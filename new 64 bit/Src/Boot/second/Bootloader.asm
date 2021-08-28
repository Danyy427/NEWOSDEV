[ORG 0x7C00]

[BITS 16]

global bootload

; Loads a second stage bootloader which loads the kernel

jmp short bootload
nop

FAT32_OSName				db		"TaraOS"
FAT32_BytesPerSector		dw		512
FAT32_SectorsPerCluster		db		8
FAT32_ReservedSectors		dw		0
FAT32_NumFATs				db		0   
FAT32_RootEntries			dw		0
FAT32_TotalSectors			dw		0
FAT32_MediaType				db		0
FAT32_SectorsPerFat			dw		0
FAT32_SectorsPerTrack		dw		0
FAT32_HeadsPerCylinder		dw		2
FAT32_HiddenSectors			dd 		0
FAT32_LargeTotalSectors		dd 		0

; The extension block
FAT32_SectorsPerFAT32		dd 		0
FAT32_Flags					dw		0
FAT32_Version				dw		0
FAT32_RootDirStart			dd 		0
FAT32_FSInfoSector			dw		0
FAT32_BackupBootSector		dw		0

; Reserved 
FAT32_Reserved0				dd		0 	;FirstDataSector
FAT32_Reserved1				dd		0 	;ReadCluster
FAT32_Reserved2				dd 		0 	;ReadCluster

FAT32_PhysicalDriveNum		db		0
FAT32_Reserved3				db		0
FAT32_BootSignature			db		0
FAT32_VolumeSerial			dd 		0
FAT32_VolumeLabel			db		"TaraOS DISK" ; 11 bytes
FAT32_FSName				db		"FAT32   " ; 8 bytes

bootload:


    mov [Drive], dl ; secure the drive number provided by BIOS

	mov sp, 0x7c00
	mov bp, sp

	mov bx, Welcome
	call printStr
	call printNewline

	mov cx, 16 ; block count
	mov bx, 0x7e00 ; offset
	mov ax, 2; lba
	mov dl, [Drive] ; drive
	call ReadDisk ; go

	mov dl, [Drive]
	jmp 0x7e00

    jmp $

printStr:
	push ax
	push bx
	mov ah, 0x0E
.loop:
	mov al, [bx]
	cmp al, 0x00
	je .return
	int 0x10
	inc bx
	jmp .loop
.return:
	pop bx
	pop ax
	ret


printNewline:
	push ax
	mov ah, 0x0E
	mov al, 10 ; newline
	int 0x10
	mov al, 13 ; carriage return
	int 0x10
	pop ax
	ret


printHexWord:
	push ax
	push bx
	push cx
	push dx
	mov cx, 4 ; we are doing a word so it is 2 bytes, 4 hex numbers
	mov ah, 0x0E ; print function in int 0x10

.loopBl:
	rol bx, 4 ; rotate the hex numbers by one so that we can go through each number
	push bx ; preserve the rotated bx for later use
	and bx, 0x000F ; leave only the last digit
	cmp bx, 0x09 ; check if it is less than or equal to 9
	jle .zero_nine ; if it is go here
	jmp .a_f ; else go here
	
.zero_nine:
	add bl, 48 ; if it is between 0 and 9 add 48 to it to get the ascii value
	mov al, bl ; move it to al
	int 0x10 ; print it
	dec cx
	cmp cx, 0 ; compare cx to 0
	pop bx
	je .done ; if it is zero we are done
	jmp .loopBl ; else go back

.a_f:
	add bl, 55 ; if it is between a and f add 55 to it to get the ascii value
	mov al, bl ; move it to al
	int 0x10 ; print it
	dec cx
	cmp cx, 0 ; compare cx to 0
	pop bx
	je .done ; if it is zero we are done
	jmp .loopBl ; else go back

.done:
	pop dx ; get everything back
	pop cx
	pop bx
	pop ax
	ret ; and we are done


; cx : number of blocks to transfer
; dx : segment for loading
; bx : offset for loading
; di : lba to start from
; dl : drive number
ReadDisk: 
	pusha
	; lets start by checking if your extensions are supported

	mov [blocc], cx
	mov [off], bx
	mov [lba], ax

	mov ah, 0x41 ; ah = 41h
	mov bx, 0x55AA ; bx = 55AAh

	int 0x13 ; go

	jc notSupported ; if there is carry, extensions are not supported

	mov ax, 0x0000
	mov ah, 0x42
	mov si, DAPACK

	int 0x13

	jc diskError

	jmp done

done:
	popa
	ret

diskError:
	mov bx, diskErrorStr
	call printStr
	call printNewline

	xor bx, bx
	mov bl, al
	call printHexWord
	call printNewline

	jmp done

notSupported:
	mov bx, ax
	call printHexWord
	call printNewline

	mov bx, extensionsNotSupportedStr
	call printStr
	call printNewline

	jmp fallback

fallback:
	jmp done

DAPACK:
	db 0x10
	db 0x00
blocc:	dw 0x00
off:	dw 0x00
segm:	dw 0x00
lba:	dd 0x00
		dd 0x00


diskErrorStr: db "Error while loading the rest of the kernel", 0
extensionsNotSupportedStr: db "INT 13 extensions not supported", 0
Welcome: db "VBR", 0

Drive: resb 1


times 510 - ($-$$) db 0x00

; boot signature
dw 0xAA55
