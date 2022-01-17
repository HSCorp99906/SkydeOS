#include "../IO.h"

unsigned char portByteIn(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "a=" (result) : "d" (port));
    return result;
 }


void portByteOut(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%ax" : :"a" (data), "d" (port));
}