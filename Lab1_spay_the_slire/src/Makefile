# Detects what OS the Makefile is being run on.
ifeq ($(OS),Windows_NT)
	TARGET_OS := windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		TARGET_OS := macos
	else ifeq ($(UNAME_S),Linux)
		TARGET_OS := linux
	else
		$(info "Unknown OS.")
	endif
endif

# Sets the C++ compiler to use.
ifneq ($(TARGET_OS),macos)
	CXX := g++
else
	# This course officially uses GNU's C++ compiler for its teaching. In macOS
	# however, calling `g++` redirects to Clang's C++ compiler. If you installed
	# `gcc` from Homebrew, you should call `g++-XX`, where "XX" is the version
	# of GCC installed. As of writing this, the current version of GCC is
	# version 16. It may be lower (or even higher!) on your device.
	CXX := g++-16
endif

# Sets the executable name.
ifneq ($(TARGET_OS),windows)
	EXEC_NAME := lab1
else
	EXEC_NAME := lab1.exe
endif

# All `*.o` and `*.d` files will end up in this directory.
BUILD_DIR := build

# Automatically finds all `*.cpp` files.
SRCS := $(wildcard *.cpp)
# Creates `*.o` and `*.d` file names from the `*.cpp` file names.
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(SRCS:%.cpp=$(BUILD_DIR)/%.d)

# C++ compiler flags.
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
# C/C++ preprocessor flags.
CPPFLAGS := -MMD -MP

# Adds sanitizer flags to OSes that support it.
ifeq ($(TARGET_OS),macos)
	CXXFLAGS += -fsanitize=leak
else ifeq ($(TARGET_OS),linux)
	CXXFLAGS += -fsanitize=address,leak,undefined
endif

# Tells Make that "all" and "clean" are not file names.
.PHONY: all clean

# Run `make all` (or simply `make`) to compile the project.
all: $(EXEC_NAME)

# Links object files into a single executable file.
$(EXEC_NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(CXXFLAGS)

# Compiles each `*.cpp` file separately into their own `*.o` files.
$(BUILD_DIR)/%.o: %.cpp $(BUILD_DIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CPPFLAGS)

# Makes sure a `build/` directory exists.
ifneq ($(TARGET_OS),windows)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
else
$(BUILD_DIR):
	mkdir $(BUILD_DIR)
endif

# Run `make clean` to delete the executable and any other intermediate files.
ifneq ($(TARGET_OS),windows)
clean:
	-rm -rf $(BUILD_DIR)
	-rm -f $(EXEC_NAME)
else
clean:
	rmdir /s /q $(BUILD_DIR)
	del /s /q $(EXEC_NAME)
endif

# Includes all the `*.d` files generated during the compilation phase.
-include $(DEPS)
