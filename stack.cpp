#include "stack.h"

static const char *err_msgs_arr[] = {
  "NO ERROR.\n",
  "ERROR: null pointer to stack.\n",
  "ERROR: null pointer to stack data.\n",
  "ERROR: wrong stack size.\n",
  "ERROR: wrong stack capacity.\n",
#ifdef DEBUG_CHECK
  "ERROR: hash does not match true value.\n",
  "ERROR: error when checking stack canary.\n",
  "ERROR: error when checking stack left_canary\n",
  "ERROR: error when checking stack right_canary\n",
  "ERROR: error when checking stack data->canary\n",
  "ERROR: error when checking stack data->left_canary\n",
  "ERROR: error when checking stack data->right_canary\n"
#endif
};

int StackCtor(Stack *stk, size_t capacity)
{

  my_assert(capacity > 0, UNREAL_VALUE);
  my_assert(stk != NULL, PATH_NULL);
  
  stk->capacity = capacity;

#ifdef DEBUG_CHECK
  stk->canary_str_l = CANARY_STR;
  stk->canary_str_r = CANARY_STR;

  size_t tmp = ( capacity * sizeof(ELEM_T) ) / sizeof(CANARY_T);
  size_t size_calloc = (3 + tmp) * sizeof(CANARY_T);

  stk->data = (ELEM_T *)((char *)calloc(size_calloc, sizeof(char)) + sizeof(CANARY_T));
  
  *( (CANARY_T *)stk->data - 1 ) = CANARY;

  *search_data_right_canary(stk) = CANARY;
#else
  stk->data = (ELEM_T *)calloc(capacity, sizeof(ELEM_T));
#endif
  
  my_assert(stk->data != NULL, PATH_NULL);
  my_assert(stk->capacity > 0, UNREAL_VALUE);

  stk->size = VENOM_SIZE;

  ELEM_T *data_memset = my_memset(stk->data, VENOM_ELEM, stk->capacity);
  my_assert(data_memset != NULL, PATH_NULL);

#ifdef DEBUG_CHECK
  stk->hash = hash_control(stk);
#endif

  ASSERT_STACK(stk);
  return 0;
}

int StackDtor(Stack *stk)
{
  my_assert(stk != NULL, PATH_NULL);
  ASSERT_STACK(stk);

#ifdef DEBUG_CHECK
  free( (char *)stk->data - sizeof(CANARY_T) );

  stk->canary_str_l = 0;
  stk->canary_str_r = 0;
#else
  free(stk->data);
#endif

  stk->data     = NULL;
  stk->size     = VENOM_SIZE;
  stk->capacity = VENOM_SIZE;  
  stk = NULL;
  return 0;
}

int StackPush(Stack *stk, ELEM_T elem)
{
  my_assert(stk != NULL, PATH_NULL);

  if (stk->size == stk->capacity){
    StackReallocIncrease(stk);
  }

  stk->data[stk->size++] = elem;

  printf("StackPush:: <" ELEM_MOD ">\n", elem);

#ifdef DEBUG_CHECK
  stk->hash = hash_control(stk);
#endif
  ASSERT_STACK(stk);
  return 0;
}

int StackPop(Stack *stk)

{
  my_assert(stk != NULL, PATH_NULL);
  ASSERT_STACK(stk);
  
  printf("StackPop:: <" ELEM_MOD ">\n", stk->data[stk->size - 1]);

  stk->data[stk->size - 1] = VENOM_ELEM;
  stk->size--;

  if (stk->size < stk->capacity / REALLOC_DECREASE_CHECK){
    StackReallocDecrease(stk);
  }

#ifdef DEBUG_CHECK
  stk->hash = hash_control(stk);
#endif

  ASSERT_STACK(stk);
  return 0;
}

int StackReallocIncrease(Stack *stk)
{
  my_assert(stk != NULL, PATH_NULL);
  ASSERT_STACK(stk);
  
  stk->capacity = stk->capacity * 2;
#ifdef DEBUG_CHECK
  size_t tmp = ( stk->capacity * sizeof(ELEM_T) ) / sizeof(CANARY_T);
  size_t size_calloc = (3 + tmp) * sizeof(CANARY_T);

  stk->data = (ELEM_T *)( (char *)realloc((char *)stk->data - sizeof(CANARY_T), size_calloc) + sizeof(CANARY_T) );

  *search_data_right_canary(stk) = CANARY;
#else
  stk->data = (ELEM_T *)realloc(stk->data, stk->capacity * sizeof(ELEM_T));
#endif

#ifdef DEBUG_CHECK
  stk->hash = hash_control(stk);
#endif

  ASSERT_STACK(stk);
  return 0;
}

int StackReallocDecrease(Stack *stk)
{
  my_assert(stk != NULL, PATH_NULL);
  stk->capacity = stk->capacity / REALLOC_FACTOR_DECREASE;

#ifdef DEBUG_CHECK
  size_t tmp = ( stk->capacity * sizeof(ELEM_T) ) / sizeof(CANARY_T);
  size_t size_calloc = (3 + tmp) * sizeof(CANARY_T);

  stk->data = (ELEM_T *)( (char *)realloc((char *)stk->data - sizeof(CANARY_T), size_calloc) + sizeof(CANARY_T) );

  *search_data_right_canary(stk) = CANARY;
#else
  stk->data = (ELEM_T *)realloc(stk->data, stk->capacity * sizeof(ELEM_T));
#endif

#ifdef DEBUG_CHECK
  stk->hash = hash_control(stk);
#endif

  ASSERT_STACK(stk);
  return 0;
}

