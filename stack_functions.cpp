#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "stack.h"


enum error StackCtor(struct Stack* stk, const char* name, int nline, const char* filename, const char* func) {

    assert(stk);

    const unsigned int START_CAPACITY = 10;

    stk->left_canary = CANARY_VALUE;

    stk->right_canary = CANARY_VALUE;

    stk->sizze = 0;

    stk->capacity = START_CAPACITY;

    stk->name = name;

    stk->nline = nline;

    stk->filename = filename;

    stk->func = func;

    stk->data = (Elem_t*) ((char*) calloc(START_CAPACITY * sizeof(Elem_t) + sizeof(stk->left_canary) * 2, sizeof(char)) + sizeof(stk->left_canary));

    *(((canary_t*) stk->data) - 1) = CANARY_VALUE;
    *((canary_t*) (stk->data + stk->capacity)) = CANARY_VALUE;

    if (!stk->data) {

        printf("Недостаточно памяти. Купите больше RAM\n");
        return NO_MEM;
    }

    return OK;
}


enum error StackDtor(struct Stack* stk) {

    assert(stk);

    stk->sizze = 0;
    stk->capacity = 0;

    if (stk->data) {

        stk->data = (Elem_t*) ((canary_t*) stk->data - 1);
        free(stk->data);
        stk->data = NULL;
    }

    return OK;
}


size_t StackVerify(const struct Stack* stk) {

    size_t all_error = 0;

    if (!stk) {

        all_error |= (size_t) pow(2, STACK_EMPTY);
    }

    if (!stk->data) {

        all_error |= (size_t) pow(2, DATA_NULL);
    }

    if (stk->sizze > stk->capacity) {

        all_error |= (size_t) pow(2, ERROR_SIZE);
    }

    if (!stk->capacity) {

        all_error |= (size_t) pow(2, ERROR_CAPACITY);
    }

    if (stk->left_canary != CANARY_VALUE || stk->right_canary != CANARY_VALUE){

        all_error |= (size_t) pow(2, ERROR_CANARY);
    }

    if(stk->data && (*(((canary_t*) stk->data) - 1) != CANARY_VALUE ||
     *((canary_t*) (stk->data + stk->capacity)) != CANARY_VALUE)) {

        all_error |= (size_t) pow(2, ERROR_CANARY);
    }

    return all_error;
}


void StackDump(const struct Stack* stk, const char* filename, const unsigned int nstring, const char* func) {

    printf("Values:\n");
    printf("stack[%p], %s creat in %s %s(%d) error from %s %s(%d)\n", &stk, stk->name, stk->filename, stk->func, stk->nline, filename, func, nstring);
    printf("size = %d\n", stk->sizze);
    printf("capacity = %d\n", stk->capacity);
    printf("data[%p]\n", (canary_t*) stk->data - 1);
    if (stk->data) {

        for (size_t elem = 0; elem < stk->capacity; elem++) {

            if (elem < stk->sizze) {

                printf("\t*[%d] = %d\n", elem, stk->data[elem]);
            }

            else {

                printf("\t [%d] = %d (POISON)\n", elem, stk->data[elem]);
            }
        }
    }

    putchar('\n');
}


void print_all_error(size_t all_error) {

    size_t nError = 0;

    putchar('\n');

    while (all_error > 0) {

        if (all_error % 2 == 1) {

            switch((enum error) nError)
            {
                case STACK_EMPTY:

                    printf("Нулевой указатель на объект структуры стек.\n");
                    break;

                case DATA_NULL:

                    printf("Нулевой указатель на стек (массив).\n");
                    break;

                case ERROR_SIZE:

                    printf("Неверное значение индекса, указывающего на последний элемент.\n");
                    break;

                case ERROR_CAPACITY:

                    printf("Неверное значение размера стека.\n");
                    break;

                case ERROR_CANARY:

                    printf("Сработала канарейка, кто-то пытается изменить данные");

                case NO_MEM:

                    break;

                case OK:

                    break;

                default:

                    break;
            }
        }

        nError += 1;
        all_error >>= 1;
    }

    putchar('\n');
}


enum error StackPush(struct Stack* stk, Elem_t value) {

    size_t error = StackVerify(stk);

    if (error) {

        print_all_error(error);
        StackDump(stk, __FILE__, __LINE__, __func__);
    }

    const size_t COEF_EXPANSION = 2;

    if (stk->sizze == stk->capacity) {

        *((canary_t*) (stk->data + stk->capacity)) = POISON;
        stk->capacity *= COEF_EXPANSION;
        stk->data = (Elem_t*) ((char*) realloc((char*) stk->data - sizeof(stk->left_canary),
            stk->capacity * sizeof(Elem_t) + sizeof(stk->left_canary) * 2) + sizeof(stk->left_canary));
        *((canary_t*) (stk->data + stk->capacity)) = CANARY_VALUE;

        if (!stk->data) {

            printf("Недостаточно памяти. Купите больше RAM\n");
            return NO_MEM;
        }
    }

    stk->data[stk->sizze++] = value;

    return OK;
}


enum error StackPop(struct Stack* stk, Elem_t* variable) {

    assert(variable);

    size_t error = StackVerify(stk);

    if (error) {

        print_all_error(error);
        StackDump(stk, __FILE__, __LINE__, __func__);
        return STACK_EMPTY;
    }


    const size_t COEF_REDUCTION = 2;

    if (stk->sizze * COEF_REDUCTION * COEF_REDUCTION == stk->capacity) {

        *((canary_t*) (stk->data + stk->capacity)) = POISON;
        stk->capacity /= COEF_REDUCTION;
        stk->data = (Elem_t*) ((char*) realloc((char*) stk->data - sizeof(stk->left_canary),
            stk->capacity * sizeof(Elem_t) + sizeof(stk->left_canary) * 2) + sizeof(stk->left_canary));
        *((canary_t*) (stk->data + stk->capacity)) = CANARY_VALUE;

        if (!stk->data) {

            printf("Недостаточно памяти. Купите больше RAM\n");
            return NO_MEM;
        }
    }

    *variable = stk->data[stk->sizze - 1];
    stk->data[stk->sizze--] = POISON;

    return OK;
}


void print_data(const Elem_t* data, const size_t sizze) {

    assert(data);

    for (size_t number = 0; number < sizze; number++) {

        printf("%d ", data[number]);
    }

    putchar('\n');
}
