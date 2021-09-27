[ORG 0x600]
[BITS 16]

global boot

; Loads VBR and sets up the system

boot: ; 0x7c00

	cld

	xor ax, ax       ; Set the segments to 0
	mov es, ax
	mov ds, ax
	mov gs, ax
	mov fs, ax
	
	
	mov cx, 0x0100   ; Move this mbr to 0x600
    mov si, 0x7C00            
    mov di, 0x0600            
    rep movsw   

	jmp 0x00:Clear ; finally get rid of unknown code segment

Clear: ; we are good to start
	

	; we know nothing except for the drive number we are in
	mov [Drive]	, dl ; secure the drive number provided by BIOS

	mov sp, 0x9000
	mov bp, sp

	
	
	mov cx, 1 ; block count
	mov bx, 0x7c00 ; offset
	mov ax, 1 ; lba
	mov dl, [Drive] ; drive
	call ReadDisk ; go

	mov dl, [Drive]
	
	mov bx, Welcome
	call printStr
	call printNewline


	jmp 0x7c00

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
	; TODO: ADD INT 0X13 AH 02
	jmp done

DAPACK: db	0x10
	    db	0
blocc:	dw	1
off:	dw	0x7C00
segm:	dw	0
lba:	dd	0x1
	    dd	0	



; strings and data
diskErrorStr: db "Error loading the kernel", 0
extensionsNotSupportedStr: db "INT 13 not supported", 0
Welcome: db "MBR",0
; bss
Drive: resb 1

; padding
times 446 - ($-$$) db 0x00

partition1: 
	db 0x80          ; Active
	db 0             ; CHS start
	db 0             
	db 0             
	db 0			 ; Type
	db 0             ; CHS end
	db 0
	db 0
	dd 1             ; LBA start
	dd 0x200         ; Sector count
partition2: 
	db 0          ; Active
	db 0             ; CHS start
	db 0             
	db 0             
	db 0			 ; Type
	db 0             ; CHS end
	db 0
	db 0
	dd 0             ; LBA start
	dd 0             ; Sector count
partition3: 
	db 0          ; Active
	db 0             ; CHS start
	db 0             
	db 0             
	db 0			 ; Type
	db 0             ; CHS end
	db 0
	db 0
	dd 0             ; LBA start
	dd 0             ; Sector count
partition4: 
	db 0          ; Active
	db 0             ; CHS start
	db 0             
	db 0             
	db 0			 ; Type
	db 0             ; CHS end
	db 0
	db 0
	dd 0             ; LBA start
	dd 0             ; Sector count



; boot signature
dw 0xAA55


; 0x7C00