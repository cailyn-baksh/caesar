#include <inttypes.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#include "cipher.h"
#include "log.h"
#include "messages.h"
#include "util.h"

#define _DEFEXTERN
#include "ui.h"
#undef _DEFEXTERN

#define FRAMERATE       24
#define FRAMEPERIOD     (1.0 / FRAMERATE)

#define KEY_CTRL(k) ((k) & 0x1F)

/*************************
 * Game struct functions *
 *************************/
void new_game(Game *game) {
    // Set all values to NULL
    memset(game, 0, sizeof(Game));

    // Initialize ciphertext and buffer_size with a cleartext message
    game->buffer_size = getmsg(&game->ciphertext);

    // Get a hash of the cleartext message
    game->answer_hash = fnv1a32(game->ciphertext);

    // Config game timer
    game->paused = false;

    // Set the game start time to the current time
    if (time(&game->start_time) == (time_t)-1) {
        // If getting the current time failed, set extra_time to infinity
        game->extra_time = INFINITY;
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

    // Jump to the correct event handler
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

create: /* Handle UI creation event */
    selection = 0;
    cipher_menu_win = newwin(nCiphers+2, 24, (LINES - nCiphers+2) / 2, (COLS - 24) / 2);
    return UIRESULT_OKAY;

destroy: /*  Handle UI destruction event*/
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
    // Draw cipher selection menu
    for (int i=0; i < 3; ++i) {
        if (i == selection) wstandout(cipher_menu_win);

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
    timeout(FRAMEPERIOD * 1000);  // getch blocks for up to one full frame
    keypad(stdscr, TRUE);
    curs_set(0);

    activeInterface = new_game_ui;

    /* Mainloop */

    // Init first ui
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

            // Handle global key events
            switch (ch) {
                case 27:
                    // Esc or alt
                    break;
                case KEY_CTRL('N'):
                    // New Game
                    break;
            }

            if (ch != ERR) {
                activeInterface(UIEVENT_KEYPRESS, &ch);
            }
        } while (timesince(&start_input, TIME_UTC) < FRAMEPERIOD);
    }

endprgm:
    // Clean up ncurses
    activeInterface(UIEVENT_DESTROY, NULL);

    endwin();

    return 0;
}

