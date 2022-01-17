#include "../VGA.h"


void vga_puts(const char* str) {
    for (int i = 0; i < strlen(str); ++i) {
        *vga_buf = str[i];
        vga_buf += 2;
    }
}
