/*
 * User Interface
 */
#ifndef _UI_H_
#define _UI_H_

#include <stddef.h>

#include <ncurses.h>

/*
 * UIs need to be able to
 * - Draw themselves
 * - Handle key events
 * - Handle mouse events, if supported
 * - Communicate complex data to the rest of the program
 *
 * Mainloop needs to
 * - Render UI elements
 * - Dispatch input (key & mouse) events
 * - Connect between UIs
 *
 * UI system 2.0:
 * - Wrapper structures around ncurses WINDOWs that provides
 *   - Visible/active property
 *   - Tab order
 *   - Event handler
 *   - Additional arbitrary data
 * - All wrapper structures are registered in a global const array
 * - Mainloop draws each 
 */

#if __INCLUDE_LEVEL__ == 0
// Header file is being compiled on its own (i.e. statically analyzed). Ensure
// that extern definitions are being analyzed instead of the declarations.
#define _DEFINE_EXTERN
#endif

/*
 * UI events
 *
 * EVENT_CREATE         Dispatched when an element is created. This event is
 *                      guaranteed to trigger before any other event.
 *
 * EVENT_DESTROY        Dispatched when an element is destroyed. An element
 *                      which has been destroyed may be reinitialized via a
 *                      create event.
 *
 * EVENT_DRAW           Indicates that the recieving element should draw its
 *                      contents to its window. Windows should be updated using
 *                      wnoutrefresh(3X) rather than wrefresh(3X) like other
 *                      events, as doupdate(3X) will be called by the mainloop
 *                      after all draws finish for better performance.
 *
 * EVENT_FOCUS          Dispatched when the user selects the element.
 *
 * EVENT_BLUR           Dispatched when the user deselects the element.
 *
 * EVENT_KEYPRESS       Triggered when the user presses a key while the element
 *                      is focused.
 *  data: int *         Points to an integer indicating which key was pressed.
 *
 * EVENT_MOUSE          Triggered when the user interacts with the element
 *                      using the mouse/cursor.
 *  data: MEVENT *      Points to a ncurses mouse event structure corresponding
 *                      to the triggering event.
 */
typedef enum {
    EVENT_CREATE,
    EVENT_DESTROY,
    EVENT_DRAW,
    EVENT_FOCUS,
    EVENT_BLUR,
    EVENT_KEYPRESS,
    EVENT_MOUSE
} Event;

/*
 * A pointer to an event handler callback function. This function handles
 * events dispatched to a Widget.
 *
 * self                 A pointer to the widget the event was triggered on.
 *                      This pointer may have a more specific type than void.
 * event                The event that was triggered
 * data                 Data associated with the event. This may be null.
 */
typedef void EventHandler(void *const self, Event event, const void *const data);

/*
 * A wrapper around an ncurses WINDOW.
 *
 * id                   An identifier number for this widget.
 *
 * win                  The underlying WINDOW
 *
 * visible              Whether or not the widget is currently to be rendered
 *                      to the screen.
 * enabled              Whether or not the widget can be selected via tabbing.
 *
 * handler              A pointer to the widget's event handler function
 * data                 Stores widget-specific data.
 */
typedef struct Widget {
    unsigned short id;

    WINDOW *win;

    bool visible;
    bool enabled;

    EventHandler *handler;
    void *data;
} Widget;

/*************************************
 * Widget Declarations & Definitions *
 *************************************/

#define WIDGET_COUNT 2

// Widget IDs
#define WIDGET_ANON                 0x0000
#define WIDGET_CIPHER_SELECT        0x0001
#define WIDGET_START_GAME_BTN         0x0002

// Widget handler functions
void cipherSelectHandler(Widget *const self, Event event, const void *const data);
void startGameBtnHandler(Widget *const self, Event event, const void *const data);

// Widget registry
#ifdef _DEFINE_EXTERN

#define _UI_H_EXTERNS_DEFINED

// Temporarily suppress incompatible pointer type warning, since event handlers
// can use specific pointer types for the self pointer
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

/*
 * Array of all the widgets
 */
Widget widgets[WIDGET_COUNT] = {
    {
        .id = WIDGET_CIPHER_SELECT,
        .visible = true,
        .handler = cipherSelectHandler
    },
    {
        .id = WIDGET_START_GAME_BTN,
        .visible = true,
        .handler = startGameBtnHandler
    }
};

#pragma GCC diagnostic pop

/*
 * Pointer to the widget which currently has user focus
 */
Widget *focused_widget = &widgets[0];

#elifndef _UI_H_EXTERNS_DEFINED
extern Widget widgets[WIDGET_COUNT];
extern Widget *focused_widget;
#endif  // _DEFINE_EXTERN


/************************
 * General UI Functions *
 ************************/

/*
 * Tests whether a widget is currently focused by the user.
 *
 * pWidget              A pointer to the widget to check.
 * Returns a boolean indicating whether or not the widget is in focus.
 */
#define isfocused(pWidget) (focused_widget == pWidget)

/*
 * Handle a Tab or Shift-Tab action by shifting the focus.
 *
 * shift                A boolean indicating which direction to tab in.
 */
void tab_focus(bool shift);

void titled_box(WINDOW *win, const char *title);

#endif  // _UI_H_

