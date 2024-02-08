#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>

/*
 * Computes a 32-bit Fowler-Noll-Vo hash for a string message, using the FNV-1a
 * algorithm.
 *
 * msg      The string to hash
 * Returns the hash as an unsigned 32-bit integer.
 */
uint32_t fnv1a32(const char *msg);

#endif  // _UTIL_H_

