#ifndef STACK_H
#define STACK_H
typedef int Elem_t;
typedef unsigned long long canary_t;

const canary_t CANARY_VALUE = 0xDEB;
const Elem_t POISON = 123;

#define STACK_CTOR(stack) StackCtor((stack), #stack, __LINE__, __FILE__, __func__);

struct Stack
{
    canary_t left_canary;
    Elem_t* data;
    unsigned int sizze;
    unsigned int capacity;
    const char* name;
    int nline;
    const char* filename;
    const char* func;
    canary_t right_canary;
};

enum error
{
    STACK_EMPTY,
    DATA_NULL,
    NO_MEM,
    ERROR_SIZE,
    ERROR_CAPACITY,
    ERROR_CANARY,
    OK = 8
};

enum error StackCtor(struct Stack* stk, const char* name, const int nline, const char* filename, const char* func);

enum error StackDtor(struct Stack* stk);

enum error StackPush(struct Stack* stk, Elem_t value);

enum error StackPop(struct Stack* stk, Elem_t* variable);

void print_data(const Elem_t* data, const unsigned int capacity);

unsigned int StackVerify(const struct Stack* stk);

void StackDump(const struct Stack* stk, const char* filename, const unsigned int nstring, const char* func);

void print_all_error(unsigned int all_error);
#endif
