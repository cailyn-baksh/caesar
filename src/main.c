#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ncurses.h>

#include "cipher.h"
#include "log.h"
#include "messages.h"
#include "util.h"

#define KEY_CTRL(k) ((k) & 0x1F)

void new_game(Game *game) {
    game->mode = NULL;

    game->key = NULL;

    game->ciphertext = NULL;
    game->buffer_size = getmsg(&game->ciphertext);

    game->answer_hash = fnv1a32(game->ciphertext);

    // Config game timer
    game->paused = false;

    if (time(&game->start_time) == (time_t)-1) {
        game->extra_time = INFINITY;
    } else {
        game->extra_time = 0;
    }
}

void end_game(Game *game) {
    free(game->key);
    free(game->ciphertext);
}


void new_game_ui(int ch) {
    static size_t selection = 0;

    /* Handle UI-local input */

    /* Draw UI */
}

void play_game_ui() {
}


int main() {
    Game game;

    // Seed RNG
    srand(time(NULL));

    // Init ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Mainloop */
    for (;;) {
        /* Handle Global Input */
        int ch = getch();

        switch (ch) {
            case KEY_CTRL('N'):
                // New Game
                printw("New Game!\n");
                break;
        }

        /* Draw UI */
        new_game_ui(ch);
    }

endprgm:
    // Clean up ncurses
    endwin();

    return 0;
}

