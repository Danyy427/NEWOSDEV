[ORG 0x7E00]

[BITS 16]

global afterload

; This will load the kernel
; Set determine and set a video mode
; move the kernel to a secure position for later use
; will switch to 32 bit, set a good GDT
; set up identity paging of the first MiB and set up a higher half kernel
; 


;; BTW Here is the complete memory map of the things we know so far
;
;   0x600-0x800 The MBR
;   0x7c00-0x7e00 The VBR
;   0x7e00-0x9e00 The second stage loader <- This is us
;	0x7c00 and growing downwards is our stack
;   0x9f00 e820 memory map location. This adress contains the number of entried in a dword
;	0x10000 to be loaded kernel image will be placed here
;   
;
;
;
;


afterload:

	mov [Drive], dl

	mov bx, Welcome
	call printStr
	call printNewline

	call enableA20 ; ax is modified, 1 for enabled, 0 for disabled

	call LoadKernel ; Load Kernel to 0x10000

	mov ax, 1920
	mov bx, 1080
	mov cl, 32
	call setVideoMode ; set the video mode

	call do_e820 ; Get Memory Map
	; Make a kernel call structure, done

	cli

	lgdt [gdtr]   ; Set GDT
	; Go 32 Bit

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	
	jmp 0x08:code32

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


LoadKernel:
	pushf     ; push every register to stack
	push ax
	push bx
	push cx
	push dx
	push di
	push si

	mov cx, 0 ; set the block counter

	mov word [segmLoop], 0x1000

	mov word [lbaLoop], 18 ; start from lba 0 for debug

kloop:

	push cx ; push the block counter to stack

	mov dl, [Drive] ; dl is the drive number
	mov di, 0x0000 ; we will only use the segment 
	mov bx, [lbaLoop] ; bx is the lba to start from

	push ax 
	mov ax, [segmLoop] ; es is the segment 
	mov es, ax
	pop ax

	mov cx, 1 ; read one block

	call ReadDisk ; read it

	pop cx ; take back the counter

	jc kloopError ; if there was an error in reading the disk go to kloop error
	; it means that we are done with the reading and couldn't find another block

	add cx, 1 ; increase the counter
	add word [lbaLoop], 1 ; move to the next lba

	add word [segmLoop], 0x20 ; increase the segment by 0x20 or 512 bytes

	jmp kloop ; loop

doneLoadingKernel:
	pop si    ; pop them back
	pop di
	pop dx
	pop cx
	pop bx
	pop ax
	popf
	ret

kloopError:
	mov bx, kernelLoadedOrTried
	call printStr

	call printNewline

	mov bx, cx
	call printHexWord

	call printNewline
	
	pop si    ; pop them back
	pop di
	pop dx
	pop cx
	pop bx
	pop ax
	popf
	ret


; cx : number of blocks to transfer
; di : offset for loading
; bx : lba to start from
; dl : drive number
; es : segment for loading
ReadDisk: 
	pusha
	; lets start by checking if your extensions are supported


	mov [blocc], cx
	mov [off], di
	mov [lba], bx
	mov [segm], es

	mov ah, 0x41 ; ah = 41h
	mov bx, 0x55AA ; bx = 55AAh

	int 0x13 ; go

	jc notSupported ; if there is carry, extensions are not supported

	mov ax, 0x0000
	mov ds, ax
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

	stc
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


argWidth: resw 1 ; we wukk have 3 arguments
argHeight: resw 1 ; lets store them here for the sake of completeness
argBpp: resb 1

; ax = width
; bx = height
; cl = bpp
setVideoMode:
	
	push ax
	push bx
	push cx
	push dx
	push di
	push si
	push ds

	mov [argWidth], ax ; store the width
	mov [argHeight], bx ; store the height
	mov [argBpp], cl ; store the bits per pixel

	xor ax, ax ; make es 0
	mov es, ax

	mov di, vbeinfoblock ; make di the address of the vbe block

	mov ax, 0x4f00
	int 0x10           ; get the vbe info

	cmp ax, 0x004f 
	jne vbeblockfailed  ; if we failed with the info just give up

	mov ax, [vbeinfoblock+0x0E+0x02] ; this is the segment
	mov es, ax

	mov si, [vbeinfoblock+0x0E] ; this is the offset

