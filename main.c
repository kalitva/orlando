#include <stdlib.h>
#include <stdbool.h>

#include "global.h"


/* main.c */
void init(void);
/* file_io.c */
void open_file(char *);
/* output.c */
void create_text_area(void);
void create_footer(void);
void init_colors(void);
void print(void);
/* input.c */
void process_keypress(void);
/* lnklist.c */
list_t *new_list(void);
void init_head(list_t *);
/* editor.c */
void insert_line(void);
void init_top_line(void);
/* syntax.c */
void init_brackets_stack(void);


int main(int argc, char *argv[])
{
  init(); 
    
  if (argc >= 2) {
    open_file(argv[1]);
  }

  print();

  while (true) {
    process_keypress();
  }
}

void init()
{
  g_lines = new_list(); /* global variable for text */

  g_config.tab_size = 2;
  g_config.is_spaces_instead_tab = true;

  insert_line();  /* insert first empty line */
  init_head(g_lines); /* head always is current line */
  init_brackets_stack();

  g_state.top_line = g_lines->first; /* top_line - first line for view */
  g_state.top_line_number = 1;
  g_state.cursor_X = 0;
  g_state.cursor_Y = 0;
  g_state.pos_X = 0;
  g_state.dirty = false;
  g_state.is_new_file = false;
  g_state.status_msg = MAIN_MESSAGE;
  g_state.file_name = NULL;

  initscr(); /* init ncurses block */
  raw();
  noecho();
  getmaxyx(stdscr, g_state.screen_rows, g_state.screen_cols);

  create_text_area();
  create_footer();
  init_colors();
}

void quit()
{
  endwin();
  exit(0);
}