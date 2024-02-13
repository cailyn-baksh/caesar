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

#define _DEFINE_EXTERN
#include "ui.h"
#undef _DEFINE_EXTERN

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
void cipherSelectHandler(Widget *const self, Event event, const void *const data) {
    // Is goto a good idea here?
    switch (event) {
        case EVENT_CREATE:
            print_log("Widget %04hXh recieved EVENT_CREATE", self->id);
            goto create;
        case EVENT_DESTROY:
            print_log("Widget %04hXh recieved EVENT_DESTROY", self->id);
            goto destroy;
        case EVENT_DRAW:
            print_log("Widget %04hXh recieved EVENT_DRAW", self->id);
            goto draw;
        default:
            print_log("Widget %04hXh recieved event %X", self->id, event);
    }

    return;

create:
    self->win = newwin(3, 5, 2, 0);
    return;

destroy:
    delwin(self->win);
    return;

draw:
    mvwprintw(self->win, 2, 0, "h");

    //wrefresh(self->win);
    wnoutrefresh(self->win);
    return;
}

/********
 * Main *
 ********/
int main() {
    // Init logging
    init_log("caesar.log");

    // Seed RNG
    srand(time(nullptr));

    /* Init ncurses */
    print_log("Initializing ncurses");

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    // Set getch to block for up to one frame duration
    timeout(FRAMEPERIOD * 1000);

    // Create widgets
    for (size_t i=0; i < WIDGET_COUNT; ++i) {
        print_log("Create widget %04hXh", widgets[i].id);
        widgets[i].handler(&widgets[i], EVENT_CREATE, nullptr);
    }

    size_t focus_index = 0;

    // Send focus event to the initially focused widget
    widgets[focus_index]
        .handler(&widgets[focus_index], EVENT_FOCUS, nullptr);

    /* Mainloop */
    print_log("Begin mainloop");
    for (;;) {
        /* Draw UI */

        // Draw toplevel UI elements
        mvprintw(0, 0, "k");

        wnoutrefresh(stdscr);

        // Draw visible widgets
        for (size_t i=0; i < WIDGET_COUNT; ++i) {
            if (widgets[i].visible == true) {
                widgets[i].handler(&widgets[i], EVENT_DRAW, nullptr);
            }
        }

        // Draw UI to terminal
        doupdate();

        /* Handle input */

        // Recieve input until it's time for a redraw
        struct timespec input_begin;
        timespec_get(&input_begin, TIME_UTC);

        do {
            int ch = getch();

            if (ch != ERR) print_log("Recieved key %d ('%c')", ch, ch);

            // Global key events
            switch (ch) {
                case 27:
                    // Esc or alt
                    break;

                case '\t':  // Tab forward
                case KEY_BTAB:  // Tab back
                    tab_focus(ch != '\t');
                    break;

                case KEY_CTRL('N'):
                    // New Game
                    break;

                case 'q':  // TODO: change to a better key
                    goto cleanup;
            }

            // UI-specific key events
            if (ch != ERR) {
                widgets[focus_index]
                    .handler(&widgets[focus_index], EVENT_KEYPRESS, &ch);
            }
        } while (timesince(&input_begin, TIME_UTC) < FRAMEPERIOD);
    }

cleanup: /* Clean up */
    // Ncurses cleanup
    for (size_t i=0; i < WIDGET_COUNT; ++i) {
        print_log("Destroy widget %04hXh", widgets[i].id);
        widgets[i].handler(&widgets[i], EVENT_DESTROY, nullptr);
    }

    print_log("Exit curses mode");
    endwin();

    // Logging cleanup
    close_log();

    return 0;
}

