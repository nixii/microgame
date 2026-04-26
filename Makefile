
CC = cc
CFLAGS = -Wall -Wextra

# include everything
IFLAGS = -Iinclude

# add in raylib
ADD_RAYLIB = -L/opt/homebrew/opt/raylib/lib -lraylib -I/opt/homebrew/opt/raylib/include

#############
# build src #
#############

# get the source and obj files
SRCS =  $(wildcard src/*.c)
SRCS += $(wildcard src/**/*.c)
SRCS += $(wildcard src/**/**/.c)
OBJS =  $(SRCS:.c=.o)

# target
TARGET = lib/libmicrogame.a

# build a single obj
src/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS) $(ADD_RAYLIB)

# build everything
engine: $(OBJS)
	ar rcs $(TARGET) $(OBJS)

###########
# testbed #
###########

# get the source and obj files
T_SRCS =  $(wildcard testbed/*.c)
T_SRCS += $(wildcard testbed/**/*.c)
T_SRCS += $(wildcard testbed/**/**/.c)
T_OBJS =  $(T_SRCS:.c=.o)

# target
T_TARGET = test

# target includes/libs
LINK_MICROGAME = -Llib -lmicrogame

# build a single obj
testbed/%.o: testbed/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS)

# build everything
test: $(T_OBJS) engine
	$(CC) -o $(T_TARGET) $(T_OBJS) $(CFLAGS) $(LINK_MICROGAME) $(ADD_RAYLIB)

#####################
# General functions #
#####################

# clean it all up
.PHONY: clean
clean:
	rm -f -- $(T_OBJS) $(T_TARGET) $(OBJS) $(TARGET)
	@echo Clean done

# run it too
.PHONY: run
run: test
	./$(TARGET)