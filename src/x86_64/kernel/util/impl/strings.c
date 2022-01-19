#include "../strings.h"


size_t strlen(const char* const STR) {

    size_t len = 0;

    while (STR[len++]); 
    return len;
}


uint32_t strcmp(const char* const STR, const char* const STR1) {
	unsigned int idx = 0;

	for (int i = 0; i < strlen(STR); ++i) {
		if (STR[i] != STR1[idx]) {
			return 0;
		}

        ++idx;
	}

	return 1;
}

