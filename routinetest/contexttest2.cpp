#include <ucontext.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void func1(char *arg) {
    printf("func1: %s\n", arg);
    sleep(1);
}

void context_test() {
    char stack[1024*1024];
    ucontext_t child, main;
    char msg[] = "Hello from context";
    
    // 初始化上下文
    getcontext(&child);
    getcontext(&main);
    
    child.uc_stack.ss_sp = stack;
    child.uc_stack.ss_size = sizeof(stack); 
    child.uc_stack.ss_flags = 0;
    child.uc_link = &main;  // 设置执行完后返回的上下文
    
    // 正确地创建上下文并传递参数
    makecontext(&child, (void(*)(void))func1, 1, msg);
    
    printf("切换到子上下文\n");
    swapcontext(&main, &child);
    
    // 子上下文执行完后返回这里
    printf("返回到主上下文\n");
}

int main() {
    context_test();
    return 0;
}