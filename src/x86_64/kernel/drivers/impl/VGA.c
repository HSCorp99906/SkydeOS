#include "../VGA.h"


void vga_puts(const char* str, char** vga_buf, uint8_t make_newline) {
    for (int i = 0; i < strlen(str); ++i) {
        **vga_buf = str[i];
        *vga_buf += 2;
    }

    if (make_newline) {
        for (int i = 0; i < 80 - strlen(str); ++i) {
            **vga_buf = ' ';
            *vga_buf += 2;
        }
    }
}


void vgaFillScreen(unsigned short bg, unsigned short fg) {
    char* vga = (char*)0xB8000;
    unsigned short attrib = (bg << 4) | (fg & 0x0F);

    for (int i = 0; i < 20000; ++i) {
        *vga = ' ';
        ++vga;
        *vga = attrib;
        ++vga;
    }
}

void vgaClearScreen() {
    char* vga = (char*)0xB8000;

    for (int i = 0; i < 20000; ++i) {
        *vga = ' ';
        vga += 2;
    }
}
