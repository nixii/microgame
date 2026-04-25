
UNAME_S = $(shell uname -s)

CC = clang
CFLAGS = -Wall -Wextra

LDFLAGS = -L/opt/homebrew/opt/raylib/lib -lraylib -L./lib -lmicrogame -lmicroengine
IFLAGS = -I/opt/homebrew/opt/raylib/include -I./include

# runtime objects
RT_SRCS =  $(wildcard runtime/*.c)
RT_SRCS += $(wildcard runtime/**/*.c)
RT_SRCS += $(wildcard runtime/**/**/*.c)
RT_OBJS =  $(RT_SRCS:.c=.o)
RT_TARGET = ./lib/libmicrogame.a

# build microgame
microgame: $(RT_OBJS)
	ar rcs $(RT_TARGET) $(RT_OBJS)

# engine objects
EG_SRCS =  $(wildcard engine/*.c)
EG_SRCS += $(wildcard engine/**/*.c)
EG_SRCS += $(wildcard engine/**/**/*.c)
EG_OBJS =  $(EG_SRCS:.c=.o)
EG_TARGET = ./lib/libmicroengine.a

# build microengine
microengine: $(EG_OBJS)
	ar rcs $(EG_TARGET) $(EG_OBJS)

# testbed objects
TB_SRCS = $(wildcard test/*.c)
TB_OBJS = $(TB_SRCS:.c=.o)
TB_TARGET = tb

# build the testbed
test: $(TB_OBJS) microgame
	$(CC) -o $(TB_TARGET) $(TB_OBJS) $(LDFLAGS) $(IFLAGS) $(CFLAGS)

# build and run the test
.PHONY: run
run: test
	@echo
	./$(TB_TARGET)

# build individual objects
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS)

# clean up
.PHONY: clean
clean:
	rm $(RT_OBJS) $(TB_OBJS) $(EG_OBJS) $(RT_TARGET) $(TB_TARGET) $(EG_TARGET)
