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

static Game game = {};

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

UICbResult _new_game_ui(UIEvent event, void *const data) {
    // TODO: communicate game settings back to main function
    // TODO: launch new game with selected settings
    // idea: make game struct a global variable (within translation unit) and
    // include game settings within it. standardize the cipher functions so
    // that they do not require the entire struct to be passed to them, &
    // manage them from the main function.
    // also, delegate more functionality to the mainloop & away from the ui
    // callback functions (especially ui sequencing)

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
        case KEY_ENTER:
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

UICbResult new_game_ui(UIEvent event, void *const data) {
    return UIRESULT_OKAY;
}

UICbResult play_game_ui(UIEvent event, void *const data) {
    return UIRESULT_OKAY;
}


/********
 * Main *
 ********/
int main() {
    UICallback *activeUI = new_game_ui;

    // Seed RNG
    srand(time(nullptr));

    // Init ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    // Set getch to block for up to one frame duration
    timeout(FRAMEPERIOD * 1000);

    /* Mainloop */

    // Init first UI
    activeUI(UIEVENT_CREATE, NULL);

    for (;;) {
        /* Draw UI */
        // Active UI screen
        switch (activeUI(UIEVENT_DRAW, NULL)) {}

        // App-global UI elements

        // Draw UI to terminal
        wnoutrefresh(stdscr);
        doupdate();

        /* Handle input */

        // Recieve input until it's time for a redraw
        struct timespec input_begin;
        timespec_get(&input_begin, TIME_UTC);

        do {
            int ch = getch();

            // Global key events
            switch (ch) {
                case 27:
                    // Esc or alt
                    break;
                case KEY_CTRL('N'):
                    // New Game
                    break;
            }

            // UI-specific key events
            if (ch != ERR) {
                switch (activeUI(UIEVENT_KEYPRESS, &ch)) {}
            }
        } while (timesince(&input_begin, TIME_UTC) < FRAMEPERIOD);
    }

    // Clean up ncurses
    endwin();

    return 0;
}

