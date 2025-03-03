CC = gcc
CFLAGS = -Wall -O2 -lm -fPIC

BUILD = build
SRCS = $(shell find . -name '*.c' ! -name 'amath.c')
OBJS = $(patsubst ./%.c, $(BUILD)/%.o, $(SRCS))

TARGET_EXEC = amath
TARGET = libamath.so

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
