#include "drivers/VGA.h"

int kmain() {
    *(char*)0xB8000 = 'A';
    return 0;
}
