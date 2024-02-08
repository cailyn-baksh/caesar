#include "util.h"

uint32_t fnv1a32(const char *msg) {
    uint32_t hash = 0x811C9DC5;

    for (; *msg; ++msg) {
        hash ^= *msg;
        hash *= 0x01000193;
    }

    return hash;
}

