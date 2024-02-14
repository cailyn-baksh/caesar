#ifndef _CIPHER_H_
#define _CIPHER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

typedef struct Game Game;

/*
 * Pointer to a function that implements a cipher. The function modifies the
 * text in place.
 *
 * textbuf  A pointer to the text buffer containing cleartext to be enciphered.
 *          The data is enciphered in-place, mutating the buffer contents.
 * keybuf   A pointer to the buffer containing the cipher key. If the buffer is
 *          NULL, then a key is generated.
 * Returns the size of the key buffer.
 */
typedef size_t cipherfn(char *const textbuf, void **keybuf);

/*
 * Encodes a string in place using a caesar cipher with the given key. This is
 * similar to the shift cipher, except the caesar cipher only enciphers
 * alphanumeric characters.
 */
void caesar_cipher(Game *game);

/*
 * Encodes a string in place using a shift cipher with the given key
 */
void shift_cipher(char *text, uint8_t key);

/*
 * Encodes a string in place using a substitution cipher
 */
void subst_cipher(char *text);

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
struct Game {
    cipherfn *cipher;

    uint8_t *key;
    size_t key_size;

    char *ciphertext;
    size_t buffer_size;

    uint32_t answer_hash;

    bool paused;
    time_t start_time;
    double extra_time;
};

void new_game(Game *game);
void end_game(Game *game);

typedef struct Cipher {
    char *name;
    cipherfn *func;
} Cipher;

#define CIPHER_COUNT 3

// Globals
#if (defined(_DEFINE_GLOBALS) && !defined(_CIPHER_H_GLOBALS_DEFINED) && __INCLUDE_LEVEL__ == 1) || __INCLUDE_LEVEL__ == 0

#define _CIPHER_H_GLOBALS_DEFINED

Cipher ciphers[CIPHER_COUNT] = {
    {
        .name = "Caesar Cipher",
        .func = nullptr
    },
    {
        .name = "Shift Cipher",
        .func = nullptr
    },
    {
        .name = "Substitution Cipher",
        .func = nullptr
    }
};
#elifndef _CIPHER_H_GLOBALS_DEFINED
extern Cipher ciphers[CIPHER_COUNT];
#endif

#endif  // _CIPHER_H_

