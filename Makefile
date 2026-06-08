
CC = cc
CFLAGS = -Wall -Wextra

# include everything
IFLAGS = -Iinclude

# add in raylib
ADD_RAYLIB = /opt/homebrew/opt/raylib/lib/libraylib.a -I/opt/homebrew/opt/raylib/include
ADD_RAYLIB += -framework Cocoa
ADD_RAYLIB += -framework IOKit
ADD_RAYLIB += -framework CoreVideo
ADD_RAYLIB += -framework Corefoundation

# optimization
OPT = -O3 # this is perfectly fine!

#############
# build src #
#############

# get the source and obj files
SRCS =  $(wildcard src/*.c)
SRCS += $(wildcard src/**/*.c)
SRCS += $(wildcard src/**/**/*.c)
OBJS =  $(SRCS:.c=.o)

# target
TARGET = lib/libmicrogame.a

# build a single obj
src/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS) $(OPT) $(ADD_RAYLIB)

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
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS) $(OPT)

# build everything
test: $(T_OBJS) engine
	$(CC) -o $(T_TARGET) $(T_OBJS) $(CFLAGS) $(LINK_MICROGAME) $(ADD_RAYLIB) $(OPT)

###################
# first test game #
###################

# get the source and obj files
TG1_SRCS =  $(wildcard testgame1/*.c)
TG1_SRCS += $(wildcard testgame1/**/*.c)
TG1_SRCS += $(wildcard testgame1/**/**/.c)
TG1_OBJS =  $(TG1_SRCS:.c=.o)

# target
TG1_TARGET = tg1

# build a single obj
testgame1/%.o: testgame1/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS) $(OPT) $(ADD_RAYLIB)

# build everything
tg1: $(TG1_OBJS) engine
	$(CC) -o $(TG1_TARGET) $(TG1_OBJS) $(CFLAGS) $(LINK_MICROGAME) $(ADD_RAYLIB) $(OPT)

###################
# the actual editor! #
###################

# get the source and obj files
EDITOR_SRCS =  $(wildcard editor/*.c)
EDITOR_SRCS += $(wildcard editor/**/*.c)
EDITOR_SRCS += $(wildcard editor/**/**/*.c)
EDITOR_OBJS =  $(EDITOR_SRCS:.c=.o)

# target
EDITOR_TARGET = microgame

# build a single obj
editor/%.o: editor/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS) $(OPT) $(ADD_RAYLIB)

# build everything
editor: $(EDITOR_OBJS) engine
	$(CC) -o $(EDITOR_TARGET) $(EDITOR_OBJS) $(CFLAGS) $(LINK_MICROGAME) $(ADD_RAYLIB) $(OPT)

#####################
# General functions #
#####################

# clean it all up
.PHONY: clean
clean:
	rm -f -- $(T_OBJS) $(T_TARGET) $(OBJS) $(TARGET) $(TG1_OBJS) $(TG1_TARGET) $(EDITOR_OBJS) $(EDITOR_TARGET)
	@echo Clean done

# run it too
.PHONY: run
run: test
	./$(TARGET)