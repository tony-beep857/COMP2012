# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g -MMD -MP -Wall -Wextra -pedantic-errors -Werror=pedantic -Iinclude -Itests
HARDENING_FLAGS = -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough -Werror=format-security -D_GLIBCXX_ASSERTIONS -fstack-protector-strong -fPIC
SANITIZE_FLAGS = -fsanitize=address,leak,undefined

ifeq (Windows_NT, $(OS))
HARDENING_FLAGS += -fstack-clash-protection -fcf-protection=full
LDFLAGS = -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va
else ifeq ($(shell uname -s),Darwin)
LDFLAGS = -fPIE -pie -Wl,-bind_at_load
else
HARDENING_FLAGS += -fstack-clash-protection -fcf-protection=full
LDFLAGS = -fPIE -pie -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now
endif

BUILD_DIR ?= build

TARGET = $(BUILD_DIR)/pkg
TARGET_SANITIZED = $(BUILD_DIR)/pkg.sanitized
TARGET_TEST = $(BUILD_DIR)/pkg_test
TARGET_TEST_SANITIZED = $(BUILD_DIR)/pkg_test.sanitized

# Sources
LIB_SRCS = src/util.cpp src/version.cpp src/package.cpp src/local_repository.cpp src/package_manager.cpp src/package_manager-given.cpp src/repository-given.cpp src/version-given.cpp
DRIVER_SRCS = src/main.cpp
FRAMEWORK_SRCS = tests/test.cpp tests/fixtures.cpp
GIVEN_SRCS = tests/given_tests.cpp

SUBMISSION = include/trie.tpp src/local_repository.cpp src/package_manager.cpp src/version.cpp

ALL_SRCS = $(LIB_SRCS) $(DRIVER_SRCS) $(FRAMEWORK_SRCS) $(GIVEN_SRCS) tests/main.cpp

SANITIZED_OBJS = $(addprefix $(BUILD_DIR)/,$(ALL_SRCS:.cpp=.sanitized.o))
SANITIZED_DEPS = $(SANITIZED_OBJS:.o=.d)

LIB_OBJS = $(addprefix $(BUILD_DIR)/,$(LIB_SRCS:.cpp=.o))
LIB_SANITIZED_OBJS = $(addprefix $(BUILD_DIR)/,$(LIB_SRCS:.cpp=.sanitized.o))
DRIVER_OBJS = $(addprefix $(BUILD_DIR)/,$(DRIVER_SRCS:.cpp=.o))
DRIVER_SANITIZED_OBJ = $(addprefix $(BUILD_DIR)/,$(DRIVER_SRCS:.cpp=.sanitized.o))
FRAMEWORK_OBJS = $(addprefix $(BUILD_DIR)/,$(FRAMEWORK_SRCS:.cpp=.o))
FRAMEWORK_SANITIZED_OBJS = $(addprefix $(BUILD_DIR)/,$(FRAMEWORK_SRCS:.cpp=.sanitized.o))
GIVEN_OBJS = $(addprefix $(BUILD_DIR)/,$(GIVEN_SRCS:.cpp=.o))
GIVEN_SANITIZED_OBJS = $(addprefix $(BUILD_DIR)/,$(GIVEN_SRCS:.cpp=.sanitized.o))
TEST_MAIN_OBJ = $(BUILD_DIR)/tests/main.o
TEST_MAIN_SANITIZED_OBJ = $(BUILD_DIR)/tests/main.sanitized.o

OBJS = $(LIB_OBJS) $(DRIVER_OBJS)
DEPS = $(OBJS:.o=.d)
TEST_DEPS = $(FRAMEWORK_OBJS:.o=.d) $(GIVEN_OBJS:.o=.d) $(TEST_MAIN_OBJ:.o=.d)

ifeq (Windows_NT, $(OS))
# Sanitizer runtimes are not available on Windows, so only the plain test binary is built.
ALL_SANITIZED_TARGETS =
TEST_EXECUTABLE = $(TARGET_TEST)
else
ALL_SANITIZED_TARGETS = $(TARGET_SANITIZED) $(TARGET_TEST_SANITIZED)
TEST_EXECUTABLE = $(TARGET_TEST_SANITIZED)
endif

all: $(TARGET) $(TARGET_TEST) $(ALL_SANITIZED_TARGETS)

sanitized: $(TARGET_SANITIZED)

# Case index or suite selector passed to the test binary; 0 runs every case.
# Override on the command line: make test TEST_CASE=117.
TEST_CASE ?= 0

test: $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE) $(TEST_CASE)

test-unsanitized: $(TARGET_TEST)
	$(TARGET_TEST) $(TEST_CASE)

$(TARGET): $(LIB_OBJS) $(DRIVER_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_TEST): $(LIB_OBJS) $(FRAMEWORK_OBJS) $(GIVEN_OBJS) $(TEST_MAIN_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(HARDENING_FLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_SANITIZED): $(LIB_SANITIZED_OBJS) $(DRIVER_SANITIZED_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(HARDENING_FLAGS) $(SANITIZE_FLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_TEST_SANITIZED): $(LIB_SANITIZED_OBJS) $(FRAMEWORK_SANITIZED_OBJS) $(GIVEN_SANITIZED_OBJS) $(TEST_MAIN_SANITIZED_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(HARDENING_FLAGS) $(SANITIZE_FLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp Makefile
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(HARDENING_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.sanitized.o: %.cpp Makefile
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(HARDENING_FLAGS) $(SANITIZE_FLAGS) -c $< -o $@

-include $(DEPS) $(SANITIZED_DEPS) $(TEST_DEPS)

ifeq (Windows_NT, $(OS))
RM_CMD := rmdir /S /Q
ZIP_CMD := zip.exe
else
RM_CMD := rm -rf
ZIP_CMD := zip
endif

TEST_DIR_PREFIX := pa3_test

clean-test:
	-$(RM_CMD) $(TEST_DIR_PREFIX)_*

clean: clean-test
	-$(RM_CMD) $(BUILD_DIR)

zinc: $(SUBMISSION)
	$(ZIP_CMD) pa3.zip $^

.PHONY: all clean sanitized test test-unsanitized
