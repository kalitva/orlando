#include <stdlib.h>
#include <stdbool.h>

#include "global.h"


/* terminal.c */
void enable_screen(void);
void disable_screen(void);
/* file_io.c */
void open_file(char *);
/* output.c */
void set_status_message(const char *, ...);
void refresh_screen(void);
/* input.c */
void process_keypress(void);
/* lnklist.c */
t_list *new_list(void);
void init_head(t_list *);
/* editor.c */
void insert_line(void);
void init_top_line(void);


void init()
{
  g_lines = new_list(); /* global variable for text */

  g_state.cursor_X = 0;
  g_state.cursor_Y = 0;
  g_state.dirty = false;
  g_state.status_msg[0] = '\0';

  insert_line();  /* insert first empty line */
  init_head(g_lines);

  g_state.top_line = g_lines->first;
  g_state.top_line_number = 1;

  set_status_message(" Quit: Ctrl + 'q' "); /* message for footer */
  enable_screen();
  atexit(disable_screen);
}

int main(int argc, char *argv[])
{
  init(); 
    
  if (argc >= 2) {
    open_file(argv[1]);
  }

  refresh_screen();

  while (true) {
    process_keypress();
  }
}