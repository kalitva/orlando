#include "defines.h"


/* terminal.c */
void enable_raw_mode(void);
void disable_raw_mode(void);
/* file_io.c */
void open_file(char*);
/* output.c */
void set_status_message(const char*, ...);
void refresh_screen(void);
/* input.c */
void process_keypress(void);
/* lnklist.c */
t_list* new_list(void);
void insert_line(void);
void init_head(t_list*);


void init()
{
  g_state.cursor_X = 0;
  g_state.cursor_Y = 0;
  g_state.dirty = false;
  g_state.status_msg[0] = '\0';

  g_lines = new_list(); /* global variable for text */

  insert_line();  /* insert first empty line */
  init_head(g_lines);
  set_status_message(" Quit: Ctrl + 'q' "); /* message for footer */
  enable_raw_mode();      /* set terminal */
  atexit(disable_raw_mode);
}

int main(int argc, char *argv[])
{
  init(); 
    
  if (argc >= 2)        /* open file */
    open_file(argv[1]);

  refresh_screen();

  while (true)          /* listen input */
    process_keypress();
}