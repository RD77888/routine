#include<stdio.h>
#include<ucontext.h>
#include<unistd.h>

int main() {
    ucontext_t context;
    getcontext(&context);
    printf("ucontext_t size: %zu\n", sizeof(context));
    printf("ucontext_t uc_mcontext size: %zu\n", sizeof(context.uc_mcontext));
    printf("ucontext_t uc_stack size: %zu\n", sizeof(context.uc_stack));
    sleep(1);
    setcontext(&context);
    printf("ucontext_t uc_link size: %zu\n", sizeof(context.uc_link));
    printf("ucontext_t uc_sigmask size: %zu\n", sizeof(context.uc_sigmask));
    return 0;
}

