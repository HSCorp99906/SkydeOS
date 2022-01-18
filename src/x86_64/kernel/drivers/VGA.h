#ifndef VGA_H
#define VGA_H

#define SCREEN_WIDTH 80

#include "../util/strings.h"
#include "../util/types.h"

void vga_puts(const char* str, char** vga_buf, uint8_t make_newline);
void vgaFillScreen(unsigned short bg, unsigned short fg);
void vgaClearScreen();

#endif
