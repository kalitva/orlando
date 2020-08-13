#include <stdbool.h>


typedef struct {
  int capacity;
  int len;
  char *str;
} line_t;

typedef struct {
	void *value;
	void *next;
	void *previous;
} node_t;

typedef struct {
  int value;
  void *previous;
} item_t;

typedef struct {
  item_t *top;
} stack_t;

typedef struct  {
	node_t *head;
	node_t *first;
	node_t *last;
	int size;
} list_t;