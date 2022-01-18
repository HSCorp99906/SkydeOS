#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "../interrupts/IDT.h"
#include "../drivers/VGA.h"

__attribute__((interrupt)) void div_by_0_handler(int_frame_32_t* int_frame_32) {
    char* vga = (char*)0xB8000;
    vgaClearScreen();
    vgaFillScreen(0x04, 0x07); 
    vga_puts("DIVIDE BY 0 ERROR CAUGHT - SYSTEM HALTED.\0", &vga);
    __asm__("hlt");
}


#endif
