BITS 64

global loadGDT

loadGDT:
    cli
    lgdt [rdi]
    
    mov ax, 0x10
    mov es, ax
    mov gs, ax
    mov ds, ax
    mov fs, ax

    pop     rdi
    mov     rax, 0x08
    push    rax
    push    rdi

    retfq