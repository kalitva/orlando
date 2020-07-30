struct s_buffer {
  char *str;
  int len;
};

typedef struct {
  int capacity;
  int len;
  char *str;
} t_line;

typedef struct {
	void *value;
	void *next;
	void *previous;
} t_node;

typedef struct {
  int value;
  void *previous;
} t_item;

typedef struct {
  t_item *top;
} t_stack;

typedef struct  {
	t_node *head;
	t_node *first;
	t_node *last;
	int size;
} t_list;