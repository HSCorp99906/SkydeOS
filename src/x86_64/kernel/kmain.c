#include "drivers/VGA.h"

char* vga = (char*)0xB8000 + 160;


int kmain() {
    const char* const greet = "Hello from the kernel!!\0";
    vga_puts(greet, &vga);
    return 0;
}
