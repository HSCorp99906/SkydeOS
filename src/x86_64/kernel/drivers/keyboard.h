#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../util/types.h"
#include "../drivers/IO.h"


typedef struct {
    uint8_t scancode;
    uint8_t chr;
} keypress_t;


keypress_t getkey();


#endif
