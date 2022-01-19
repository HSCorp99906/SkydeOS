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


// Remap PIC to use interrupts above 15.

void remap_pic(void) {
    uint8_t pic_1_mask, pic_2_mask;

    // Save current masks
    pic_1_mask = portByteIn(PIC_1_DATA);
    pic_2_mask = portByteIn(PIC_2_DATA);

    // ICW 1 (Initialization control word) - bit 0 = send up to ICW 4, bit 4 = initialize PIC
    portByteOut(PIC_1_CMD, 0x11);
    io_wait();
    portByteOut(PIC_2_CMD, 0x11);
    io_wait();

    // ICW 2 - Where to map the base interrupt in the IDT
    portByteOut(PIC_1_DATA, NEW_IRQ_0);
    io_wait();
    portByteOut(PIC_2_DATA, NEW_IRQ_8);
    io_wait();
    
    // ICW 3 - Where to map PIC2 to the IRQ line in PIC1; Mapping PIC2 to IRQ 2
    portByteOut(PIC_1_DATA, 0x4);  // Bit # (0-based) - 0100 = bit 2 = IRQ2
    io_wait();
    portByteOut(PIC_2_DATA, 0x2);  // Binary # for IRQ in PIC1, 0010 = 2
    io_wait();
   
    // ICW 4 - Set x86 mode
    portByteOut(PIC_1_DATA, 0x1); 
    io_wait();
    portByteOut(PIC_2_DATA, 0x1); 
    io_wait();

    // Save current masks
    portByteOut(PIC_1_DATA, pic_1_mask);
    portByteOut(PIC_2_DATA, pic_2_mask);
}

#endif
