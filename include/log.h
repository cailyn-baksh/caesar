#ifndef _LOG_H_
#define _LOG_H_

#include <stdint.h>
#include <stdio.h>

#include "cipher.h"

#define _Q(s) #s

#define Q(s) _Q(s)

void _init_log(FILE *f);

/*
 * Initialize logging
 *
 * f            A file stream pointer or string file path indicating which file
 *              should be used for logging.
 */
#define init_log(f) _init_log(_Generic((f), char *: fopen(f, "a"),  FILE *: f))

/*
 * Closes the log file
 */
void close_log();

// TODO: log in another thread to prevent lag
void _print_log(const char *fmt, ...);

/*
 * Prints a formatted string to the log file, along with the source file the
 * log message originated from and the time the message occurred at. This
 * function has the same arguments as printf(3).
 */
#define print_log(fmt, ...) _print_log("%s:%s():%zu: " fmt, __BASE_FILE__, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

void log_game_state(FILE *file, Game *game);


#endif  // _LOG_H_

