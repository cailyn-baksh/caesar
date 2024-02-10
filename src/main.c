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

/*************************
 * Game struct functions *
 *************************/
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

/************************
 * UI Element Callbacks *
 ************************/

UICbResult new_game_ui(UIEvent event, void *const data) {
    const size_t nCiphers = 3;
    const char *cipher_menu_options[] = {
        "Caesar Cipher",
        "Shift Cipher",
        "Substitution Cipher"
    };

    static int selection = 0;

    static WINDOW *cipher_menu_win;

    switch (event) {
        case UIEVENT_CREATE:
            goto create;
        case UIEVENT_DESTROY:
            goto destroy;
        case UIEVENT_DRAW:
            goto draw;
        case UIEVENT_KEYPRESS:
            goto keypress;
        default:
    }

    return UIRESULT_OKAY;

create:
    selection = 0;
    cipher_menu_win = newwin(nCiphers+2, 24, (LINES - nCiphers+2) / 2, (COLS - 24) / 2);
    return UIRESULT_OKAY;

destroy:
    delwin(cipher_menu_win);
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

    wrefresh(cipher_menu_win);  // Trigger immediate redraw of modified window
    return UIRESULT_OKAY;


draw: /* Handle draw event */
    for (int i=0; i < 3; ++i) {
        if (i == selection) wstandout(cipher_menu_win);

        //mvprintw(i, 1, "%s", cipher_menu_options[i]);
        mvwprintw(cipher_menu_win, i+1, 1, "%s", cipher_menu_options[i]);

        wstandend(cipher_menu_win);
    }
    box(cipher_menu_win, 0, 0);

    wnoutrefresh(cipher_menu_win);

    return UIRESULT_OKAY;
}

void play_game_ui() {
}


/********
 * Main *
 ********/
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
    activeInterface(UIEVENT_CREATE, NULL);

    for (;;) {
        /* Draw UI */
        activeInterface(UIEVENT_DRAW, NULL);
        doupdate();

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

            activeInterface(UIEVENT_KEYPRESS, &ch);
        } while (timesince(&start_input, TIME_UTC) < FRAMEPERIOD);
    }

endprgm:
    // Clean up ncurses
    activeInterface(UIEVENT_DESTROY, NULL);

    endwin();

    return 0;
}

