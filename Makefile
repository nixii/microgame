
UNAME_S = $(shell uname -s)

CC = clang
CFLAGS = -Wall -Wextra

LDFLAGS = -L/opt/homebrew/opt/raylib/lib -lraylib
IFLAGS = -I/opt/homebrew/opt/raylib/include

SRCS =  $(wildcard src/*.c)
SRCS += $(wildcard src/**/*.c)
SRCS += $(wildcard src/**/**/*.c)
OBJS =  $(SRCS:.c=.o)

TARGET = mg

# build individual objects
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS)

# build the whole thing
.PHONY: all
all: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(IFLAGS) $(LDFLAGS)

# build and run
.PHONY: run
run: all
	./$(TARGET)

# clean it up
.PHONY: clean
clean:
	rm $(OBJS) $(TARGET)