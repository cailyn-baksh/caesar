#include <threads.h>
#include <time.h>

#include "log.h"
#include "util.h"

uint32_t fnv1a32(const char *msg) {
    uint32_t hash = 0x811C9DC5;

    for (; *msg; ++msg) {
        hash ^= *msg;
        hash *= 0x01000193;
    }

    return hash;
}

uintmax_t sleep_ms(uintmax_t ms) {
    // Convert ms into a timespec struct
    struct timespec ts = {
        .tv_sec = ms / 1000,
        .tv_nsec = (ms % 1000) * 1'000'000
    };

    if (thrd_sleep(&ts, &ts) == -1) {
        // If the sleep terminates early, return the remaining time
        ms = ts.tv_sec * 1000;
        ms += ts.tv_nsec / 1'000'000;
        return ms;
    }

    // Sleep succeeded, thus 0ms remain
    return 0;
}

int wnputw(WINDOW *win, size_t n, const char *str) {
    for (; n != 0; --n, ++str) {
        if (waddch(win, *str) == ERR) return ERR;
    }

    return OK;
}

int mvwnputw(WINDOW *win, int y, int x, size_t n, const char *str) {
    if (wmove(win, y, x) == ERR) return ERR;

    return wnputw(win, n, str);
}
