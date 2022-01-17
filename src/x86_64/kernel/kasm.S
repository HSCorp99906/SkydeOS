global _start
extern kmain

section .text
_start:
    mov eax, 2           ; Set bit 2.
    out 0x92, eax        ; Enable A20 line.

    call kmain
    jmp $
