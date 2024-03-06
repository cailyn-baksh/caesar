#ifndef _LOG_H_
#define _LOG_H_

#include <stdint.h>
#include <stdio.h>

#include "cipher.h"

#define _Q(s) #s
#define Q(s) _Q(s)

enum LogLevel {
    LOG_FATAL = 0,
    LOG_ERROR = 6,
    LOG_WARNING = 12,
    LOG_URGENT = 18,
    LOG_INFO = 24,
    LOG_DEBUG = 30
};

// Globals
#if (defined(_DEFINE_GLOBALS) && !defined(_LOG_H_GLOBALS_DEFINED) && __INCLUDE_LEVEL__ == 1) || __INCLUDE_LEVEL__ == 0
#define _LOG_H_GLOBALS_DEFINED
enum LogLevel logLevel = LOG_DEBUG;
#elifndef _LOG_H_GLOBALS_DEFINED
extern enum LogLevel logLevel;
#endif  // _DEFINE_GLOBALS

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
void close_log(void);

// TODO: log in another thread to prevent lag
/*
 * Prints a formatted string to the log file, in the format
 *
 * "[YYYY-MM-DDTHH:mm:ss] $level: $fmt\n" % va_args
 *
 * level            The level of this message. Messages are ignored if level is
 *                  greater than the global logLevel.
 * fmt              The format string to log. See printf(3) for details.
 */
void print_log(enum LogLevel level, const char *fmt, ...);

/*
 * Prints a formatted string to the log file, in the format
 *
 * "[YYYY-MM-DDTHH:mm:ss] $level: FILENAME:FUNCNAME():LINE: $fmt\n" % va_args
 *
 * level            The level of this message. Messages are ignored if level is
 *                  greater than the global logLevel.
 * fmt              The format string to log. See printf(3) for details.
 */
#define print_log_src(lvl, fmt, ...) print_log(lvl, "%s:%s():%zu: " fmt, __BASE_FILE__, __func__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

/*
 * Writes the contents of a Game struct to the log file.
 */
void log_game_state(Game *game);

#endif  // _LOG_H_

