#include "../VGA.h"


void vga_puts(const char* str, char** vga_buf) {
    for (int i = 0; i < strlen(str); ++i) {
        **vga_buf = str[i];
        *vga_buf += 2;
    } 
}
