#include "stack.h"
#include <TXLib.h>
#include <stdio.h>


int main() {

    struct Stack stk = {};

    STACK_CTOR(&stk);

    STACK_CTOR(&stk);

    for (int i = 0; i < 15; i++) {

        StackPush(&stk, i);
    }

    print_data(stk.data, stk.sizze);

    Elem_t number = 0;

    char* address = ((char*) stk.data) - sizeof(stk.left_canary);

    for (size_t i = 10; i < 20; i++) {

        address[i] = '1';
    }

    StackPop(&stk, &number);
    printf("%d\n", number);

    stk.sizze = 7;

    stk.hash_structure = 0;
    stk.hash_data = 0;
    stk.hash_structure = hash_function(&stk, sizeof(struct Stack));

    StackPop(&stk, &number);

    printf("%d\n", number);

    print_data(stk.data, stk.sizze);

    address = ((char*) stk.data) + size_stack(stk.capacity);

    for (size_t i = 1; i < 8; i++) {

        address[i] = '1';
    }

    print_data(stk.data, stk.sizze);

    StackPop(&stk, &number);

    printf("%d\n", number);

    StackDtor(&stk);
}
