#include "drivers/VGA.h"
#include "interrupts/IDT.h"
#include "util/exceptions.h"

char* vga = (char*)0xB8000 + 160;


int kmain() { 
    const char* const greet = "Kernel Loaded.";
    vgaClearScreen();       // Clears the screen.
    vgaFillScreen(0x07, 0x04);
    vga_puts(greet, &vga);  // Puts the greeting message.

    // Setup interrupts.
    init_idt_32();

    // Setup exceptions.
    set_idt_desc_32(0, div_by_0_handler, TRAP_GATE_FLAGS);

    return 0;
}