int *stack_verification(Stack *stk, const char* name_func)
{
  int *storage_err = (int *)calloc(ERROR_CNT + 1, sizeof(int));
  int amount_err = 0;
    
  if (stk == NULL){
    amount_err++;
    storage_err[amount_err] = STACK_ERR;
    storage_err[0] = amount_err;

    return storage_err;
  }

#ifdef DEBUG_CHECK

  if (stk->canary_str_l != CANARY_STR && stk->canary_str_r != CANARY_STR){
    amount_err++;
    storage_err[amount_err] = STACK_CANARY_ERR;
  }
  else{
    if (stk->canary_str_l != CANARY_STR){
      amount_err++;
      storage_err[amount_err] = STACK_LEFT_CANARY_ERR;
    }

    if (stk->canary_str_r != CANARY_STR){
      amount_err++;
      storage_err[amount_err] = STACK_RIGHT_CANARY_ERR;
    }
  }
  
  if (stk->hash != hash_control(stk)){
    amount_err++;
    storage_err[amount_err] = STACK_HASH_ERR;
  }
#endif

  if (stk->data == NULL){
    amount_err++;
    storage_err[amount_err] = STACK_DATA_ERR;
  }

#ifdef DEBUG_CHECK
  else{
    if (*( (CANARY_T *)stk->data - 1 ) != CANARY && *search_data_right_canary(stk) != CANARY){
      amount_err++;
      storage_err[amount_err] = STACK_DATA_CANARY_ERR;
    }

    if (*((CANARY_T *)stk->data - 1) != CANARY){
      amount_err++;
      storage_err[amount_err] = STACK_DATA_canary_str_l_ERR;
    }

    if (*search_data_right_canary(stk) != CANARY){
      amount_err++;
      storage_err[amount_err] = STACK_DATA_canary_str_r_ERR;
    }
  }
#endif

  if (stk->size == 0 && strcmp(name_func, "StackCtor")){
    amount_err++;
    storage_err[amount_err] = STACK_SIZE_ERR;
  }

  storage_err[0] = amount_err;

  return storage_err;
}

void stack_dump(Stack *stk, int *code_error, const char *file_err, const char *func_err, const int line_err)
{
  if (stk != NULL){
    fprintf(stderr, "----------------------------------------------------------------------------\n");
    fprintf(stderr, "stack[%p] \"stk\" called from %s(%d) %s\n", stk, file_err, line_err, func_err);
    fprintf(stderr, "Amount error:  %d\n", code_error[0]);
    fprintf(stderr, "----------------------------------------------------------------------------\n");

    bool health_data = 1; 
    for (int i = 1; i <= code_error[0]; i++){
      sort_output_error(stk, code_error[i]);
      
      if ((code_error[i] == STACK_DATA_ERR) | (code_error[i] == STACK_SIZE_ERR))
        health_data = 0;
    }
    
    if (health_data){
      for(size_t i = 0; i < stk->size; i++)
        fprintf(stderr, "[" ELEM_MOD "]\n", stk->data[i]);
    }

    fprintf(stderr, "----------------------------------------------------------------------------\n");
  }
  
  else
    fprintf(stderr, "Stack[NULL] \"stk\" called from %s(%d) %s\n", file_err, line_err, func_err);

  char live_soft = 'q';
  while((live_soft != 'y') && (live_soft != 'n')){
    fprintf(stderr, "Чтобы завершить программу(все данные будут потеряны) введите \"y\"\n");
    fprintf(stderr, "Чтобы программа продолжила работу(возможно неопределенное поведение) введите \"n\"\n");
    
    scanf("%c", &live_soft);
    if(live_soft == 'y')
      abort();
    if(live_soft == 'n')
      break;
    clear_the_input_buffer();
  }
}

void sort_output_error(Stack *stk, int error)
{
switch (error){
  case STACK_SIZE_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[3]);
    fprintf(stderr, "\t Stack size = %lu\n", stk->size);
    break;

  case STACK_CAPACITY_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[4]);
    fprintf(stderr, "\t Stack capacity = %lu\n", stk->capacity);
    break;

  case STACK_DATA_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[2]);
    break;

#ifdef DEBUG_CHECK
  case STACK_HASH_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[5]);
    break;

  case STACK_CANARY_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[6]);
    fprintf(stderr, "\t Stack canary_left = %llu\n", stk->canary_str_l);
    fprintf(stderr, "\t Stack canary_right = %llu\n", stk->canary_str_r);
    break;

  case STACK_LEFT_CANARY_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[7]);
    fprintf(stderr, "\t Stack canary_left = %llu\n", stk->canary_str_l);
    break;

  case STACK_RIGHT_CANARY_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[8]);
    fprintf(stderr, "\t Stack canary_right = %llu\n", stk->canary_str_r);
    break;

  case STACK_DATA_CANARY_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[9]);
    fprintf(stderr, "\t Stack data canary_left = %llu\n",  *((CANARY_T *)stk->data - 1));
    fprintf(stderr, "\t Stack data canary_right = %llu\n", *search_data_right_canary(stk));
    break;

  case STACK_DATA_canary_str_l_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[10]);
    fprintf(stderr, "\t Stack data canary_left = %llu\n", *((CANARY_T *)stk->data - 1));
    break;  

  case STACK_DATA_canary_str_r_ERR:
    fprintf(stderr, "\t %s", err_msgs_arr[11]);
    fprintf(stderr, "\t Stack data canary_right = %llu\n", *search_data_right_canary(stk));
    break;
#endif

  default:
    break;
}

}
#ifdef DEBUG_CHECK
CANARY_T *search_data_right_canary(Stack *stk)
{
  CANARY_T *pointer_canary = (CANARY_T *)( (char *)stk->data + ( ( stk->capacity * sizeof(ELEM_T) ) / sizeof(CANARY_T) ) * sizeof(CANARY_T) + sizeof(CANARY_T) );  
  return pointer_canary;
}
#endif