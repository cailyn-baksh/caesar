# Caesar

A code-breaking game.

**Current Limitations**
- ASCII only
- Cipher keys restricted to 1 byte

## Building

**Requirements**
- GNU Make
- C23 with GNU Extension (`-std=gnu2x`)
- Ncurses
- Libconfig

## v1.0.0 Features/Todo list &mdash; 3.7% (1/26)

- [ ] Curses UI
  - [ ] UI framework
  - [ ] Mainloop
  - [ ] New game menu
  - [ ] Show decrypted text overlaid/side-by-side as progress is made
- [ ] Ciphers
  - [x] Caesar cipher
  - [ ] ASCII shift cipher
  - [ ] Substitution cipher
  - [ ] XOR cipher
- [ ] Messages
  - [ ] Public Domain literature source texts
  - [ ] User-supplied text from file
  - [ ] Selectable length & challenge rating for messages
- [ ] Helper tools
  - [ ] ASCII table
  - [ ] Hex viewer
  - [ ] Frequency analyzer
- [ ] User config
- [ ] Documentation
  - [ ] Developer documentation
    - [ ] Header docs (high-level overview)
    - [ ] Source code docs (detailed)
  - [ ] User docs
    - [ ] README.md
    - [ ] Program usage/navigation/keybind help
    - [ ] How to play


## Future Features

- UTF-8 support

