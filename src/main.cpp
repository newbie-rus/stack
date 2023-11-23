#include "../include/stack.h"

int main()
{
  Stack stk = {};
  StackCtor(&stk, 2);
  
  for (int i = 0; i < 30; i++)
    StackPush(&stk, i);

  for (int i = 0; i < 25; i++)
    StackPop(&stk);
  
  StackDtor(&stk);
}
