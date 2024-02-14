#include <string.h>

#include <curses.h>

#include "log.h"
#include "ui.h"

void tab_focus(bool shift) {
    // Save widget to blur
    Widget *prevFocus = focused_widget;

    // Change focused widget
    if (shift) {
        // Shift-Tab: move backwards
        if (focused_widget == &widgets[0]) {
            // First widget is focused. Loop around to last widget
            focused_widget = &widgets[WIDGET_COUNT-1];
        } else {
            // Focus previous widget
            --focused_widget;
        }
    } else {
        // Tab: move forwards
        if (focused_widget == &widgets[WIDGET_COUNT-1]) {
            // Last widget is focused. Loop around to first widget
            focused_widget = &widgets[0];
        } else {
            // Focus next widget
            ++focused_widget;
        }
    }

    // Send blur event to currently focused widget
    prevFocus->handler(prevFocus, EVENT_BLUR, nullptr);

    // Send focus event to newly focused widget
    focused_widget->handler(focused_widget, EVENT_FOCUS, nullptr);
}

void titled_box(WINDOW *win, const char *title) {
    // ACS_trbl where t, r, b, & l may be Blank, Single, Double, or Thick
    wborder(win, ACS_SBSB, ACS_SBSB, ACS_BSBS, ACS_BSBS, ACS_BSSB, ACS_BBSS, ACS_SSBB, ACS_SBBS);
    //box(win, 0, 0);

    mvwprintw(win, 0, 2, "%s", title);

    mvwaddch(win, 0, 1, ACS_SBBS);
    mvwaddch(win, 0, strlen(title) + 2, ACS_SSBB);
}

