#include <stdlib.h>

#include "types.h"

stack_t *new_stack()
{
	stack_t *stack = malloc(sizeof(stack_t));

	stack->top = NULL;

	return stack;
}

void push(stack_t *stack, int value)
{
	item_t *item = malloc(sizeof(item_t));
	item->value = value;

	item->previous = stack->top;
	stack->top = item;
}

int pop(stack_t *stack)
{
	if (!stack->top) {
		return 0;
	}

	int value;
	item_t *top;

	value = stack->top->value;
	top = stack->top;

	stack->top = stack->top->previous;

	free(top);

	return value;
}

int peek(stack_t *stack)
{
	return stack->top ? stack->top->value : 0;
}