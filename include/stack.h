#ifndef STACK_HEADER
#define STACK_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG_CHECK
const int ERROR_CNT = 13;
#else
const int ERROR_CNT = 6;
#endif

const double VENOM_ELEM = -13;
const size_t VENOM_SIZE = 0;

const size_t REALLOC_FACTOR_DECREASE = 2;
const size_t REALLOC_DECREASE_CHECK = 4;

#define ELEM_MOD "%.2lf"
typedef double ELEM_T;

#ifdef DEBUG_CHECK
const unsigned long long CANARY = 0xab0baab0baab0ba;
const unsigned long long CANARY_STR = 0xdeaddeaddeaddead;
typedef unsigned long long CANARY_T;
typedef unsigned long long HASH_TYPE;
#endif

enum code_error
{
	STACK_OK,
	STACK_ERR,
	STACK_DATA_ERR,
	STACK_SIZE_ERR,
	STACK_CAPACITY_ERR,
#ifdef DEBUG_CHECK
	STACK_HASH_ERR,
	STACK_CANARY_ERR,
	STACK_LEFT_CANARY_ERR,
	STACK_RIGHT_CANARY_ERR,
	STACK_DATA_CANARY_ERR,
	STACK_DATA_canary_str_l_ERR,
	STACK_DATA_canary_str_r_ERR,
#endif
};

struct Stack
{
#ifdef DEBUG_CHECK
	CANARY_T canary_str_l;
#endif
	size_t size;
	ELEM_T *data;
	size_t capacity;
#ifdef DEBUG_CHECK
	const CANARY_T STACK_VALUE_DEFAULT = 0;
	HASH_TYPE hash = STACK_VALUE_DEFAULT;
	CANARY_T canary_str_r;
#endif
};

int StackCtor(Stack *stk, size_t capacity);
int StackDtor(Stack *stk);

int StackPush(Stack *stk, ELEM_T elem);
int StackPop(Stack *stk, ELEM_T *num);

int StackReallocIncrease(Stack *stk);
int StackReallocDecrease(Stack *stk);

int *stack_verification(Stack *stk, const char* name_func);
void stack_dump(Stack *stk, int *code_error, const char *file_err, const char *func_err, const int line_err);
void sort_output_error(Stack *stk, int error);

ELEM_T *my_memset(ELEM_T *destination, ELEM_T elem, size_t size);

#ifdef DEBUG_CHECK
CANARY_T *search_data_right_canary(Stack *stk);

HASH_TYPE hash_control (Stack *stk);

char *convert_bin_msg (Stack *stk, char *msg);

size_t conversion_bin_sys(size_t value, char *text, size_t text_len);

HASH_TYPE hash_crc_32 (char *message);
#endif

#define ASSERT_STACK(stack)                                          \
{                                                                    \
	int *code_err = stack_verification(stack, __func__);             \
	if (code_err[0] != STACK_OK)                                     \
	{                                                                \
		stack_dump(stack, code_err, __FILE__, __func__, __LINE__);   \
		return code_err[0];                                          \
	}                                                                \
	free(code_err);                                                  \
}         

#endif