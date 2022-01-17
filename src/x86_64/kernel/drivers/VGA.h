#ifndef VGA_H
#define VGA_H

#include "../util/strings.h"

char* vga_buf = (char*)0xB8000;

void vga_puts(const char* str);


#endif
