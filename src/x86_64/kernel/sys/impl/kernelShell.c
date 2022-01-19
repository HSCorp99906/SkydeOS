#include "../kernelShell.h"

#define BUFFER_SIZE 15


uint16_t x = 16;
uint16_t y = 2;

void start_shell() {
    extern char* vga;

    char writeBuf[2] = "0\0";
    char commandBuffer[BUFFER_SIZE];
    commandBuffer[14] = '\0';
    uint16_t cmdBufIdx = 0;
    
    __asm__ __volatile__("mov %%edx, %%eax; mov $2, %%edx; int $0xC9" : : "d"(x));

    vga_puts("", &vga, 1);
    vga_puts("Kernel@KessOS~$ ", &vga, 0);

    while (1) {
        keypress_t keypress = getkey();

        switch (keypress.scancode) {
            case 28:
                if (strcmp(commandBuffer, "REBOOT\0")) {
                    __asm__ __volatile__("int $0x32");
                }

                continue;
            case 83:
                if (x > 16) {
                    vga -= 2;
                    --x;
                    *vga = ' ';
                    __asm__ __volatile__("mov %%edx, %%eax; mov $2, %%edx; int $0xC9" : : "d"(x));
                    commandBuffer[cmdBufIdx] = '\0';
                    --cmdBufIdx;
                }

                continue;
              
        }

        if (x < 30) { 
            *writeBuf = keypress.chr;
            vga_puts(writeBuf, &vga, 0);
            ++x;
            __asm__ __volatile__("mov %%edx, %%eax; mov $2, %%edx; int $0xC9" : : "d"(x));
            commandBuffer[cmdBufIdx] = keypress.chr;
            ++cmdBufIdx;
            vga -= 2;

        }
    }
}
