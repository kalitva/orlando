#include <stdbool.h>


typedef struct {
	int value;
	void *previous;
} t_node;

typedef struct {
	t_node *top;
} t_stack;