vbeloop:
	
	;mov bx, [ds:si] ; write the mode numbers on screem
	;call printHexWord

	;mov bx, space ; print a space
	;call printStr

	cmp word [ds:si], 0xFFFF ; has the list terminated
	je vbedonewithout              ; if it is then end 

	call getModeInfo        ; if it hasn't terminated get the info of the mode
	

	push ax
	mov ax, word [width]
	cmp ax, word [argWidth]  ; compare the mode we want with the arguments
	pop ax
	jne nextMode                       ; if they are not equal just go to tbe next mode

	push ax
	mov ax, word [height]
	cmp ax, word [argHeight]  ; compare the mode we want with the arguments
	pop ax
	jne nextMode

	push ax
	mov al, byte [bpp]
	cmp al, byte [argBpp]  ; compare the mode we want with the arguments
	pop ax
	jne nextMode
	
	; if we have found such mode set it as our video mode

	xor ax, ax
	mov es, ax
	mov di, ax

	mov bx, word [ds:si]
	mov ax, 0x4f02
	
	int 0x10 ; go
	
	jmp vbedone                        ; here all of the arguments match and the mode number is [ds:si] so just end

nextMode:
	add si, 2                          ; increase the si by the and go to the next mdoe
	jmp vbeloop

vbeblockfailed:
	mov bx, vbeblockfail              ; print a fail message
	call printStr

	call printNewline
	pop ds
	pop si
	pop di                  ; wrap everything up
	pop dx
	pop cx
	pop bx 
	pop ax
	stc
	ret

vbedone:
	pop ds
	pop si
	pop di
	pop dx
	pop cx
	pop bx 
	pop ax
	clc
	ret

vbedonewithout:

	mov bx, videoModeNotFoundError
	call printStr

	pop ds
	pop si
	pop di
	pop dx
	pop cx
	pop bx 
	pop ax
	stc
	ret

; gets the information of video mode and puts the ones that are important to VBEINFOFORKERNEL
getModeInfo:
	pushf
	pusha

	; here we have ds:si as the pointer to the video mode number
	mov cx, [ds:si] ; move the video mode number to cx for the interrupt
	or cx, 0x4000

	xor ax, ax
	mov es, ax ; make es 0

	mov di, modeinfoblock ; address of the info block

	mov ax, 0x4f01 ; get vbe mode info function

	int 0x10 ; get the video mode info

	mov ax, word [modeinfoblock+0x10]
	mov word [bpsl], ax
	mov ax, word [modeinfoblock+0x12]
	mov word [width], ax
	mov ax, word [modeinfoblock+0x14]
	mov word [height], ax
	mov al, byte [modeinfoblock+0x19]
	mov byte [bpp], al
	mov al, byte [modeinfoblock+0x1F]
	mov byte [rmasks], al
	mov al, byte [modeinfoblock+0x20]
	mov byte [rmaskp], al
	mov al, byte [modeinfoblock+0x21]
	mov byte [gmasks], al
	mov al, byte [modeinfoblock+0x22]
	mov byte [gmaskp], al
	mov al, byte [modeinfoblock+0x23]
	mov byte [bmasks], al
	mov al, byte [modeinfoblock+0x24]
	mov byte [bmaskp], al
	mov ax, word [modeinfoblock+0x28]
	mov [physicalFramebuffersegm], ax
	mov ax,word [modeinfoblock+0x2A]
	mov [physicalFramebufferoffs], ax


	popa
	popf
	ret



; gets the memory map and puts it ti 0x9f00
mmap_ent equ 0x9f00             ; the number of entries will be stored at 0x8000
do_e820:
        mov di, 0x9f04          ; Set di to 0x8004. Otherwise this code will get stuck in `int 0x15` after some entries are fetched 
	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes
	int 0x15
	jc short .failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
	jc short .e820f		; carry set means "end of list already reached"
	mov edx, 0x0534D4150	; repair potentially trashed register
