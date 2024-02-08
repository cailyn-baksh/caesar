#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

/*
 * Represents a single game instance.
 *
 * mode         The gamemode.
 *
 * key          The cipher key
 *
 * ciphertext   Buffer containing the initial encrypted text. The buffer's
 *              contents are mutated in place.
 * buffer_size  Total length of the buffer.
 *
 * answer_hash  32-bit FNV-1a hash of the original text to check if the puzzle
 *              has been solved.
 *
 * start_time   The time the game was started/last unpaused at.
 * extra_time   Added to the time elapsed since start_time to account for
 *              pausing.
 */
typedef struct {
    void *mode;

    uint8_t key;

    char *ciphertext;
    size_t buffer_size;

    uint32_t answer_hash;

    bool paused;
    time_t start_time;
    double extra_time;
} Game;

#endif  // _GAME_H_

