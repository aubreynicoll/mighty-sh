P := main
CC := gcc
CFLAGS := -I$(HOME)/local/include -fms-extensions -Wall -Wextra
LDFLAGS := -L$(HOME)/local/lib
LDLIBS :=

# use gcc-12 if running macOS since gcc is clang alias
ifeq ($(shell uname), Darwin) 
	CC := gcc-12
endif

ifeq ($(BUILD_PROFILE), release)
	CFLAGS := $(CFLAGS) -O3 -Werror
else
	CFLAGS := $(CFLAGS) -g -Og -DDEBUG_MODE
endif


ifeq ($(ENABLE_GPROF), 1)
	CFLAGS := $(CFLAGS) -pg
	LDFLAGS := $(LDFLAGS) -pg
endif


$(P): $(patsubst %.c,%.o,$(wildcard *.c))

clean:
	rm -rf $(P) *.o
