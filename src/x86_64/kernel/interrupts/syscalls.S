%define MAX_SYSCALLS 1

extern syscalls
global asm_syscall_dispatcher

asm_syscall_dispatcher:
    cmp eax, MAX_SYSCALLS - 1
    jg invalid_syscall

    pusha
    call [syscalls + eax]       ; Calls syscall.
    popa
    iretd

invalid_syscall:
    iretd
