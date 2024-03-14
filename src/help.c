#include <ncurses.h>

#include "log.h"
#include "resource.h"
#include "ui.h"
#include "util.h"

void helpScreenHandler(Widget *const self, Event event, const void *const data) {
    print_log_src(LOG_DEBUG, "Widget recieved event code 0x%X with data ptr %p", event, data);
    switch (event) {
        case EVENT_CREATE:
            self->win = newwin(20, 20, 0, 0);
            self->enabled = true;
            break;
        case EVENT_DESTROY:
            delwin(self->win);
            break;
        case EVENT_DRAW:
            mvwnputw(self->win, 0, 0, res_keymap_size, _binary_res_keymap_txt_start);

            // Update virtual screen
            wnoutrefresh(self->win);
            break;
    }
    return;
}
