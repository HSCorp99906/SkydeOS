#include "../VGA.h"


void vga_puts(const char* str) {
    static char* vga_buf = (char*)0xB8000;

    for (int i = 0; i < strlen(str); ++i) {
        *vga_buf = str[i];
        vga_buf += 2;
    }
}
