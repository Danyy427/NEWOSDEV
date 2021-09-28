[BITS 64]

global kentry

extern kmain

section .top_of_kernel

kentry:
    call kmain
    
    jmp $
