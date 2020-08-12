#include "dtypes/types.h"

#include <ncurses.h>

#define MAIN_MESSAGE "quit: Ctrl-Q | info: Ctrl-I"


struct s_state {
  int cursor_X;
  int cursor_Y;
  int pos_X;
  int pos_Y;
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
  bool is_spaces_instead_tab;
  int tab_size;
};


struct s_state g_state;
struct s_config g_config;
list_t *g_lines;
WINDOW *text_area;
WINDOW *footer;