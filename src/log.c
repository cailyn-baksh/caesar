#include <inttypes.h>
#include <string.h>
#include <time.h>

#include "log.h"

#define INDENT "  "

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
            INDENT ".mode = %p\n"
            INDENT ".key = [%p] %s\n"
            INDENT ".ciphertext = [%p] \"%s\"\n"
            INDENT ".buffer_size = %zu\n"
            INDENT ".answer_hash = %" PRIX32 "\n"
            INDENT ".paused = %s\n"
            INDENT ".start_time = %s\n"
            INDENT ".extra_time = %.2F\n",
            game, game->mode, game->key, key_preview, game->ciphertext,
            cipher_preview, game->buffer_size, game->answer_hash,
            game->paused ? "true" : "false", time_str, game->extra_time);
}

