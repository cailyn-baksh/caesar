#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <stddef.h>

/*
 * Allocates an appropriately sized buffer and populates it with a randomly
 * selected null-terminated cleartext string message.
 *
 * buffer       This will point to the buffer after the function returns. This
 *              buffer must be manually freed.
 * Returns the length of the buffer, including the null terminator.
 */
size_t getmsg(char **buffer);

#endif  // _MESSAGES_H_

