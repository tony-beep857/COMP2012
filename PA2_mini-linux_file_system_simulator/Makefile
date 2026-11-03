# Compiler and flags
CXX = g++
CXXFLAGS = -MMD -MP -Wall -Wextra -Werror=vla -std=c++17 -g

# Executable name
TARGET = pa2.exe
TARGET_LEAK = pa2_leak.exe

# Source files
SRCS = main.cpp FileEntry.cpp File.cpp Directory.cpp FileSystem.cpp FileSystem-given.cpp Group.cpp User.cpp OperatingSystem.cpp OperatingSystem-given.cpp FileEntry-given.cpp Directory-given.cpp Device.cpp Diagnose.cpp
SUBMISSION = User.cpp Group.cpp FileEntry.cpp File.cpp Directory.cpp FileSystem.cpp OperatingSystem.cpp Device.cpp

# Convert to .d files
DEPS = $(SRCS:.cpp=.d)

# Convert to .o files
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)
leak: $(TARGET_LEAK)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_LEAK): $(OBJS)
	$(CXX) $(CXXFLAGS) -g -fsanitize=leak,address,undefined -o $@ $^

$(OBJS): Makefile
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp) -o $@

-include $(DEPS)

ifeq (Windows_NT, $(OS))
RM_CMD := del /F
ZIP_CMD := zip.exe
else
RM_CMD := rm -vf
ZIP_CMD := zip
endif

# Clean up
clean:
	$(RM_CMD) $(DEPS) $(OBJS) $(TARGET) $(TARGET_LEAK)

zinc: $(SUBMISSION)
	$(ZIP_CMD) pa2.zip $^

.PHONY: all clean
