CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

TARGET = indice
SRCS = ./src/main.cpp \
       ./src/services/CLI.cpp \
       ./src/services/Indexer.cpp \
       ./src/services/Serializer.cpp \
       ./src/services/QueryProcessor.cpp \
       ./src/services/TextProcessor.cpp \
       ./src/services/Index.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
