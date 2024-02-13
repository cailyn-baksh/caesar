#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "log.h"

#define INDENT "  "

FILE *logfile = nullptr;

void _init_log(FILE *f) {
    if (logfile != nullptr) close_log();
    if (f != nullptr) {
        logfile = f;

        // Indicate start of new segment in log file
        for (int i=0; i < 40; ++i) {
            fputc('=', logfile);
            fputc('-', logfile);
        }
        fputc('\n', logfile);

        print_log("Initialized logger.");
    } else {
        logfile = stderr;

        print_log("Target log file stream was null; falling back to stderr.");
    }
}

void close_log() {
    print_log("Closing log file");

    if (logfile != nullptr && logfile != stderr && logfile != stdout) {
        fclose(logfile);
    }

    logfile = nullptr;
}

void _print_log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    FILE *f = (logfile != nullptr) ? logfile : stderr;

    // Get datetime
    const time_t cur_time = time(nullptr);
    char time_str[23] = { '\0' };

    // [YYYY-MM-DDTHH:mm:ss]
    strftime(time_str, 32, "[%FT%T]", localtime(&cur_time));
    fprintf(f, "%s ", time_str);

    // Print log message
    vfprintf(f, fmt, args);

    fprintf(f, "\n");

    va_end(args);
}

void log_game_state(FILE *file, Game *game) {
    if (game == NULL) {
        fprintf(file, "log_game_state: Game ptr is NULL!");
        return;
    }

    char cipher_preview[20] = { '\0' };
    strncpy(cipher_preview, game->ciphertext, 16);
    strcpy(&cipher_preview[16], "...");

    char key_preview[20] = { '\0' };
    for (size_t i=0; (i < game->key_size) && (i < 8); ++i) {
        sprintf(&key_preview[i*2], "%02" PRIX8, game->key[i]);
    }
    strcpy(&cipher_preview[16], "...");

    char time_str[16] = { '\0' };
    strftime(time_str, 16, "%a %T", localtime(&game->start_time));

    fprintf(file,
            "Game @ %p\n"
            INDENT ".key = [%p] %s\n"
            INDENT ".ciphertext = [%p] \"%s\"\n"
            INDENT ".buffer_size = %zu\n"
            INDENT ".answer_hash = %" PRIX32 "\n"
            INDENT ".paused = %s\n"
            INDENT ".start_time = %s\n"
            INDENT ".extra_time = %.2F\n",
            game, game->key, key_preview, game->ciphertext, cipher_preview,
            game->buffer_size, game->answer_hash,
            game->paused ? "true" : "false", time_str, game->extra_time);
}

