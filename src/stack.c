#include <malloc.h>

#include "../include/stack.h"

static const char *err_msgs_arr[] = {
	"No mistakes.\n",
    "ERROR: General error.\n",
    "ERROR: Double call StackCtor.\n",
	"ERROR: Null pointer to stack data.\n",
	"ERROR: Wrong stack size.\n",
	"ERROR: Wrong stack capacity.\n",
    "ERROR: Malloc didn't allocate memory.\n",
    "ERROR: Calling StackPop on an empty stack.\n",
	"ERROR: Two hash does not match true value.\n",
    "ERROR: Hash data with canary is not correct.\n",
    "ERROR: Hash data without canary is not correct.\n",
	"ERROR: Stack left_canary does not match true value.\n",
	"ERROR: Stack right_canary does not match true value.\n",
	"ERROR: Data left_canary does not match true value.\n",
	"ERROR: Data right_canary does not match true value.\n",
	"ERROR: Null pointer to stack.\n",
    "ERROR: Null pointer to array errors.\n"
};

static uint_t ERROR_STORAGE = 0;

int StackCtor(Stack *stk, size_t capacity)
{
    if (stk == NULL)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_PTR]);
        return ERR_STK_PTR;
    }
    else if (capacity <= 0)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_СPT]);
        return ERR_STK_СPT;
    }
    else if (stk->life)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_TWO_CTOR]);
        return ERR_TWO_CTOR;
    }
    
    stk->life     = true;
	stk->capacity = capacity;
	stk->size     = 0;

#ifdef DEBUG
	stk->stk_lcanary = CANARY_STD_VAL;
	stk->stk_rcanary = CANARY_STD_VAL;
    
	size_t tmp         = (capacity * sizeof(elem_t)) / sizeof(canary_t);
	size_t size_calloc = (3 + tmp) * sizeof(canary_t);

	stk->data = (elem_t *)((char *)calloc(size_calloc, sizeof(char)) + sizeof(canary_t));
    if (stk->data == NULL)
    {
        fprintf(stderr, "%s\n", err_msgs_arr[ERR_MALLOC]);
        return ERR_MALLOC;
    }

	*((canary_t *)stk->data - 1)  = CANARY_STD_VAL;
	*SearchPtrDataRCanary(stk) = CANARY_STD_VAL;
#else
	stk->data = (elem_t *)calloc(capacity, sizeof(elem_t));
#endif 
    
    if (stk->data == NULL)
    {
        fprintf(stderr, "%s\n", err_msgs_arr[ERR_MALLOC]);
        return ERR_MALLOC;
    }

    for (size_t i = 0; i < capacity; i++)
        stk->data[i] = VENOM_ELEM;

    COUNT_HASH(stk);

    return ASSERT_STACK(stk);
}

int StackDtor(Stack *stk)
{
    if (stk == NULL)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_PTR]);
        return ERR_STK_PTR;
    }
    if (ASSERT_STACK(stk))
        return ERR_NOT_NAME;

#ifdef DEBUG
	free((char *)stk->data - sizeof(canary_t));

	stk->stk_lcanary = 0;
	stk->stk_rcanary = 0;
#else
	free(stk->data);
#endif

	stk->data     = NULL;
	stk->size     = VENOM_SIZE;
	stk->capacity = VENOM_SIZE;  
	stk->life     = false;
	
    return 0;
}

int StackPush(Stack *stk, elem_t elem)
{
    if (stk == NULL)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_PTR]);
        return ERR_STK_PTR;
    }
    if (ASSERT_STACK(stk))
        return ERR_NOT_NAME;
	
    if (stk->size == stk->capacity)
		StackReallocIncrease(stk);

	stk->data[stk->size] = elem;
    stk->size++;

	PRINT_INFO("StackPush:: <" ELEM_MOD ">\n", elem);

	COUNT_HASH(stk);

	return ASSERT_STACK(stk);
}

int StackPop(Stack *stk, elem_t *num)
{
    if (stk == NULL)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_PTR]);
        return ERR_STK_PTR;
    }
	if (ASSERT_STACK(stk))
        return ERR_NOT_NAME;
    
    if (stk->size <= 0)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_EMPTY]);
        return ERR_STK_EMPTY;
    }

	PRINT_INFO("StackPop::  <" ELEM_MOD ">\n", stk->data[stk->size - 1]);
	
	*num = stk->data[stk->size - 1];

	stk->data[stk->size - 1] = VENOM_ELEM;
	stk->size--;

    COUNT_HASH(stk);
	
    if ((stk->size < (stk->capacity / RLC_COEF_CHECK_DEC)) && (stk->size > 5))
		StackReallocDecrease(stk);

    return ASSERT_STACK(stk);
}

