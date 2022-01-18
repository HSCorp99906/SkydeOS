#include "drivers/VGA.h"
#include "interrupts/IDT.h"
#include "interrupts/syscalls.h"
#include "util/exceptions.h"

char* vga = (char*)0xB8000;


int kmain() { 
    const char* const greet = "Kernel Loaded.\0";
    vgaClearScreen();       // Clears the screen.
    vgaFillScreen(0x07, 0x04);
    vga_puts(greet, &vga, 1); 

    // Setup interrupts.
    init_idt_32();

    // Setup exceptions.

    set_idt_desc_32(0, div_by_0_handler, TRAP_GATE_FLAGS);

    // Setup software interrupts.
    // INT_GATE_USER_FLAGS is so we can make a syscall from userland.
    set_idt_desc_32(0x79, asm_syscall_dispatcher, INT_GATE_USER_FLAGS);
    __asm__ __volatile__("mov $0, %eax; int $0x79");

    return 0;
}
