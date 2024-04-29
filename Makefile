CC = clang

CFLAGS = -Wall -Werror -std=c11
DEBUG_CFLAGS = $(CFLAGS) -g -O0
REALESE_CFLAGS = $(CFLAGS) -Ofast

SOURCES_DIR = src
SOURCES = $(wildcard $(SOURCES_DIR)/*.c)

BUILD_DIR = build
DEBUG_TARGET := $(BUILD_DIR)/debug
RELEASE_TARGET := $(BUILD_DIR)/release

LIBS = -lm -lraylib

ifeq ($(shell uname -s),Darwin)
	LIBS += -framework IOKit -framework Cocoa -framework OpenGL
endif

all: dirs
	@$(CC) $(REALESE_CFLAGS) $(LIBS) $(SOURCES) -o $(RELEASE_TARGET)
	@echo "Compiled successfully!"

debug: dirs
	@$(CC) $(DEBUG_CFLAGS) $(LIBS) $(SOURCES) -o $(DEBUG_TARGET)
	@echo "Compiled successfully!"

run: all
	@./$(RELEASE_TARGET)

dirs:
	@mkdir -p build

clear:
	@rm -rf $(BUILD_DIR)
