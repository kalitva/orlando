#include <stdlib.h>

#include "types.h"

t_stack *new_stack()
{
	t_stack *stack = malloc(sizeof(t_stack));

	stack->top = NULL;

	return stack;
}

void push(t_stack *stack, int value)
{
	t_item *item = malloc(sizeof(t_item));
	item->value = value;

	item->previous = stack->top;
	stack->top = item;
}

int pop(t_stack *stack)
{
	if (!stack->top) {
		return 0;
	}

	int value;
	t_item *top;

	value = stack->top->value;
	top = stack->top;

	stack->top = stack->top->previous;

	free(top);

	return value;
}

int peek(t_stack *stack)
{
	return stack->top ? stack->top->value : 0;
}