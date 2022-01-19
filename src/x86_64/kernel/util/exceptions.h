#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "../interrupts/IDT.h"
#include "../drivers/VGA.h"

__attribute__((interrupt)) void div_by_0_handler(int_frame_32_t* int_frame_32) {
    char* vga = (char*)0xB8000;
    __asm__ __volatile__("int $0x24"); 
    vga_puts("DIVIDE BY 0 ERROR CAUGHT - SYSTEM HALTED.\0", &vga, 1);
    __asm__ __volatile__("hlt");
}


#endif
