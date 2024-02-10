#include <threads.h>
#include <time.h>

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
    struct timespec ts = {
        .tv_sec = ms / 1000,
        .tv_nsec = (ms % 1000) * 1'000'000
    };

    if (thrd_sleep(&ts, &ts) == -1) {
        ms = ts.tv_sec * 1000;
        ms += ts.tv_nsec / 1'000'000;
    }

    return 0;
}

double timesince(const struct timespec *const ts_start, int base) {
    struct timespec ts_now;
    timespec_get(&ts_now, base);

    double diff = ts_now.tv_sec - ts_start->tv_sec;
    diff += (ts_now.tv_nsec - ts_start->tv_nsec) / 1'000'000'000.0;

    return diff;
}

// this is either the worst or best code ive ever written
void _await_and_close_flag_thrd(void *data) {
    uintmax_t ms = *(uintmax_t *)data;
    _Atomic bool *flag = *((_Atomic bool **)(data + sizeof(uintmax_t)));

    // Set the flag to true to indicate that data has been recieved
    *flag = true;

    sleep_ms(ms);

    *flag = false;
}

int async_open_flag_for(_Atomic bool *flag, uintmax_t ms) {
    thrd_t thread;
    uint8_t data[sizeof(uintmax_t) + sizeof(_Atomic bool *)];

    *((uintmax_t *)data) = ms;
    *((_Atomic bool **)(data + sizeof(uintmax_t))) = flag;

    int result = thrd_create(&thread, (thrd_start_t)_await_and_close_flag_thrd, data);

    // Idle until flag is set so that data remains on the stack
    while (!(*flag));

    return result;
}

