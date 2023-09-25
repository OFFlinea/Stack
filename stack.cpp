#include "stack.h"
#include <TXLib.h>
#include <stdio.h>


int main() {

    struct Stack stk = {};

    STACK_CTOR(&stk);

    for (int i = 0; i < 15; i++) {

        StackPush(&stk, i);
    }

    print_data(stk.data, stk.sizze);

    Elem_t number = 0;

    stk.data = NULL;

    StackPop(&stk, &number);

    printf("%d\n", number);

    StackDtor(&stk);
}
