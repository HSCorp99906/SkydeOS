#ifndef VGA_H
#define VGA_H

#define SCREEN_WIDTH 80

#include "../util/strings.h"

void vga_puts(const char* str, char** vga_buf);
void vgaFillScreen(unsigned short bg, unsigned short fg);
void vgaClearScreen();

#endif
