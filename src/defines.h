#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define TAB 2

#define CTRL_KEY(k) ((k) & 0x1f)

#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLLIGHT_STRINGS (1<<1)
#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#define BUF_INIT { NULL, 0 }

enum editor_key {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

enum editor_highlight {
  HL_NORMAL = 0,
  HL_COMMENT,
  HL_MLCOMMENT,
  HL_KEYWORD1,
  HL_KEYWORD2,
  HL_STRING,
  HL_NUMBER,
  HL_MATCH
};


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

typedef struct  {
	t_node *head;
	t_node *first;
	t_node *last;
	int size;
} t_list;

struct s_state {
  int cursor_X;
  int cursor_Y;
  int screen_rows;
  int screen_cols;
  t_node *top_line;
  int top_line_number;
  char *file_name;
  char status_msg[80];
  bool dirty;
};


struct s_state g_state;
t_list *g_lines;