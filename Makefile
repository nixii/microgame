
UNAME_S = $(shell uname -s)

# just normal compiler
CC = clang
CFLAGS = -Wall -Wextra

# links and includes for raylib
RL_LDFLAGS = -L/opt/homebrew/opt/raylib/lib -lraylib
RL_IFLAGS = -I/opt/homebrew/opt/raylib/include

# include the custom headers
IFLAGS = -Iinclude

# link the stuff
MICRO_LINKDIR = -Llib
RUNTIME_LINK = -lmicrogame
RENDERER_LINK = -lmicrorender
ENGINE_LINK = -lmicroengine

# final destinations
TARGET_TEST = test
TARGET_MICROGAME = microgame
TARGET_MICRORUNTIME = lib/microgame.a
TARGET_MICROENGINE = lib/libmicroengine.a
TARGET_MICRORENDER = lib/libmicrorender.a

##############
# TESTBED    #
##############

# source files
TEST_SRCS = $(wildcard testbed/*.c)
TEST_OBJS = $(TEST_SRCS:.c=.o)

# compile a test source
testbed/%.o: runtime/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS)

# compile the whole test thing
$(TARGET_TEST): $(TEST_OBJS) runtime
	$(CC) -o $(TARGET_TEST) $(TEST_OBJS) $(CFLAGS)

##############
# RUNTIME    #
##############

# source files
RUNTIME_SRCS = $(wildcard runtime/*.c)
RUNTIME_OBJS = $(RUNTIME_SRCS:.c=.o)

# compile a test source
runtime/%.o: runtime/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(RL_LDFLAGS) $(RL_IFLAGS) $(IFLAGS)

# compile the whole test thing
runtime: $(RUNTIME_OBJS) engine
	ar rcs $(TARGET_MICRORUNTIME) $(RUNTIME_OBJS)

##############
# ENGINE     #
##############

# source files
ENGINE_SRCS =  $(wildcard engine/*.c)
ENGINE_SRCS += $(wildcard engine/**/*.c)
ENGINE_OBJS = $(ENGINE_SRCS:.c=.o)

# compile a test source
engine/%.o: engine/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS)

# compile the whole test thing
engine: $(ENGINE_OBJS)
	ar rcs $(TARGET_MICROENGINE) $(ENGINE_OBJS)

##############
# RENDERER   #
##############

# source files
RENDER_SRCS = $(wildcard renderer/*.c)
RENDER_OBJS = $(RENDER_SRCS:.c=.o)

# compile a test source
renderer/%.o: renderer/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(IFLAGS)

# compile the whole test thing
renderer: $(RENDER_OBJS)
	ar rcs $(TARGET_MICRORENDER) $(RENDER_OBJS)

##############
# CLEAN      #
##############
.PHONY: clean
clean:
	rm -f $(TEST_OBJS) $(TARGET_TEST)
	rm -f $(RUNTIME_OBJS) $(TARGET_MICRORUNTIME)
	rm -f $(ENGINE_OBJS) $(TARGET_MICROENGINE)
	rm -f $(RENDER_OBJS) $(TARGET_MICRORENDER)
	@echo Clean completed.