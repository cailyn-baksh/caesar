NAME = caesar
VERSION = v0.1.0
SRCS = $(wildcard src/*.c)
INCLUDES = include/
LIBS = ncurses

OPTIM = -O3 -flto
DEFS = 'VERSION="$(VERSION)"'
WARNINGS = all extra no-char-subscripts no-switch
CFLAGS = -std=gnu2x $(addprefix -W,$(WARNINGS)) $(OPTIM) $(addprefix -D,$(DEFS))

all: $(patsubst src/%,bin/%.o,$(SRCS))
	$(CC) $(CFLAGS) -o bin/$(NAME) $^ $(addprefix -l,$(LIBS))

debug: OPTIM = -O0
debug: DEFS += DEBUG
debug: WARNINGS += no-unused-label no-unused-variable
debug: CFLAGS += -g
debug: all

bin/%.c.o: src/%.c | bin
	$(CC) $(CFLAGS) $(addprefix -I,$(INCLUDES)) -c -o $@ $^

bin:
	mkdir -p bin

clean:
	find bin/* -type f -delete

.PHONY: build clean FORCE bin
FORCE:

