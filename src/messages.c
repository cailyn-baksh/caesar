#include <stdlib.h>
#include <string.h>

#include "messages.h"

const char msg[] = "Hello World!";

size_t getmsg(char **buffer) {
    size_t msglen = strlen(msg) + 1;

    *buffer = malloc(msglen * sizeof(char));
    strncpy(*buffer, msg, msglen);
    (*buffer)[msglen-1] = '\0';

    return msglen;
}

