global _start
extern kmain

section .text
_start:    
    mov [0xB8000], byte 'X'
    jmp $
