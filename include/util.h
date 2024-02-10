#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <time.h>

/*
 * Computes a 32-bit Fowler-Noll-Vo hash for a string message, using the FNV-1a
 * algorithm.
 *
 * msg      The string to hash
 * Returns the hash as an unsigned 32-bit integer.
 */
uint32_t fnv1a32(const char *msg);

/*
 * Sleeps for a number of milliseconds
 *
 * ms       The number of milliseconds to sleep for
 * The return value is the total number of milliseconds remaining, if the sleep
 * did not last the full duration. Thus, the function returns 0 on success.
 */
uintmax_t sleep_ms(uintmax_t ms);

/*
 * Like difftime(3), but using struct timespec for nanosecond accuracy.
 *
 * ts_end       The timespec structures to compare
 * ts_beg
 * base         TIME_UTC or another nonzero integer indicating the time base.
 * Returns the difference between the two times with maximum possible precision
 */
double difftime_ns(struct timespec *ts_end, struct timespec *ts_beg, int base);

/*
 * Determines the number of seconds since a given time to nanosecond accuracy.
 *
 * ts_start     A pointer to the timespec structure to compare against.
 * base         The time base of used to acquire a
 * Returns the duration in seconds as a double with maximum precision.
 */
double timesince(const struct timespec *const ts_start, int base);

/*
 * Asynchronously toggles a flag after a given period of time
 *
 * flag     A pointer to the flag to toggle
 * ms       The number of milliseconds to wait before toggling
 */
void async_toggle_after(_Atomic bool *const flag, uintmax_t ms);

/*
 * Sets a flag before clearing it asynchronously after the specified time has
 * elapsed.
 *
 * flag     A pointer to the flag
 * ms       The duration in milliseconds to wait before clearing the flag
 * Returns thrd_success on success, otherwise returns the relevant thrd error.
 */
int async_open_flag_for(_Atomic bool *flag, uintmax_t ms);

#endif  // _UTIL_H_

