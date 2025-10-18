# Pintos Threads Makefile
# Builds all .c files in this directory

BUILD_DIR = build
CC = i386-elf-gcc
CFLAGS = -Wall -g -O -std=gnu99 -I../lib -I../devices -I../threads -I.

CFILES = $(wildcard *.c)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(CFILES))

all: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.bin: $(OBJS)
	i386-elf-ld -o $@ $(OBJS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

