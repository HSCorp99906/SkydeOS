/*
 * Syscall handler/dispatcher written by Ian Marco Moffett.
 * Made with suffering and pain :D
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#define MAX_SYSCALLS 1
#include "../drivers/VGA.h"


extern void asm_syscall_dispatcher();

void syscall_test0() {
    char* vga = (char*)0xB8000 + 324;
    vga_puts("Syscall test0 has been called.\0", &vga, 1);
    return;
}


// Syscall table.
void* syscalls[MAX_SYSCALLS] = {
    syscall_test0
};


#endif
