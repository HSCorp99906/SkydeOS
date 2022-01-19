#include "../memory.h"


void memset(void* s, int c, unsigned long long len) {
    unsigned char* dst = s;

    while (len > 0) {
        *dst = (unsigned char)c;
        ++dst;
        --len;
    }
}
