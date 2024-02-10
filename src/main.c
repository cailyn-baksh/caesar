#include <inttypes.h>
#include <math.h>
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

#define _NOEXTERN
#include "ui.h"
#undef _NOEXTERN

#define FRAMERATE       5
#define FRAMEPERIOD     (1.0 / FRAMERATE)

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


UICbResult new_game_ui(UIEvent event, const void *const data) {
    const char *menu_options[] = {
        "Caesar Cipher",
        "Shift Cipher",
        "Substitution Cipher"
    };

    static int selection = 0;


    switch (event) {
        case UIEVENT_CREATE:
            // Initialize state variables
            selection = 0;
            break;
        case UIEVENT_DRAW:
            goto draw;
        case UIEVENT_KEYPRESS:
            goto keypress;
        default:
    }

    return UIRESULT_OKAY;


keypress: /* Handle keypress event */
    switch (*(int *)data) {
        case KEY_UP:
            --selection;
            break;
        case KEY_DOWN:
            ++selection;
            break;
    }

    if (selection > 2) selection = 0; else if (selection < 0) selection = 2;

    return UIRESULT_OKAY;


draw: /* Handle draw event */
    for (int i=0; i < 3; ++i) {
        if (i == selection) standout();

        mvprintw(i, 1, "%s", menu_options[i]);

        standend();
    }

    return UIRESULT_REFRESH;
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
    timeout(FRAMEPERIOD);
    keypad(stdscr, TRUE);
    curs_set(0);

    activeInterface = new_game_ui;

    /* Mainloop */
    for (;;) {
        /* Handle Global Input */
        struct timespec start_input;
        timespec_get(&start_input, TIME_UTC);

        int ch;

        do {
            ch = getch();

            switch (ch) {
                case ERR:
                    // No input was recieved
                    break;
                case 27:
                    // Esc or alt
                    break;
                case KEY_CTRL('N'):
                    // New Game
                    break;
            }

            switch (activeInterface(UIEVENT_KEYPRESS, &ch)) {
                case UIRESULT_ERROR:
                    // Report err
                    break;
                case UIRESULT_REFRESH:
                    refresh();
                    break;
            }
        } while (timesince(&start_input, TIME_UTC) < FRAMEPERIOD);

        /* Draw UI */
        switch (activeInterface(UIEVENT_DRAW, NULL)) {
            case UIRESULT_ERROR:
                // Report err
                break;
            case UIRESULT_REFRESH:
                refresh();
                break;
        }
    }

endprgm:
    // Clean up ncurses
    endwin();

    return 0;
}

