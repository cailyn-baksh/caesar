#ifndef _LOG_H_
#define _LOG_H_

#include <stdint.h>
#include <stdio.h>

#include "cipher.h"

void _init_log(FILE *f);

/*
 * Initialize logging
 */
#define init_log(f) _init_log(_Generic((f), char *: fopen(f, "a"),  FILE *: f))

void close_log();

// TODO: log in another thread to prevent lag
void _print_log(const char *fmt, ...);

#define print_log(fmt, ...) _print_log(__BASE_FILE__ ": " fmt __VA_OPT__(,) __VA_ARGS__)

void log_game_state(FILE *file, Game *game);


#endif  // _LOG_H_

