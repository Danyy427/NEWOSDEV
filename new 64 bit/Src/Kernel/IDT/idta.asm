global idt_flush

idt_flush:
    lidt[rdi]
    ret

extern isr0_handler
extern isr1_handler
extern isr2_handler
extern isr3_handler
extern isr4_handler
extern isr5_handler
extern isr6_handler
extern isr7_handler
extern isr8_handler
extern isr9_handler
extern isr10_handler
extern isr11_handler
extern isr12_handler
extern isr13_handler
extern isr14_handler
extern isr15_handler
extern isr16_handler
extern isr17_handler
extern isr18_handler
extern isr19_handler
extern isr20_handler
extern isr21_handler
extern isr22_handler
extern isr23_handler
extern isr24_handler
extern isr25_handler
extern isr26_handler
extern isr27_handler
extern isr28_handler
extern isr29_handler
extern isr30_handler
extern isr31_handler

extern irq0_handler
extern irq1_handler
extern irq2_handler
extern irq3_handler
extern irq4_handler
extern irq5_handler
extern irq6_handler
extern irq7_handler
extern irq8_handler
extern irq9_handler
extern irq10_handler
extern irq11_handler
extern irq12_handler
extern irq13_handler
extern irq14_handler
extern irq15_handler


%macro PUSHALL 0
push rdi
push rsi
push rax
push rcx
push rbx
push rdx
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15
push rsp
push rbp
%endmacro

%macro POPALL 0
pop rbp
add rsp, 8
pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rdx
pop rbx
pop rcx
pop rax
pop rsi
pop rdi
add rsp, 0x10
%endmacro




global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

%macro ISR_NOERR 1
isr%1:
    push    0
    push    %1

    PUSHALL

    mov     rdi, rsp
    sub     rsp, 0x28

    cld
    call    isr%1_handler
    add     rsp, 0x28

	POPALL

	iretq

%endmacro

%macro ISR_ERR 1
isr%1:
    push    %1
    PUSHALL

    mov     rdi, rsp
    sub     rsp, 0x28

    cld
    call    isr%1_handler
    add     rsp, 0x28

	POPALL

	iretq
%endmacro

%macro IRQ 1
irq%1:
	push 0
	push (%1 + 32)
    PUSHALL

    mov     rdi, rsp
    sub     rsp, 0x28

    cld
    call    irq%1_handler
    add     rsp, 0x28

	POPALL

	iretq
%endmacro

; 0: Divide By Zero Exception
ISR_NOERR 0 

; 1: Debug Exception
ISR_NOERR 1 

; 2: Non Maskable Interrupt Exception
ISR_NOERR 2 

; 3: Int 3 Exception
ISR_NOERR 3 

; 4: INTO Exception
ISR_NOERR 4 

; 5: Out of Bounds Exception
ISR_NOERR 5 

; 6: Invalid Opcode Exception
ISR_NOERR 6 

; 7: Coprocessor Not Available Exception
ISR_NOERR 7 
; 8: Double Fault Exception (With Error Code!)
ISR_ERR 8 

; 9: Coprocessor Segment Overrun Exception
ISR_NOERR 9 

; 10: Bad TSS Exception (With Error Code!)
ISR_ERR 10 

; 11: Segment Not Present Exception (With Error Code!)
ISR_ERR 11 

; 12: Stack Fault Exception (With Error Code!)
ISR_ERR 12 

; 13: General Protection Fault Exception (With Error Code!)
ISR_ERR 13 

; 14: Page Fault Exception (With Error Code!)
ISR_ERR 14 

; 15: Reserved Exception
ISR_ERR 15 

; 16: Floating Point Exception
ISR_ERR 16 

; 17: Alignment Check Exception
ISR_ERR 17 

; 18: Machine Check Exception
ISR_ERR 18 

; 19: Reserved
ISR_ERR 19 

; 20: Reserved
ISR_ERR 20 

; 21: Reserved
ISR_ERR 21 

; 22: Reserved
ISR_ERR 22 

; 23: Reserved
ISR_ERR 23 

; 24: Reserved
ISR_ERR 24 

; 25: Reserved
ISR_ERR 25 

; 26: Reserved
ISR_ERR 26 

; 27: Reserved
ISR_ERR 27 

; 28: Reserved
ISR_ERR 28 

; 29: Reserved
ISR_ERR 29 

; 30: Reserved
ISR_ERR 30 

; 31: Reserved
ISR_ERR 31 

; IRQ handlers
IRQ 0 
IRQ 1 
IRQ 2 
IRQ 3 
IRQ 4 
IRQ 5 
IRQ 6 
IRQ 7 
IRQ 8 
IRQ 9 
IRQ 10 
IRQ 11 
IRQ 12 
IRQ 13 
IRQ 14 
IRQ 15 