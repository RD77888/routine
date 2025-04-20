# Makefile for compiling C++ multithreading programs

# 编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = -pthread

# 目标和依赖关系
TARGET = simpleconditionvar
SRCS = simpleconditionvar.cpp
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 编译规则
$(TARGET): $(OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 生成目标文件的规则
%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
    rm -f $(OBJS) $(TARGET)

# 调试版本
debug: CXXFLAGS += -g -DDEBUG
debug: all

# 优化版本
release: CXXFLAGS += -O2 -DNDEBUG
release: all

# 声明伪目标
.PHONY: all clean debug release