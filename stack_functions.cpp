#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <inttypes.h>
#include "stack.h"


enum error StackCtor(struct Stack* stk, const char* name, int nline, const char* filename, const char* func) {

    if (!stk) {

        printf("Нулевой указатель на стек\n");

        return STACK_EMPTY;
    }

    const unsigned int START_CAPACITY = 11;

    stk->left_canary = CANARY_VALUE;

    stk->right_canary = CANARY_VALUE;

    stk->sizze = 0;

    stk->capacity = START_CAPACITY;

    stk->name = name;

    stk->nline = nline;

    stk->filename = filename;

    stk->func = func;

    stk->hash_structure = 0;
    stk->hash_data = 0;

    stk->data = (Elem_t*) ((char*) calloc(size_stack(START_CAPACITY) + sizeof(stk->left_canary) * 2,
        sizeof(char)) + sizeof(stk->left_canary));

    add_poison(stk->data, stk->sizze, stk->capacity);

    if (!stk->data) {

        printf("Недостаточно памяти. Купите больше RAM\n");
        return NO_MEM;
    }

    *(((canary_t*) stk->data) - 1) = CANARY_VALUE;
    *((canary_t*) (((char*) stk->data) + size_stack(START_CAPACITY))) = CANARY_VALUE;

    stk->hash_structure = hash_function(stk, sizeof(struct Stack));
    stk->hash_data = hash_function(stk->data, size_stack(stk->capacity));

    return OK;
}


enum error StackDtor(struct Stack* stk) {

    if (!stk) {

        printf("Нулевой указатель на стек\n");

        return STACK_EMPTY;
    }

    stk->sizze = 0;
    stk->capacity = 0;

    if (stk->data) {

        stk->data = (Elem_t*) ((canary_t*) stk->data - 1);
        free(stk->data);
        stk->data = NULL;
    }

    return OK;
}


size_t StackVerify(struct Stack* stk) {

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

    if (stk->data && (*(((canary_t*) stk->data) - 1) != CANARY_VALUE ||
     *((canary_t*) (((char*) stk->data) + size_stack(stk->capacity))) != CANARY_VALUE)) {

        all_error |= (size_t) pow(2, ERROR_CANARY);
    }

    size_t hash_structure = stk->hash_structure;
    size_t hash_data = stk->hash_data;

    stk->hash_structure = 0;
    stk->hash_data = 0;
    stk->hash_structure = hash_function(stk, sizeof(struct Stack));
    stk->hash_data = hash_function(stk->data, size_stack(stk->capacity));

    if (hash_data != stk->hash_data) {

        all_error |= (size_t) pow(2, ERROR_HASH_DATA);
    }

    if (hash_structure != stk->hash_structure) {

        all_error |= (size_t) pow(2, ERROR_HASH_STRUCTURE);
    }

    return all_error;
}


void StackDump(const struct Stack* stk, const char* filename, const unsigned int nstring, const char* func) {

    printf("Values:\n");
    printf("stack[%p], %s creat in %s %s(%d) error from %s %s(%d)\n", &stk, stk->name,
        stk->filename, stk->func, stk->nline, filename, func, nstring);
    if (stk) {

        printf("size = %d\n", stk->sizze);
        printf("capacity = %d\n", stk->capacity);
        printf("left_canary = %I64X\n", stk->left_canary);
        printf("right_canary = %I64X\n", stk->right_canary);
        printf("hash_structure = %d\n", stk->hash_structure);
        printf("hash_data = %d\n", stk->hash_data);
        printf("data[%p]\n", (stk->data) ? (canary_t*) stk->data - 1 : NULL);

        printf("\n\tleft_canary in data = %I64X\n", *(((canary_t*) stk->data) - 1));
        printf("\tright_canary in data = %I64X\n\n", *((canary_t*)
            (((char*) stk->data) + size_stack(stk->capacity))));

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

                    printf("Сработала канарейка, кто-то пытается изменить данные\n");
                    break;

                case ERROR_HASH_DATA:

                    printf("Изменился hash стека, кто-то меняет то, что менять нельзя\n");
                    break;

                case ERROR_HASH_STRUCTURE:

                    printf("Изменился hash структуры, кто-то меняет то, что менять нельзя\n");
                    break;

                case NO_MEM:

                    break;

                case ERROR_VARIABLE:

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
        return STACK_EMPTY;
    }

    const size_t COEF_EXPANSION = 2;

    if (stk->sizze == stk->capacity) {

        *((canary_t*) (stk->data + stk->capacity)) = POISON;
        stk->capacity *= COEF_EXPANSION;

        stk->data = (Elem_t*) ((char*) realloc((char*) stk->data - sizeof(stk->left_canary),
            size_stack(stk->capacity) + sizeof(stk->left_canary) * 2) + sizeof(stk->left_canary));

        add_poison(stk->data, stk->sizze, stk->capacity);

        *((canary_t*) (((char*) stk->data) + size_stack(stk->capacity))) = CANARY_VALUE;

        if (!stk->data) {

            printf("Недостаточно памяти. Купите больше RAM\n");
            return NO_MEM;
        }
    }

    stk->data[stk->sizze++] = value;

    stk->hash_structure = 0;
    stk->hash_data = 0;
    stk->hash_structure = hash_function(stk, sizeof(struct Stack));
    stk->hash_data = hash_function(stk->data, size_stack(stk->capacity));

    return OK;
}


enum error StackPop(struct Stack* stk, Elem_t* variable) {

    if (!variable) {

        printf("Нулевой указатель на переменную, в которую записывается значение, взятое из стека\n");

        return ERROR_VARIABLE;
    }

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
            size_stack(stk->capacity) + sizeof(stk->left_canary) * 2) + sizeof(stk->left_canary));

        add_poison(stk->data, stk->sizze, stk->capacity);

        *((canary_t*) (((char*) stk->data) + size_stack(stk->capacity))) = CANARY_VALUE;

        if (!stk->data) {

            printf("Недостаточно памяти. Купите больше RAM\n");
            return NO_MEM;
        }
    }

    *variable = stk->data[stk->sizze - 1];
    stk->data[stk->sizze--] = POISON;

    stk->hash_structure = 0;
    stk->hash_data = 0;
    stk->hash_structure = hash_function(stk, sizeof(struct Stack));
    stk->hash_data = hash_function(stk->data, size_stack(stk->capacity));

    return OK;
}


enum error print_data(const Elem_t* data, const size_t sizze) {

    if (!data) {

        printf("Нулевой указатель на стек\n");

        return DATA_NULL;
    }

    for (size_t number = 0; number < sizze; number++) {

        printf("%d ", data[number]);
    }

    putchar('\n');

    return OK;
}


enum error add_poison(Elem_t* data, const size_t sizze, const size_t capacity) {

    if (!data) {

        printf("Нулевой указатель на стек\n");

        return DATA_NULL;
    }

    for (size_t i = sizze; i < capacity; i++) {

        data[i] = POISON;
    }

    return OK;
}


//gnu hash from https://blogs.oracle.com/solaris/post/gnu-hash-elf-sections
size_t hash_function(const void* data, const size_t capacity) {

    if (!data) {

        printf("Нулевой указатель на стек\n");

        return 0;
    }

    size_t h = 5381;

    for(size_t i = 0; i < capacity; i++) {

        h = 33 * h + ((const char*) data)[i];
    }

    return h;
}


size_t size_stack(const size_t capacity) {

    return capacity * sizeof(Elem_t) +
    ((sizeof(canary_t) - capacity * sizeof(Elem_t) % sizeof(canary_t)) % sizeof(canary_t));
}