.jmpin:
	jcxz .skipent		; skip any 0 length entries
	cmp cl, 20		; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skipent		; if length uint64_t is 0, skip entry
	inc bp			; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx		; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
	mov [mmap_ent], bp	; store the entry count
	clc			; there is "jc" on end of list to this point, so the carry must be cleared
	ret
.failed:
	stc			; "function unsupported" error exit
	ret



check_a20:
    pushf
    push ds
    push es
    push di
    push si
 
    cli
 
    xor ax, ax ; ax = 0
    mov es, ax
 
    not ax ; ax = 0xFFFF
    mov ds, ax
 
    mov di, 0x0500
    mov si, 0x0510
 
    mov al, byte [es:di]
    push ax
 
    mov al, byte [ds:si]
    push ax
 
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF
 
    cmp byte [es:di], 0xFF
 
    pop ax
    mov byte [ds:si], al
 
    pop ax
    mov byte [es:di], al
 
    mov ax, 0
    je check_a20__exit
 
    mov ax, 1
 
check_a20__exit:
    pop si
    pop di
    pop es
    pop ds
    popf
 
    ret

enableA20: ; ok lets start
	pushf ; flags
	push bx

	call check_a20 ; is it already enabled

	cmp ax, 1
	je enableA20_exit ; if it is already activated just go

	; start with int 15

	mov     ax,2403h                ;--- A20-Gate Support ---
	int     15h
	jb      a20_ns                  ;INT 15h is not supported
	cmp     ah,0
	jnz     a20_ns                  ;INT 15h is not supported
	
	mov     ax,2402h                ;--- A20-Gate Status ---
	int     15h
	jb      a20_failed              ;couldn't get status
	cmp     ah,0
	jnz     a20_failed              ;couldn't get status
	
	cmp     al,1
	jz      a20_activated           ;A20 is already activated
	
	mov     ax,2401h                ;--- A20-Gate Activate ---
	int     15h
	jb      a20_failed              ;couldn't activate the gate
	cmp     ah,0
	jnz     a20_failed              ;couldn't activate the gate
	
a20_activated: ; if int 15 worked then we can check it and leave

	call check_a20 ; ok there we test int 15

	cmp ax, 1
	je enableA20_exit

	cli	          ; lets test the keyboard controller
 
    call    a20wait
    mov     al,0xAD
    out     0x64,al
 
    call    a20wait
	mov     al,0xD0
    out     0x64,al
 
    call    a20wait2
    in      al,0x60
    push    eax
 
    call    a20wait
    mov     al,0xD1
    out     0x64,al
 
    call    a20wait
    pop     eax
    or      al,2
    out     0x60,al
 
    call    a20wait
    mov     al,0xAE
    out     0x64,al
 
    call    a20wait
    sti                 ; soo... that was easy
    
	call check_a20      ; is it done?

	cmp ax, 1
	je enableA20_exit   ; yes? no?
 
	in al, 0x92         ; try fast A20
	or al, 2
	out 0x92, al

	call check_a20      ; or we done yet?

	cmp ax, 1
	je enableA20_exit

	mov bx, A20LineCannotBeEnabled
	call printStr
	
	mov ax, 0
	call printNewline

	pop bx
	popf
	ret

enableA20_exit:

	mov bx, A20Enabled
	call printStr

	call printNewline

	pop bx
	popf
	ret

		
a20wait:
        in      al,0x64
        test    al,2
        jnz     a20wait
        ret
 
 
a20wait2:
        in      al,0x64
        test    al,1
        jz      a20wait2
        ret

a20_ns:
	mov bx, A20LineNTS
	call printStr

	call printNewline
	
	jmp a20_activated

a20_failed:
	mov bx, A20LineFailed
	call printStr

	call printNewline

	jmp a20_activated



