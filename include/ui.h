#ifndef _UI_H_
#define _UI_H_

#include <stddef.h>

// TODO: rework UI interface into something nicer from the mainloop side
// (maybe something like adding a query event so that UI status only needs to
// be checked once/loop?)

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

/*
 * Events that a UI callback function may recieive
 *
 * UIEVENT_CREATE       Create a new instance of the interface
 * UIEVENT_CLOSE        Close the interface
 * UIEVENT_DRAW         Draw a frame. Windows should be updated using
 *                      wnoutrefresh(3X) as the mainloop will call doupdate(3X)
 *                      after it has finished its own drawing.
 * UIEVENT_KEYPRESS     Handle a keypress
 */
typedef enum {
    UIEVENT_CREATE,
    UIEVENT_DESTROY,
    UIEVENT_DRAW,
    UIEVENT_KEYPRESS
} UIEvent;

/*
 * Results that a UI callback function may return
 *
 * UIRESULT_ERROR       An error occurred which prevented the event from being
 *                      handled properly.
 * UIRESULT_OKAY        Event was handled and program state remains unchanged.
 * UIRESULT_DONE        The UI has successfully completed its task; the next UI
 *                      screen should be displayed.
 */
typedef enum {
    UIRESULT_ERROR,
    UIRESULT_OKAY,
    UIRESULT_DONE
} UICbResult;

/*
 * A callback function for a UI screen.
 *
 * event        The event that triggered the callback
 * data         A pointer to data associated with the event. May be NULL.
 */
typedef UICbResult UICallback(UIEvent event, void *const data);

#endif  // _UI_H_

