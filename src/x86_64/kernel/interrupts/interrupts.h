#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "IDT.h"
#include "../drivers/IO.h"

__attribute__((interrupt)) void sys_restart(int_frame_32_t* int_frame_32) {
    portByteOut(0x64, 0xFE);
}


/*
 * This ISR requires a value in EDX.
 * If EDX is 1 then read, 
 * if 0 then write, value to write
 * must be in EAX.
 *
 * If reading, the value will be stored
 * in EAX.
 */
__attribute__((interrupt)) void keyboard_handler(int_frame_32_t* int_frame_32) {
    register uint8_t edxFlag asm("edx");

    if (edxFlag) {
        register unsigned char eax asm("eax") = portByteIn(0x60); 
    } else {
        register unsigned char data asm("eax");
        portByteOut(0x60, data);
    }
}


__attribute__((interrupt)) void red_screen(int_frame_32_t* int_frame_32) {
    char* vga = (char*)0xB8000;
    vgaClearScreen();
    vgaFillScreen(0x04, 0x07); 
}

#endif
