#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "IDT.h"
#include "../drivers/IO.h"
#include "../util/types.h"


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

__attribute__((interrupt)) void move_mouse(int_frame_32_t* frame) {
    const uint16_t VGA_WIDTH = 80;

    register uint16_t x asm("eax");
    register uint16_t y asm("edx");

    uint16_t pos = y * VGA_WIDTH + x;
 
	portByteOut(0x3D4, 0x0F);
	portByteOut(0x3D5, (uint8_t) (pos & 0xFF));
	portByteOut(0x3D4, 0x0E);
	portByteOut(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

#endif
