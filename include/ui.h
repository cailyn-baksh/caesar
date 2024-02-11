#ifndef _UI_H_
#define _UI_H_

#include <stddef.h>

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

typedef enum {
    UIRESULT_OKAY
} UICbResult;

/*
 * A callback function for a UI screen.
 *
 * event        The event that triggered the callback
 * data         A pointer to data associated with the event. May be NULL.
 */
typedef UICbResult UICallback(UIEvent event, void *const data);

#ifndef _DEFEXTERN
/*
 * A pointer to the callback function for the currently active UI.
 */
extern UICallback *activeInterface;
#else
UICallback *activeInterface = NULL;
#endif  // _DEFEXTERN

#endif  // _UI_H_

