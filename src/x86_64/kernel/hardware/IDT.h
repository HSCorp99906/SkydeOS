#ifndef IDT_H
#define IDT_H

#include "../util/types.h"
#include "../drivers/VGA.h"

/*
 * A gate (call, interrupt, task or trap) is used 
 * to transfer control of execution across segments.
 */

#define TRAP_GATE_FLAGS 0x8F     // Defines 32-bit trap gate.
#define INT_GATE_FLAGS  0x8E    // Defines 32-bit interrupt gate.
#define INT_GATE_USER_FLAGS 0xEE    // Used for syscalls.

typedef struct {
    uint16_t isr_low;       // Lower 16 bits of ISR address.
    uint16_t kernel_cs;     // The GDT segment selector that the CPU will load into CS before calling the ISR.
    uint8_t reserved;       // Set to zero.
    uint8_t attributes;     // Type and attributes.
    uint16_t isr_high;      // Higher 16 bits of ISR.
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;       // IDT register. 

idt_entry_t idt32[256];     // The actual IDT.
idtr_t idtr32;

typedef struct {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t sp;
    uint32_t ss;
} __attribute__((packed)) int_frame_32_t;    // Interrupt frame.

// Default exception handler (no error code).
__attribute__((interrupt)) void default_excp_handler(int_frame_32_t* int_frame_32) {
    vgaClearScreen();
    vgaFillScreen(0x04, 0x07);
    const char const* ERROR_MSG = "A FATAL EXCEPTION OCCURRED - SYSTEM HALTED.";
    char* vga = (char*)0xB8000;
    vga_puts(ERROR_MSG, &vga);
    __asm__("hlt");
}


__attribute__((interrupt)) void default_excp_handler_err_code(int_frame_32_t* int_frame_32, uint32_t error_code) {
    vgaClearScreen();
    vgaFillScreen(0x04, 0x07);
    const char const* ERROR_MSG = "A FATAL EXCEPTION OCCURRED - SYSTEM HALTED.";
    char* vga = (char*)0xB8000;
    vga_puts(ERROR_MSG, &vga);
    __asm__("hlt");
}

__attribute__((interrupt)) void default_int_handler(int_frame_32_t* int_frame_32) { /* NOT IMPLEMENTED FOR NOW. */ }

// Add an ISR to IDT.
void set_idt_desc_32(uint8_t entry_number, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt32[entry_number];     // Interrupt descriptor.
    
    // Low 16 bits of ISR address.
    descriptor->isr_low = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;       // Address of kernel code segment.
    descriptor->reserved = 0;           // Reserved must be 0.
    descriptor->attributes = flags;
    descriptor->isr_high = ((uint32_t)isr >> 16) & 0xFFFF;     // High 16 bits.
}

// Setup default exceptions.
void init_idt_32() {
    // Each descripter is 8 bytes, limit is 255.
    idtr32.limit = (uint16_t)(8 * 255);
    idtr32.base = (uint32_t)&idt32[0];

    // Setup exception handling (IRS 0-31).

    for (uint8_t entry = 0; entry < 32; ++entry) {
        if (entry == 8 || entry == 10 || entry == 11 || entry == 12 || entry == 13  ||
            entry == 14  || entry == 17 || entry == 21) {
            // Exception takes error code.
            set_idt_desc_32(entry, default_excp_handler_err_code, TRAP_GATE_FLAGS);
        } else {
            // Exception doesn't take error code.
            set_idt_desc_32(entry, default_excp_handler, TRAP_GATE_FLAGS);
        }
    }

    // Setup regular interrupts.
    
    for (uint16_t entry = 32; entry < 256; ++entry) {
        set_idt_desc_32(entry, default_int_handler, INT_GATE_FLAGS);
    }

    __asm__ __volatile__("lidt (%0)" : : "memory"(idtr32));  // Loads IDT.
}


#endif
