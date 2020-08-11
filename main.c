#include <stdlib.h>
#include <stdbool.h>

#include "global.h"

/* main.c */
void init(void);
/* terminal.c */
void enable_screen(void);
void enable_mouse_tracking(void);
/* file_io.c */
void open_file(char *);
/* output.c */
void refresh_screen(void);
void create_text_area(void);
/* input.c */
void process_keypress(void);
/* lnklist.c */
list_t *new_list(void);
void init_head(list_t *);
/* editor.c */
void insert_line(void);
void init_top_line(void);


int main(int argc, char *argv[])
{
  init(); 
    
  if (argc >= 2) {
    open_file(argv[1]);
  }

  while (true) {
    process_keypress();
  }
}

void init()
{
  g_lines = new_list(); /* global variable for text */

  g_config.tab_size = 2;

  g_state.cursor_X = 0;
  g_state.cursor_Y = 0;
  g_state.dirty = false;
  g_state.is_new_file = false;
  g_state.status_msg = MAIN_MESSAGE;

  insert_line();  /* insert first empty line */
  init_head(g_lines);

  g_state.top_line = g_lines->first;
  g_state.top_line_number = 1;

  initscr();
  raw();
  getmaxyx(stdscr, g_state.screen_rows, g_state.screen_cols);
  create_text_area();
  noecho();
}