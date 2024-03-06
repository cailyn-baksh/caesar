#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#define _DEFINE_GLOBALS
#include "log.h"
#undef _DEFINE_GLOBALS

#define INDENT "  "

static FILE *logfile = nullptr;

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
    } else {
        logfile = stderr;

        print_log_src(LOG_WARNING, "Target log file stream was null; falling back to stderr.");
    }
}

void close_log(void) {
    if (logfile != nullptr && logfile != stderr && logfile != stdout) {
        fclose(logfile);
    }

    logfile = nullptr;
}

const char *logLevelToStr(enum LogLevel level) {
    const char *strs =
        "FATAL\0"
        "ERROR\0"
        "WARN\0\0"
        "URGNT\0"
        "INFO\0\0"
        "DEBUG\0"
        ;

    return &strs[level];
}

void print_log(enum LogLevel level, const char *fmt, ...) {
    if (level > logLevel) return;

    va_list args;
    va_start(args, fmt);

    FILE *f = (logfile != nullptr) ? logfile : stderr;

    // Get datetime
    const time_t cur_time = time(nullptr);
    char time_str[23] = { '\0' };

    // [YYYY-MM-DDTHH:mm:ss]
    strftime(time_str, 32, "[%FT%T]", localtime(&cur_time));
    fprintf(f, "%s ", time_str);

    // Log level
    fprintf(f, "%5s: ", logLevelToStr(level));

    // Print log message
    vfprintf(f, fmt, args);

    fprintf(f, "\n");
    fflush(f);

    va_end(args);
}

void log_game_state(Game *game) {
    if (game == NULL) {
        fprintf(logfile, "log_game_state: Game ptr is NULL!");
        return;
    }

    char cipher_preview[20] = { '\0' };
    strncpy(cipher_preview, game->ciphertext, 16);
    strcpy(&cipher_preview[16], "...");

    char key_preview[20] = { '\0' };
    for (size_t i=0; (i < game->key_size) && (i < 8); ++i) {
        sprintf(&key_preview[i*2], "%02" PRIX8, ((uint8_t *)game->key)[i]);
    }
    strcpy(&cipher_preview[16], "...");

    char time_str[16] = { '\0' };
    strftime(time_str, 16, "%a %T", localtime(&game->start_time));

    fprintf(logfile,
            "Game @ %p\n"
            "  .key = [%p] %s\n"
            "  .ciphertext = [%p] \"%s\"\n"
            "  .buffer_size = %zu\n"
            "  .answer_hash = %" PRIX32 "\n"
            "  .paused = %s\n"
            "  .start_time = %s\n"
            "  .extra_time = %.2F\n",
            game, game->key, key_preview, game->ciphertext, cipher_preview,
            game->buffer_size, game->answer_hash,
            game->paused ? "true" : "false", time_str, game->extra_time);
}

