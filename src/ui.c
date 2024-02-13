#include "log.h"
#include "ui.h"

void tab_focus(bool shift) {
    // Send blur event to currently focused widget
    focused_widget->handler(focused_widget, EVENT_BLUR, nullptr);

    // Change focused widget
    if (shift) {
        // Shift-Tab: move backwards
        if (focused_widget == &widgets[0]) {
            // First widget is focused. Loop around to last widget
            focused_widget = &widgets[WIDGET_COUNT-1];
        } else {
            // Focus previous widget
            focused_widget -= sizeof(Widget);
        }
    } else {
        // Tab: move forwards
        if (focused_widget == &widgets[WIDGET_COUNT-1]) {
            // Last widget is focused. Loop around to first widget
            focused_widget = &widgets[0];
        } else {
            // Focus next widget
            focused_widget += sizeof(Widget);
        }
    }

    // Send focus event to newly focused widget
    focused_widget->handler(focused_widget, EVENT_FOCUS, nullptr);
}

