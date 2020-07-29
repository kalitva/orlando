#include <stdlib.h>
#include <stdio.h>

#include "dtypes.h"


/*lnklist.c */
t_node *new_node(void *);


t_stack *new_stack()
{
	t_stack *stack = malloc(sizeof(t_stack));

	stack->top = NULL;

	return stack;
}

void push(t_stack *stack, int value)
{
	t_node *node = malloc(sizeof(t_node));
	node->value = value;

	node->previous = stack->top;
	stack->top = node;
}

int pop(t_stack *stack)
{
	if (!stack->top) {
		return 0;
	}

	int value;
	t_node *top;

	value = stack->top->value;
	top = stack->top;

	stack->top = stack->top->previous;

	free(top);

	return value;
}

int main()
{
	t_stack *stack = new_stack();

	for (int i = 0; i < 10; i++) {
		push(stack, i);
	}

	for (int i = 1; i < 15; i++) {
		printf("%d \n", pop(stack));
	}
}