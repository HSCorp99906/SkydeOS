#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "IDT.h"
#include "../drivers/IO.h"

__attribute__((interrupt)) void sys_restart(int_frame_32_t* int_frame_32) {
    portByteOut(0x64, 0xFE);
}


#endif