int StackReallocIncrease(Stack *stk)
{
    if (stk == NULL)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_PTR]);
        return ERR_STK_PTR;
    }
	if (ASSERT_STACK(stk))
        return ERR_NOT_NAME;

	stk->capacity = stk->capacity * RLC_COEF_INC;

#ifdef DEBUG
	size_t tmp         = ( stk->capacity * sizeof(elem_t) ) / sizeof(canary_t);
	size_t size_calloc = (3 + tmp) * sizeof(elem_t);

	stk->data = (elem_t *)( (char *)realloc((char *)stk->data - sizeof(canary_t), size_calloc) + sizeof(canary_t) );

	*SearchPtrDataRCanary(stk) = CANARY_STD_VAL;
#else
	stk->data = (elem_t *)realloc(stk->data, stk->capacity * sizeof(elem_t));
#endif
    
    COUNT_HASH(stk);
    
    return ASSERT_STACK(stk);
}

int StackReallocDecrease(Stack *stk)
{
    if (stk == NULL)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_PTR]);
        return ERR_STK_PTR;
    }
	if (ASSERT_STACK(stk))
        return ERR_NOT_NAME;
	
    stk->capacity /= RLC_COEF_DEC;

#ifdef DEBUG
	size_t tmp         = ( stk->capacity * sizeof(elem_t) ) / sizeof(canary_t);
	size_t size_calloc = (3 + tmp) * sizeof(canary_t);

	stk->data = (elem_t *)( (char *)realloc((char *)stk->data - sizeof(canary_t), size_calloc) + sizeof(canary_t) );

	*SearchPtrDataRCanary(stk) = CANARY_STD_VAL;
#else
	stk->data = (elem_t  *)realloc(stk->data, stk->capacity * sizeof(elem_t));
#endif
    
    COUNT_HASH(stk);
    
    return ASSERT_STACK(stk);
}

#ifdef DEBUG

int HashControl(Stack *stk)
{
    if (stk == NULL)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_PTR]);
        return ERR_STK_PTR;
    }

    uchar_t *cdatac_ptr = (uchar_t *)stk->data - sizeof(canary_t);
    uchar_t *data_ptr   = (uchar_t *)stk->data;

    size_t sz_cdatac    = stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t);
    size_t sz_data      = stk->capacity * sizeof(elem_t);
    
    hash_t hash_cdatac  = HASH_FUNC(cdatac_ptr, sz_cdatac);
    hash_t hash_data    = HASH_FUNC(data_ptr,   sz_data);

    if ((hash_cdatac != stk->hash_cdatac) && (hash_data != stk->hash_data))
        return ERR_TWO_HASH;
    else if (hash_cdatac != stk->hash_cdatac)
        return ERR_HASH_CDATAC;
    else if (hash_data != stk->hash_data)
        return ERR_HASH_DATA;

    return 0;
}

int CountHash(Stack *stk)
{
    if (stk == NULL)
    {
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_PTR]);
        return ERR_STK_PTR;
    }

    uchar_t *cdatac_ptr = (uchar_t *)stk->data - sizeof(canary_t);
    uchar_t *data_ptr   = (uchar_t *)stk->data;

    size_t sz_cdatac    = stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t);
    size_t sz_data      = stk->capacity * sizeof(elem_t);
    
    stk->hash_cdatac    = HASH_FUNC(cdatac_ptr, sz_cdatac);
    stk->hash_data      = HASH_FUNC(data_ptr,   sz_data);

    return 0;
}

#endif /* ifdef DEBUG */

int AssertStack(Stack *stk, const char *file_err, const char *func_err, const int line_err)
{
	ERROR_STORAGE = StackVerification(stk);
	if (ERROR_STORAGE != 0)
	{
        StackDump(stk, file_err, func_err, line_err);
		return ERR_NOT_NAME;
	}

    return 0;
}

uint_t StackVerification(Stack *stk)
{
	if (stk == NULL)
	{
		ERROR_STORAGE |= MSK_STK_PTR;
		return ERROR_STORAGE;
	}

#ifdef DEBUG
    if (stk->stk_lcanary != CANARY_STD_VAL)
		ERROR_STORAGE |= MSK_STK_LCANARY;
    
    if (stk->stk_rcanary != CANARY_STD_VAL)
        ERROR_STORAGE |= MSK_STK_RCANARY;
#endif

	if (stk->data == NULL)
		ERROR_STORAGE |= MSK_STK_DATA;

#ifdef DEBUG
	else
	{
        switch (HASH_CONTROL(stk))
        {
            case ERR_TWO_HASH:
                ERROR_STORAGE |= MSK_TWO_HASH;
                break;
            case ERR_HASH_CDATAC:
                ERROR_STORAGE |= MSK_HASH_CDATAC;
                break;
            case ERR_HASH_DATA:
                ERROR_STORAGE |= MSK_HASH_DATA;
                break;
            default:
                break;
        }

        if (*((canary_t *)stk->data - 1) != CANARY_STD_VAL)
            ERROR_STORAGE |= MSK_DATA_LCANARY;
        
        if (*SearchPtrDataRCanary(stk) != CANARY_STD_VAL)
            ERROR_STORAGE |= MSK_DATA_RCANARY;
	}
#endif

	return ERROR_STORAGE;
}

