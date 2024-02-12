#include "log.h"
#include "ui.h"

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
    self->win = newwin(1, 1, 0, 0);
    return;

destroy:
    delwin(self->win);
    return;

draw:
    mvwprintw(self->win, 2, 0, "h");

    wrefresh(self->win);
    //wnoutrefresh(self->win);
    return;
}
