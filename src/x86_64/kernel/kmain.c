#include "drivers/VGA.h"
#include "interrupts/IDT.h"
#include "interrupts/PIC.h"
#include "interrupts/syscalls.h"
#include "interrupts/interrupts.h"
#include "util/exceptions.h"
#include "sys/autostart.h"

char* vga = (char*)0xB8000;


int kmain() { 
    const char* const greet = "Kernel Loaded.\0";
    vgaClearScreen();       // Clears the screen.
    vgaFillScreen(0x07, 0x04);
    vga_puts(greet, &vga, 1); 

    // Mask off all hardware interrupts.
    disable_pic();

    remap_pic();

    // Setup interrupts.
    init_idt_32();

    // Setup exceptions.

    set_idt_desc_32(0, div_by_0_handler, TRAP_GATE_FLAGS);

    // Setup software interrupts.
    // INT_GATE_USER_FLAGS is so we can make a syscall from userland.
    set_idt_desc_32(0x79, asm_syscall_dispatcher, INT_GATE_USER_FLAGS);
    set_idt_desc_32(0x32, sys_restart, INT_GATE_FLAGS);
    set_idt_desc_32(0xC8, red_screen, INT_GATE_FLAGS);
    set_idt_desc_32(0xC9, move_mouse, INT_GATE_FLAGS);
    
    __autostart();

    return 0;
}