void StackDump(Stack *stk, const char *file_err, const char *func_err, const int line_err)
{
    fprintf(stderr, "|----------------------------------------------------------------------------|\n");
    fprintf(stderr, "|                                                                            |\n");
    fprintf(stderr, "|                             STACK DUMP                                     |\n");
    fprintf(stderr, "|                                                                            |\n");
    fprintf(stderr, "|----------------------------------------------------------------------------|\n");
    fprintf(stderr, "|\t Called from file: %s; line: %d; func: %s\n|\n", file_err, line_err, func_err);

    if ((ERROR_STORAGE & MSK_STK_PTR))
        fprintf(stderr, "%s", err_msgs_arr[ERR_STK_PTR]);
    else
	{
		fprintf(stderr, "|\t Stack pointer [%p]\n", stk);
        fprintf(stderr, "|\t Amount error:  %d\n", amount_error(ERROR_STORAGE));
		
        bool health_data = true; 
        if ((ERROR_STORAGE & MSK_STK_DATA) || (ERROR_STORAGE & MSK_STK_SZ))
            health_data = false;

		SortOutputError(stk);

		if (health_data)
		{
			for(size_t i = 0; i < stk->size; i++)
				fprintf(stderr, "|\t\t[" ELEM_MOD "]\n", stk->data[i]);
		}
	}
    fprintf(stderr, "|----------------------------------------------------------------------------|\n");
}

int amount_error(uint_t num)
{
    if (num != 0)
    {
        int count = 0;
        while (num >> 1)
        {
            num >>= 1;
            count++;
        }
        return count;
    }
    else
        return 0;
}

void SortOutputError(Stack *stk)
{
    if (ERROR_STORAGE & MSK_STK_SZ)
    {
        fprintf(stderr, "|\t %s", err_msgs_arr[ERR_STK_SZ]);
        fprintf(stderr, "|\t Stack size = %lu\n", stk->size);
    }

    if (ERROR_STORAGE & MSK_STK_СPT)
    {
        fprintf(stderr, "|\t %s", err_msgs_arr[ERR_STK_СPT]);
        fprintf(stderr, "|\t Stack capacity = %lu\n", stk->capacity);
    }

    if (ERROR_STORAGE & MSK_STK_DATA)
        fprintf(stderr, "|\t %s", err_msgs_arr[ERR_STK_DATA]);

#ifdef DEBUG
    if (ERROR_STORAGE & MSK_TWO_HASH)
        fprintf(stderr, "|\t %s", err_msgs_arr[ERR_TWO_HASH]);

    if (ERROR_STORAGE & MSK_STK_LCANARY)
    {
        fprintf(stderr, "|\t %s", err_msgs_arr[ERR_STK_LCANARY]);
        fprintf(stderr, "|\t Stack canary_left = %llu\n", stk->stk_lcanary);
    }

    if (ERROR_STORAGE & MSK_STK_RCANARY)
    {
        fprintf(stderr, "|\t %s", err_msgs_arr[ERR_STK_RCANARY]);
        fprintf(stderr, "|\t Stack canary_right = %llu\n", stk->stk_rcanary);
    }

    if (ERROR_STORAGE & MSK_DATA_LCANARY)
    {
        fprintf(stderr, "|\t %s", err_msgs_arr[ERR_DATA_LCANARY]);
        fprintf(stderr, "|\t Stack data canary_left = %llu\n", *((canary_t *)stk->data - 1));
    }

    if (ERROR_STORAGE & MSK_DATA_RCANARY)
    {
        fprintf(stderr, "|\t %s", err_msgs_arr[ERR_DATA_RCANARY]);
        fprintf(stderr, "|\t Stack data canary_right = %llu\n", *SearchPtrDataRCanary(stk));
    }
#endif
}

#ifdef DEBUG
canary_t *SearchPtrDataRCanary(Stack *stk)
{
	canary_t *pointer_canary = (canary_t *)( (char *)stk->data + 
			( ( stk->capacity * sizeof(elem_t) ) / sizeof(canary_t) ) * sizeof(canary_t) + sizeof(canary_t) );  
	
	return pointer_canary;
}
#endif
