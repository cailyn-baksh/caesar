#ifndef _CIPHER_H_
#define _CIPHER_H_

#include <stdint.h>

/*
 * Encodes a string in place using a caesar cipher with the given key. This is
 * similar to the shift cipher, except the caesar cipher only enciphers
 * alphanumeric characters.
 */
void caesar_cipher(char *text, uint8_t key);

/*
 * Encodes a string in place using a shift cipher with the given key
 */
void shift_cipher(char *text, uint8_t key);

/*
 * Encodes a string in place using a substitution cipher
 */
void subst_cipher(char *text);

#endif  // _CIPHER_H_

