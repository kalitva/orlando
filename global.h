#include <stdbool.h>

#include "dtypes/types.h"


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
  PAGE_DOWN
};

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

struct s_config {
  int tab_size;
};


struct s_state g_state;
struct s_config g_config;
t_list *g_lines;