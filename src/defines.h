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
#define KILO_QUIT_TIMES 1

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


struct char_buffer {
  char *str;
  int len;
};

typedef struct {
  int capacity;
  int len;
  char *str;
} Line;

typedef struct {
	void *value;
	void *next;
	void *previous;
} Node;

typedef struct  {
	Node *head;
	Node *first;
	Node *last;
	int size;
} List;

struct Editor_config {
  int cursor_X;
  int cursor_Y;
  int row_offset;
  int col_offset;
  int screen_rows;
  int screen_cols;
  int num_rows;
  char *file_name;
  char status_msg[80];
  bool dirty;
  bool is_new_file;
  struct editor_syntax *syntax;
};