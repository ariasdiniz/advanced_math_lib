CC = gcc-15
CFLAGS = -std=gnu17 -Wall -O3 -lm -fPIC -march=native

BUILD = build
SRCS = $(shell find . -name '*.c' ! -name 'amath.c')
OBJS = $(patsubst ./%.c, $(BUILD)/%.o, $(SRCS))

TARGET_EXEC = amath
SYSTEM = $(shell uname -s)

ifeq ($(SYSTEM), Darwin)
	TARGET = libamath.dylib
else ifeq ($(SYSTEM), Linux)
	TARGET = libamath.so
else
$(error This library compiles only for Linux and MacOS systems.)
endif

all: $(TARGET) $(TARGET_EXEC)

$(TARGET_EXEC): amath.c $(OBJS)
	$(CC) -c amath.c -o $(BUILD)/amath.o $(CFLAGS)
	$(CC) -o amath amath.h $(BUILD)/amath.o $(OBJS) $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -shared -o $@ $(OBJS) $(CFLAGS)

$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

clean: 	
	rm -rf $(BUILD)
	rm -f $(TARGET)
	rm -f $(TARGET_EXEC)
