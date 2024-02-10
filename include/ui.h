#ifndef _UI_H_
#define _UI_H_

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
    UIEVENT_CLOSE,
    UIEVENT_DRAW,
    UIEVENT_KEYPRESS
} UIEvent;

/*
 * Result signals that a UI callback function may return
 *
 * UIRESULT_OKAY        Success, no action needed.
 * UIRESULT_ERROR       Unspecified error
 * UIRESULT_REFRESH     Trigger a screen refresh
 */
typedef enum {
    UIRESULT_OKAY,
    UIRESULT_ERROR,
    UIRESULT_REFRESH
} UICbResult;

/*
 * A callback function for a UI screen.
 *
 * event        The event that triggered the callback
 * data         A pointer to data associated with the event. May be NULL.
 */
typedef UICbResult UICallback(UIEvent event, const void *const data);

#ifndef _NOEXTERN
extern UICallback *activeInterface;
#else
UICallback *activeInterface = NULL;
#endif  // _NOEXTERN

#endif  // _UI_H_

