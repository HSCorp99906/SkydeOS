#include "drivers/VGA.h"
#include "hardware/IDT.h"

char* vga = (char*)0xB8000 + 160;


int kmain() { 
    const char* const greet = "Kernel Loaded.";
    vgaClearScreen();       // Clears the screen.
    vgaFillScreen(0x07, 0x04);
    vga_puts(greet, &vga);  // Puts the greeting message.
    return 0;
}
