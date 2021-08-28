[ORG 0x7E00]

[BITS 16]

global afterload

; This will load the kernel
; Set determine and set a video mode
; move the kernel to a secure position for later use
; will switch to 32 bit, set a good GDT
; set up identity paging of the first MiB and set up a higher half kernel
; 
afterload:

	mov [Drive], dl

	mov bx, Welcome
	call printStr
	call printNewline








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
	mov [lba], di

	mov ah, 0x41 ; ah = 41h
	mov bx, 0x55AA ; bx = 55AAh

	int 0x13 ; go

	jc notSupported ; if there is carry, extensions are not supported

	mov ax, 0x0000
	mov ah, 0x42
	mov si, DAPACK

	int 0x10

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
Welcome: db "We reached the second stage", 0


Drive: resb 1

times 8192 - ($-$$) db 0
