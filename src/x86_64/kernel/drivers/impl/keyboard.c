#include "../keyboard.h"


keypress_t getkey() {
     const uint8_t* const SC_ASCII = "\x00\x1B" "1234567890-=" "\x08"
    "\x00" "qwertyuiop[]" "\x0D\x1D" "asdfghjkl;'`" "\x00" "\\"
    "zxcvbnm,./" "\x00\x00\x00" " ";

    while (1) { 
        if (portByteIn(0x64) & 0x01) {

            uint8_t scancode = portByteIn(0x60);
            uint8_t chr = SC_ASCII[scancode];
       
            switch (scancode) {
                case 28:
                    {
                        keypress_t curKeypress = {
                            .scancode = 28,
                            .chr = ' ',
                        };

                        return curKeypress;
                    }
                case 83:
                    {
                        keypress_t curKeypress = {
                            .scancode = 83,
                            .chr = ' ',
                        };

                        return curKeypress;
                    }
            }

            if (chr < 'a' || chr > 'z') {
                continue;
            }

            chr -= 0x20;

            keypress_t curKeypress = {
                .scancode = scancode,
                .chr = chr,
            };

            return curKeypress;
        }
    }
}
