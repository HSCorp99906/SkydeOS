#include "../IO.h"

unsigned char portByteIn(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
 }


void portByteOut(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}


void io_wait() {
    // Port 0x80 is used for checkpoints during POST
    //  Linux uses this, so we "should" be OK to use it also
    __asm__ __volatile__ ("outb %%al, $0x80" : : "a"(0) );
}
