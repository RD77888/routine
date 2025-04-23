# Makefile for compiling C++ multithreading programs

# 编译器和编译选项
CXX = g++               # 指定使用g++编译器
CXXFLAGS = -std=c++11 -Wall -Wextra  # 编译选项：C++11标准、显示所有警告
LDFLAGS = -pthread      # 链接选项：链接pthread库，必须用于多线程程序

# 所有可执行目标 
TARGETS = simpleconditionvar conditionwait notify notify_all

# 默认目标 - 编译所有程序
all: $(TARGETS)

# 每个cpp文件编译成对应的可执行文件
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# 清理生成的文件
clean:
	rm -f $(TARGETS)

# 调试版本
debug: CXXFLAGS += -g -DDEBUG
debug: all

# 优化版本
release: CXXFLAGS += -O2 -DNDEBUG
release: all

# 声明伪目标
.PHONY: all clean debug release