#ifndef _UI_H_
#define _UI_H_

#include <stddef.h>

/*
 * Events that a UI callback function may recieive
 *
 * UIEVENT_CREATE       Create a new instance of the interface
 * UIEVENT_CLOSE        Close the interface
 * UIEVENT_DRAW         Draw a frame
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

#ifndef _NOEXTERN
extern UICallback *activeInterface;
#else
UICallback *activeInterface = NULL;
#endif  // _NOEXTERN

#endif  // _UI_H_

