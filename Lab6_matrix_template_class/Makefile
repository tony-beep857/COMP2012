CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra -Werror=vla

BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = .
MAIN_OBJS = $(BUILD_DIR)/main.o
MAIN_DEPS = $(MAIN_OBJS:.o=.d)

ifeq ($(OS),Windows_NT)
    SANITIZER_FLAGS =
    MKDIR = if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)" & if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
    RM_CMD = powershell.exe -Command Remove-Item -Recurse -Force -ErrorAction SilentlyContinue
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Linux)
        SANITIZER_FLAGS = -fsanitize=address,leak,undefined
    else ifeq ($(UNAME),Darwin)
        SANITIZER_FLAGS = -fsanitize=address,undefined
    endif
    MKDIR = mkdir -p $(BIN_DIR) $(BUILD_DIR)
    RM_CMD = rm -rf
endif

.PHONY: all main sanitize clean
all: main
main: $(BIN_DIR)/main

$(BIN_DIR)/main: $(MAIN_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SANITIZER_FLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SANITIZER_FLAGS) -MMD -MP -c $< -o $@

$(BIN_DIR) $(BUILD_DIR):
	$(MKDIR)

sanitize: clean main

clean:
	$(RM_CMD) $(BIN_DIR) $(BUILD_DIR)

-include $(MAIN_DEPS)