[BITS 32]

code32:

	mov ax, 0x10 ; data selector
	mov es, ax   ; reset all segments
	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	call isCPUIDAvailable ; is cpuid available
	cmp eax, 0            ; yes? no?
	je noCpuid            ; no

	mov eax, 0x0000
	cpuid
	mov dword [cpuManufacturer], ebx
	mov dword [cpuManufacturer+0x04], edx
	mov dword [cpuManufacturer+0x08], ecx

	mov eax, 0x80000000    ; Set the A-register to 0x80000000.
    cpuid                  ; CPU identification.
    cmp eax, 0x80000001    ; Compare the A-register with 0x80000001.
    jb NoLongMode         ; It is less, there is no long mode.

	mov eax, 0x80000001    ; Set the A-register to 0x80000001.
    cpuid                  ; CPU identification.
    test edx, 1 << 29      ; Test if the LM-bit, which is bit 29, is set in the D-register.
    jz NoLongMode         ; They aren't, there is no long mode.

	; so now we know that we have longmode

	call makePages

	mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
    rdmsr                        ; Read from the model-specific register.
    or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
    wrmsr                        ; Write to the model-specific register.

	call enablePaging

	lgdt [gdtr64] ; Set GDT64
	
	jmp 0x08:code64
	;TODO: Go 64 Bit
	;TODO: Call kernel

	jmp $

noCpuid:
	; this is bad
	; I thnink we die here
	jmp $

NoLongMode:
	; TODO:
	; yea... so if we dont have long mode we should just call a 32 bit kernel
	; so write a 32 bit kernel for that
	jmp $

isCPUIDAvailable:
	pushfd                               ;Save EFLAGS
    pushfd                               ;Store EFLAGS
    xor dword [esp],0x00200000           ;Invert the ID bit in stored EFLAGS
    popfd                                ;Load stored EFLAGS (with ID bit inverted)
    pushfd                               ;Store EFLAGS again (ID bit may or may not be inverted)
    pop eax                              ;eax = modified EFLAGS (ID bit may or may not be inverted)
    xor eax,[esp]                        ;eax = whichever bits were changed
    popfd                                ;Restore original EFLAGS
    and eax,0x00200000                   ;eax = zero if ID bit can't be changed, else non-zero
    ret


; makes the paging structure at 0x1000
makePages:
	push eax
	push ecx
	push ebx
	push edi

	mov edi, 0x1000    ; Set the destination index to 0x1000.
    mov cr3, edi       ; Set control register 3 to the destination index.
    xor eax, eax       ; Nullify the A-register.
    mov ecx, 4096      ; Set the C-register to 4096.
    rep stosd          ; Clear the memory.
    mov edi, cr3       ; Set the destination index to control register 3.

	mov DWORD [edi], 0x2003      ; Set the uint32_t at the destination index to 0x2003.
    add edi, 0x1000              ; Add 0x1000 to the destination index.
    mov DWORD [edi], 0x3003      ; Set the uint32_t at the destination index to 0x3003.
    add edi, 0x1000              ; Add 0x1000 to the destination index.
    mov DWORD [edi], 0x4003      ; Set the uint32_t at the destination index to 0x4003.
    add edi, 0x1000              ; Add 0x1000 to the destination index.	

	mov ebx, 0x00000003          ; Set the B-register to 0x00000003.
    mov ecx, 512                 ; Set the C-register to 512.
 
.SetEntry:
    mov DWORD [edi], ebx         ; Set the uint32_t at the destination index to the B-register.
    add ebx, 0x1000              ; Add 0x1000 to the B-register.
    add edi, 8                   ; Add eight to the destination index.
    loop .SetEntry               ; Set the next entry.

	 mov eax, cr4                 ; Set the A-register to control register 4.
    or eax, 1 << 5               ; Set the PAE-bit, which is the 6th bit (bit 5).
    mov cr4, eax                 ; Set control register 4 to the A-register.

	pop edi
	pop ebx
	pop ecx
	pop eax
	ret

