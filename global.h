#include "dtypes/types.h"

#define MAIN_MESSAGE " quit: Ctrl-Q | info: Ctrl-I"


enum e_keys {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN,
  CTRL_HOME_KEY,
  CTRL_END_KEY
};

struct s_state {
  int cursor_X;
  int cursor_Y;
  int screen_rows;
  int screen_cols;
  node_t *top_line;
  int top_line_number;
  bool is_new_file;
  bool dirty;
  char *file_name;
  char *status_msg;
};

struct s_config {
  int tab_size;
};


struct s_state g_state;
struct s_config g_config;
list_t *g_lines;