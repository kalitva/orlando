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

struct editor_syntax {
  char *file_type;
  char **file_match;
  char **keywords;
  char *single_line_comment_start;
  char *multiline_comment_start;
  char *multiline_comment_end;
  int flags;
};

typedef struct erow {
  int idx;
  int size;
  int rsize;
  char *chars;
  char *render;
  unsigned char *hl;
  int hl_open_comment;
} erow;

struct editor_config {
  int cx;
  int cy;
  int rx;
  int row_offset;
  int col_offset;
  int screen_rows;
  int screen_cols;
  int num_rows;
  erow *row;
  char *file_name;
  char status_msg[80];
  bool dirty;
  bool is_new_file;
  struct editor_syntax *syntax;
};