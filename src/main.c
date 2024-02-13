#include "../include/stack.h"
#include <stdio.h>

int main()
{
	Stack stk = {};
	StackCtor(&stk, 2);

	for (int i = 0; i < 50; i++)
    {
        // if (i == 34)
            // stk.data[23] = 139;

	    if (StackPush(&stk, i))
        {
            printf("Пуш проверку прошел\n");
            return 1;
        } 
    }

	elem_t p = 0;
	for (int i = 0; i < 50; i++)
	{
        // if (i == 32)
        //     stk.data[0] = 333;
		
        if (StackPop(&stk, &p))
        {
            printf("Работает к стэку не подобраться(POP)\n");
            return 1;
        }
	}

	StackDtor(&stk);
}
