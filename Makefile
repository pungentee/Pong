CC = clang
DEBUG_FLAGS = -Wall -Werror -g -O0
REALESE_FLAGS = -Wall -Werror -0fast

SOURCE:=$(wildcard *.c)

BUILD_DIR = build
EXEC = Pong

LIBS = -lm -lraylib

ifeq ($(shell uname -s),Darwin)
	LIBS += -framework IOKit -framework Cocoa -framework OpenGL
endif

all:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(REALESE_FLAGS) $(LIBS) -o $(BUILD_DIR)/$(EXEC) $(SOURCE)
	@echo "Successfully compiled!"

run: all
	@./$(BUILD_DIR)/$(EXEC)

debug:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(DEBUG_FLAGS) $(LIBS) -o $(BUILD_DIR)/$(EXEC) $(SOURCE)
	@echo "Successfully compiled!"

clear:
	@rm -rf $(BUILD_DIR)
