#ifndef STACK_HEADER
#define STACK_HEADER

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "mydef.h"

typedef double         elem_t;
typedef ullint_t       canary_t;
typedef uint_least32_t hash_t;
#define ELEM_MOD "%.2lf"

static const elem_t VENOM_ELEM = -13;
static const size_t VENOM_SIZE = 0;

static const size_t RLC_COEF_DEC       = 2;
static const size_t RLC_COEF_INC       = 2;
static const size_t RLC_COEF_CHECK_DEC = 4;

static const canary_t CANARY_STD_VAL = 0xab0baab0baab0ba;

#ifdef DEBUG
typedef struct {
    canary_t stk_lcanary;    
    bool     life;
    size_t   size;
    elem_t   *data;
    size_t   capacity;
    hash_t   hash_cdatac;
    hash_t   hash_data;
    canary_t stk_rcanary;
} Stack;
#else
typedef struct Stack {
    bool   life;	
    size_t size;
	elem_t *data;
	size_t capacity;
} Stack;
#endif // DEBUG


// Error part
static const int ERROR_CNT = 14;
enum Code_error
{
	ERR_NOT_NAME     = 1,
    ERR_TWO_CTOR     = 2,
	ERR_STK_DATA     = 3,
	ERR_STK_SZ       = 4,
	ERR_STK_СPT      = 5,
    ERR_MALLOC       = 6,
    ERR_STK_EMPTY    = 7,
	ERR_TWO_HASH     = 8,
    ERR_HASH_CDATAC  = 9,
    ERR_HASH_DATA    = 10,
	ERR_STK_LCANARY  = 11,
	ERR_STK_RCANARY  = 12,
	ERR_DATA_LCANARY = 13,
	ERR_DATA_RCANARY = 14,
    ERR_STK_PTR      = 15,
    ERR_ARR_ERR      = 16
};
enum MaskError {
	MSK_NOT_NAME     = 0x1,         // 0000 0000 0000 0000 0000 0001
    MSK_TWO_CTOR     = 0x2,         // 0000 0000 0000 0000 0000 0010
	MSK_STK_DATA     = 0x4,         // 0000 0000 0000 0000 0000 0100
	MSK_STK_SZ       = 0x8,         // 0000 0000 0000 0000 0000 1000
	MSK_STK_СPT      = 0x10,        // 0000 0000 0000 0000 0001 0000
    MSK_MALLOC       = 0x20,        // 0000 0000 0000 0000 0010 0000
    MSK_STK_EMPTY    = 0x40,        // 0000 0000 0000 0000 0100 0000
	MSK_TWO_HASH     = 0x80,        // 0000 0000 0000 0000 1000 0000
    MSK_HASH_CDATAC  = 0x100,       // 0000 0000 0000 0001 0000 0000
    MSK_HASH_DATA    = 0x200,       // 0000 0000 0000 0010 0000 0000
	MSK_STK_LCANARY  = 0x400,       // 0000 0000 0000 0100 0000 0000
	MSK_STK_RCANARY  = 0x800,       // 0000 0000 0000 1000 0000 0000
	MSK_DATA_LCANARY = 0x1000,      // 0000 0000 0001 0000 0000 0000
	MSK_DATA_RCANARY = 0x2000,      // 0000 0000 0010 0000 0000 0000
    MSK_STK_PTR      = 0x4000,      // 0000 0000 0100 0000 0000 0000
    MSK_ARR_ERR      = 0x8000       // 0000 0000 1000 0000 0000 0000
};

// Functions list
int StackCtor(Stack *stk, size_t capacity);
int StackDtor(Stack *stk);

int StackPush(Stack *stk, elem_t elem);
int StackPop(Stack *stk, elem_t *num);

int StackReallocIncrease(Stack *stk);
int StackReallocDecrease(Stack *stk);

int    AssertStack      (Stack *stk, const char *file_err, const char *func_err, const int line_err);
uint_t StackVerification(Stack *stk);
void   StackDump        (Stack *stk, const char *file_err, const char *func_err, const int line_err);
void   SortOutputError  (Stack *stk);
int    amount_error     (uint_t num);


#ifdef DEBUG
canary_t *SearchPtrDataRCanary(Stack *stk);
int      HashControl(Stack *stk);
hash_t   Crc32 (uchar_t *buf, size_t len);
int      CountHash(Stack *stk);
#endif


// Define list
#define ASSERT_STACK(stk_ptr) AssertStack(stk_ptr, __FILE__, __func__, __LINE__)

#ifdef DEBUG
    #define HASH_CONTROL(stk_ptr)    HashControl(stk_ptr)
    #define HASH_FUNC(buf_ptr, size) Crc32(buf_ptr, size)
    #define COUNT_HASH(stk_ptr)      CountHash(stk_ptr)
#else
    #define HASH_CONTROL(stk_ptr)    {}
    #define HASH_FUNC(buf_ptr, size) {}
    #define COUNT_HASH(stk_ptr)      {}
#endif

#endif /*  STACK_HEADER  */
