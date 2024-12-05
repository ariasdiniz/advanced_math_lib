CC = gcc
CFLAGS = -Wall -O2 -lm -fPIC

BUILD = build
SRCS = $(shell find . -name '*.c')
OBJS = $(patsubst ./%.c, $(BUILD)/%.o, $(SRCS))

TARGET = libamath.so

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -shared -o $@ $(OBJS) $(CFLAGS)

$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -f $(TARGET)
