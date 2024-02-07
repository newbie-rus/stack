#include "../include/stack.h"

int main()
{
	Stack stk = {};
	StackCtor(&stk, 2);

	for (int i = 0; i < 30; i++)
	StackPush(&stk, i);

	ELEM_T p = 0;
	for (int i = 0; i < 30; i++)
	{
		StackPop(&stk, &p);
	}
	StackDtor(&stk);
}