enablePaging:
	push eax
	mov eax, cr0                 ; Set the A-register to control register 0.
    or eax, 1 << 31 | 1 << 0     ; Set the PG-bit, which is the 31nd bit, and the PM-bit, which is the 0th bit.
    mov cr0, eax                 ; Set control register 0 to the A-register.
	pop eax
	ret

[BITS 64]

code64:

	mov ax, 0x10
	mov es, ax
	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	
	mov rdi, kernelInfoStruct
	jmp 0x10000

	jmp $


DAPACK:
	    db 0x10
	    db 0x00
blocc:	dw 0x00
off:	dw 0x00
segm:	dw 0x00
lba:	dd 0x00
		dd 0x00


gdt:
gdt_null: ; null descriptor is just empty
	dd 0
	dd 0
gdt_code:
	dw 0xFFFF ; limit
	dw 0x0000 ; base
	db 0x00   ; base
	db 0b10011010 ; access byte
	db 0b11001111 ; flags and limit
	db 0x0000 ; base
gdt_data:
	dw 0xFFFF ; limit
	dw 0x0000 ; base
	db 0x00   ; base
	db 0b10010010 ; access byte
	db 0b11001111 ; flags and limit
	db 0x0000 ; base
gdt_end:
gdtr:
	dw gdt_end - gdt - 1 
	dd gdt

gdt64:
gdt64_null: ; null descriptor is just empty
	dd 0
	dd 0
gdt64_code:
	dw 0xFFFF ; limit
	dw 0x0000 ; base
	db 0x00   ; base
	db 0b10011010 ; access byte
	db 0b10101111 ; flags and limit
	db 0x0000 ; base
gdt64_data:
	dw 0xFFFF ; limit
	dw 0x0000 ; base
	db 0x00   ; base
	db 0b10010010 ; access byte
	db 0b10101111 ; flags and limit
	db 0x0000 ; base
gdt64_usercode:
	dw 0xFFFF ; limit
	dw 0x0000 ; base
	db 0x00   ; base
	db 0b11111010 ; access byte
	db 0b10101111 ; flags and limit
	db 0x0000 ; base
gdt64_userdata:
	dw 0xFFFF ; limit
	dw 0x0000 ; base
	db 0x00   ; base
	db 0b11110010 ; access byte
	db 0b10101111 ; flags and limit
	db 0x0000 ; base
gdt64_end:
gdtr64:
	dw gdt64_end - gdt64 - 1
	dq gdt64




lbaLoop: resw 1
segmLoop: resw 1

diskErrorStr: db "Error while loading the rest of the kernel", 0
extensionsNotSupportedStr: db "INT 13 extensions not supported", 0
Welcome: db "We reached the second stage", 0
A20LineNTS: db "A20 Line for INT 15 is not supported", 0
A20LineFailed: db "A20 Line failed on enable, INT 15", 0
A20LineCannotBeEnabled: db "A20 is dead, get some help", 0
A20Enabled: db "A20 Enabled", 0
Drive: resb 1
kernelLoadedOrTried: db "Kernel loaded (we tried atleast)", 0
vbeblockfail: db "Could not get the vbe info block",0
space: db " ",0
videoModeNotFoundError: db "That video mode was not found try again", 0
nocpuid: db "CPUID is not available", 0


vbeinfoblock: 
	vbesign: db "VBE2"
	resb 508
modeinfoblock: resb 256

kernelInfoStruct:
VBEINFOFORKERNEL:
	bpsl: resw 1
	width: resw 1
	height: resw 1
	bpp: resb 1	
	rmasks: resb 1
	rmaskp: resb 1
	gmasks: resb 1
	gmaskp: resb 1
	bmasks: resb 1
	bmaskp: resb 1
	physicalFramebuffersegm: resw 1
	physicalFramebufferoffs: resw 1
memoryMapLocation: 
	mmap_ss: dw 0x9f04
cpuInfo:
	cpuManufacturer: resb 12

times 8192 - ($-$$) db 0

