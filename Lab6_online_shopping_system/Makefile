SRCS = main.cpp Customer.cpp CustomerData.cpp Platform.cpp Product.cpp Rating.cpp Shop.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# Uncomment the following line to enable sanitizers
# CXXFLAGS += -fsanitize=address,leak,undefined

ifeq ($(OS),Windows_NT)
	EXEC = shopping_system.exe
	RM = del
else
	EXEC = shopping_system
	RM = rm -f
endif

all: $(EXEC)
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

-include $(DEPS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -MMD -MP -c $<

clean:
	$(RM) $(EXEC) $(OBJS) $(DEPS)
