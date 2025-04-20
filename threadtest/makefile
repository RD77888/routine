# Makefile for compiling C++ multithreading programs

# 编译器和编译选项
CXX = g++               # 指定使用g++编译器
CXXFLAGS = -std=c++11 -Wall -Wextra  # 编译选项：C++11标准、显示所有警告
LDFLAGS = -pthread      # 链接选项：链接pthread库，必须用于多线程程序

# 目标和依赖关系
TARGET = simpleconditionvar
SRCS = simpleconditionvar.cpp
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)                        # 默认目标依赖于最终可执行文件

# 编译规则
$(TARGET): $(OBJS)                    # 可执行文件依赖于.o文件
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)  
    # 链接.o文件生成可执行文件

# 生成目标文件的规则
%.o: %.cpp                            # 模式规则：每个.o文件依赖于对应的.cpp文件
	$(CXX) $(CXXFLAGS) -c $< -o $@    
    # 编译.cpp生成.o文件

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