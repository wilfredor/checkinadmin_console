APP := checkin
SRC := adm.c lang.c
OBJ := $(SRC:.c=.o)

# Auto-pick a sensible compiler; can be overridden: `make CC=gcc`.
UNAME_S := $(shell uname -s)
DEFAULT_CC := gcc
ifeq ($(UNAME_S),Darwin)
DEFAULT_CC := clang
endif
CC ?= $(DEFAULT_CC)

CFLAGS ?= -std=c11 -Wall -Wextra -pedantic
CFLAGS_DEBUG := -O0 -g3
CFLAGS_RELEASE := -O2
LDFLAGS ?=

.PHONY: all release debug mac linux win bin help clean

all: release
release: CFLAGS += $(CFLAGS_RELEASE)
release: $(APP)

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: $(APP)

mac: CC := clang
mac: release

linux: CC := gcc
linux: release

bin: release
	@mkdir -p bin
	@cp $(APP) bin/$(APP)-$(UNAME_S)
	@[ -f $(APP).exe ] && cp $(APP).exe bin/$(APP)-windows.exe || true

$(APP): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Build a Windows binary with MinGW (requires a MinGW toolchain in PATH).
win: CC := x86_64-w64-mingw32-gcc
win: APP := $(APP).exe
win: release

help:
	@echo "Targets: all (release), debug, mac, linux, win, bin, clean"
	@echo "Examples:"
	@echo "  make           # build release with auto CC"
	@echo "  make debug     # debug flags"
	@echo "  make mac       # force clang"
	@echo "  make linux     # force gcc"
	@echo "  make win       # cross-compile to Windows (MinGW)"
	@echo "  make bin       # drop binaries into ./bin (ignored by git)"

clean:
	$(RM) $(OBJ) $(APP) $(APP).exe bin/$(APP)-* bin/$(APP).exe bin/$(APP)
