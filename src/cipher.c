#include <stdio.h>
#include <stdlib.h>

#include "cipher.h"
#include "util.h"

void caesar_cipher(Game *game) {
    if (game->key == nullptr) {
        // Generate key
        game->key_size = sizeof(uint8_t);
        game->key = malloc(game->key_size);
        deref_as(uint8_t, game->key) = rand();
    }

    char *text = game->ciphertext;

    for (char ch; (ch = *text); ++text) {
        char base, mod;

        if (ch >= '0' && ch <= '9') {
            // Number
            base = '0';
            mod = 10;
        } else if (ch >= 'A' && ch <= 'Z') {
            // Upper alpha
            base = 'A';
            mod = 26;
        } else if (ch >= 'a' && ch <= 'z') {
            // Lower alpha
            base = 'a';
            mod = 26;
        } else {
            // Ignore this char
            continue;
        }

        *text = (((ch - base) + deref_as(uint8_t, game->key)) % mod) + base;
    }
}

void shift_cipher(char *text, uint8_t key) {
    while (*text) {
        putchar(*text);
        *text += key;
        putchar(*text);
        putchar('\n');
        ++text;
    }
}
