NAME = caesar
VERSION = v0.2.0
SRCS = $(wildcard src/*.c)
RES = $(wildcard res/*)
INCLUDES = include/
LIBS = m ncurses

SRC_OBJS = $(patsubst src/%,bin/%.o,$(SRCS))
RES_OBJS = $(patsubst res/%,bin/%.res,$(RES))

CLEAN_PATTERNS = ./bin/* ./*.log

find_args = $(let first rest,$1,-path '$(first)' $(if $(rest),-o $(call find_args,$(rest))))

OPTIM = -O3 -flto
DEFS = 'VERSION="$(VERSION)"'
WARNINGS = all extra no-char-subscripts no-switch
CFLAGS = -std=gnu2x $(addprefix -W,$(WARNINGS)) $(OPTIM) $(addprefix -D,$(DEFS))

all: $(SRC_OBJS) $(RES_OBJS)
	$(CC) $(CFLAGS) -o bin/$(NAME) $^ $(addprefix -l,$(LIBS))

debug: OPTIM = -O0
debug: DEFS += DEBUG
debug: WARNINGS += no-unused-label no-unused-variable
debug: CFLAGS += -g
debug: all

sources-only: $(SRC_OBJS)
resources-only: $(RES_OBJS)

bin/%.c.o: src/%.c | bin
	$(CC) $(CFLAGS) $(addprefix -I,$(INCLUDES)) -c -o $@ $^

bin/%.res: res/% | bin
	ld -r -T resources.ld -o $@ $^

bin:
	mkdir -p bin

clean:
	find . \( $(call find_args,$(CLEAN_PATTERNS)) \) -type f -delete -printf "Delete %p\n"

.PHONY: build clean FORCE bin
FORCE:

