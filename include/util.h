#ifndef _UTIL_H_
#define _UTIL_H_

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include <ncurses.h>

/*
 * Cleaner-looking way to cast a pointer to another type and dereference it.
 *
 * type     The type to dereference the pointer as
 * ptr      The pointer to dereference
 * Produces an lvalue expression of the specified type
 */
#define deref_as(type, ptr) (*((type *)ptr))

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
 * The return value is the total number of milliseconds remaining. If the sleep
 * succeeded and was not interrupted, this will be 0.
 */
uintmax_t sleep_ms(uintmax_t ms);

int wnputw(WINDOW *win, size_t n, const char *str);
int mvwnputw(WINDOW *win, int y, int x, size_t n, const char *str);

#define nputw(n, str) wnputw(stdscr, n, str)
#define mvnputw(y, x, n, str) mvwnputw(stdscr, y, x, n, str)

#endif  // _UTIL_H_

