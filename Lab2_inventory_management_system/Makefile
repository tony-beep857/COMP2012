# credit: lab slides about makefile
# Get the shell name
UNAME := $(shell uname)

# Create variables for Windows
COMMA = , $(empty)
SPACE = $(empty) $(empty)

# Variables for compilation
CPPFLAGS = -std=c++17 -Wall -Werror=vla
SRCS = main.cpp slot.cpp inventory.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)
EXE = lab2
ZIP_FILES = slot.cpp inventory.cpp

# Check OS
ifeq ($(UNAME), Linux) # Linux
	CPPFLAGS += -fsanitize=address,leak,undefined
	RM = -rm
	RM_FILES = $(EXE) $(OBJS) $(DEPS)
	ZIPCMD = zip $(EXE).zip $(ZIP_FILES)
else ifeq ($(UNAME), Darwin) # macOS
	CPPFLAGS += -fsanitize=address,undefined
	RM = -rm
	RM_FILES = $(EXE) $(OBJS) $(DEPS)
	ZIPCMD = zip $(EXE).zip $(ZIP_FILES)
else # Windows
	RM = -powershell.exe del -ErrorAction SilentlyContinue
	RM_FILES = $(EXE), $(EXE).exe, $(subst $(SPACE),$(COMMA),$(OBJS) $(DEPS))
	ZIPCMD = powershell.exe Compress-Archive -Force -Path $(subst $(SPACE),$(COMMA),$(ZIP_FILES)) -DestinationPath .\$(EXE).zip
endif

# Rules: 
# target: dependencies
#	command used to create the target
$(EXE): $(OBJS)
	g++ $(CPPFLAGS) -o $@ $^

# To include the .d dependency files
-include $(DEPS)

# -MMD -MP creates the .d dependency files
%.o: %.cpp
	g++ $(CPPFLAGS) -c -MMD -MP $<

# Remove non-source files
clean:
	$(RM) $(RM_FILES)

# Create a ZIP archive for submission
zip:
	$(ZIPCMD)
