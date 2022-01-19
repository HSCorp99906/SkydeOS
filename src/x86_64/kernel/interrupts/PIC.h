#ifndef PIC_H
#define PIC_H

#include "../util/types.h"
#include "../drivers/IO.h"

#define PIC_1_CMD 0x20
#define PIC_1_DATA 0x21

#define PIC_2_CMD 0xA0
#define PIC_2_DATA 0xA1

#define NEW_IRQ_0 0x20
#define NEW_IRQ_8 0x28

#define PIC_EOI 0x20        // End of interrupt command.

/*
 * IRQ is what interrupt we are
 * handling.
 */
void send_pic_eoi(uint8_t irq) {
    if (irq >= 8) {
        portByteOut(PIC_2_CMD, PIC_EOI);
    }

    portByteOut(PIC_1_CMD, PIC_EOI);
}


void disable_pic() {
    portByteOut(PIC_2_CMD, 0xFF);
    portByteOut(PIC_1_CMD, 0xFF);
}


// Set IQR mask by setting a bit in the IMR (interrupt mask register).

void set_irq_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC_2_DATA;
    } else {
        irq -= 8;
        port = PIC_1_DATA;
    }

    value = portByteIn(port) & ~(1 << irq);
    portByteOut(port, value);
}


#endif
