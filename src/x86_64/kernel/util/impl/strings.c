#include "../strings.h"


size_t strlen(const char* const STR) {

    size_t len = 0;

    while (STR[len] != '\0') {
        ++len;
    }

    return len;
}
