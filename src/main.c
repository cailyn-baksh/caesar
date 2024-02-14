#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#define _DEFINE_EXTERN
#include "cipher.h"
#include "ui.h"
#undef _DEFINE_EXTERN

#include "log.h"
#include "messages.h"
#include "util.h"

// Although the screen only updates automatically 5 times every second, input
// events can trigger a redraw more frequently.
#define FRAMERATE       5
#define FRAMEPERIOD     (1.0 / FRAMERATE)

#define KEY_CTRL(k) ((k) & 0x1F)

static Game game;

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
    static size_t cursor = 0;

    print_log_src("Widget %04hXh recieved event code 0x%X with data ptr %p", self->id, event, data);

    switch (event) {
        case EVENT_CREATE:
            goto create;
        case EVENT_DESTROY:
            goto destroy;
        case EVENT_FOCUS:
        case EVENT_BLUR:
            break;
        case EVENT_DRAW:
            goto draw;
        case EVENT_KEYPRESS:
            goto keypress;
    }

    print_log_src("Widget %04hXh has no handler for this event. Ignoring.", self->id);

    return;

create:
    self->win = newwin(CIPHER_COUNT+2, 24, 2, 0);
    self->enabled = true;

    self->data = malloc(sizeof(size_t));
    deref_as(size_t, self->data) = cursor;
    return;

destroy:
    delwin(self->win);
    free(self->data);
    return;

keypress:

    switch (deref_as(int, data)) {
        case KEY_UP:
            if (cursor != 0)
                --cursor;
            else
                cursor = CIPHER_COUNT-1;
            break;
        case KEY_DOWN:
            if (cursor < CIPHER_COUNT-1)
                ++cursor;
            else
                cursor = 0;
            break;
        case '\n':
            deref_as(size_t, self->data) = cursor;
            break;
    }

    return;

draw:
    // Draw the registeredcipher;s names as a ligt
    for (size_t i=0; i < CIPHER_COUNT; ++i) {
        int attrs = 0;

        if (isfocused(self) && cursor == i) attrs |= A_STANDOUT;
        if (deref_as(size_t, self->data) == i) attrs |= A_BOLD;
        wattron(self->win, attrs);

        mvwprintw(self->win, i+1, 1, "%s", ciphers[i].name);

        wattroff(self->win, attrs);
    }

    titled_box(self->win, "Ciphers");

    // Update virtual screen
    wnoutrefresh(self->win);

    return;
}

void startGameBtnHandler(Widget *const self, Event event, const void *const data) {
    print_log_src("Widget %04hXh recieved event code 0x%X with data ptr %p", self->id, event, data);

    switch (event) {
        case EVENT_CREATE:
            goto create;
        case EVENT_DESTROY:
            goto destroy;
        case EVENT_FOCUS:
        case EVENT_BLUR:
            break;
        case EVENT_DRAW:
            goto draw;
        case EVENT_KEYPRESS:
            goto keypress;
    }

    print_log_src("Widget %04hXh has no handler for this event. Ignoring.", self->id);

    return;

create:
    self->win = newwin(1, 24, LINES - 3, 0);
    self->enabled = true;

    return;

destroy:
    delwin(self->win);
    return;

keypress:
    switch (deref_as(int, data)) {
        case '\n':
            // Pressed
            break;
    }

    return;

draw:
    ;
    int attrs = A_BOLD;

    if (isfocused(self)) attrs |= A_REVERSE;

    wattron(self->win, attrs);
    mvwprintw(self->win, 0, 0, "Start Game!");
    wattroff(self->win, attrs);

    // Update virtual screen
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
    print_log_src("Initializing ncurses");

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    // Set getch to block for up to one frame duration
    timeout(FRAMEPERIOD * 1000);

    // Create widgets
    for (size_t i=0; i < WIDGET_COUNT; ++i) {
        print_log_src("Create widget %04hXh", widgets[i].id);
        widgets[i].handler(&widgets[i], EVENT_CREATE, nullptr);
    }

    size_t focus_index = 0;

    // Send focus event to the initially focused widget
    widgets[focus_index]
        .handler(&widgets[focus_index], EVENT_FOCUS, nullptr);

    /* Mainloop */
    print_log_src("Begin mainloop");
    for (;;) {
        /* Draw UI */

        // Draw toplevel UI elements
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
        int ch = getch();  // Blocks for at most FRAMEPERIOD seconds

        // Process input if it as recieved
        if (ch != ERR) {
            print_log("KEY [%d] %s", ch, keyname(ch), ch);

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

                case KEY_CTRL('Q'):  // TODO: change to a better key
                    goto cleanup;
            }

            // UI-specific key events
            widgets[focus_index]
                .handler(&widgets[focus_index], EVENT_KEYPRESS, &ch);
        }
    }

cleanup: /* Clean up */
    // Ncurses cleanup
    for (size_t i=0; i < WIDGET_COUNT; ++i) {
        print_log_src("Destroy widget %04hXh", widgets[i].id);
        widgets[i].handler(&widgets[i], EVENT_DESTROY, nullptr);
    }

    print_log_src("Exit curses mode");
    endwin();

    // Logging cleanup
    close_log();

    return 0;
}

