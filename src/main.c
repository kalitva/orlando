#include "defines.h"
#include "data.h"


/* terminal.c */
void disable_raw_mode(void);
void enable_raw_mode(void);
/* file_io.c */
void open_file(char*);
/* output.c */
void set_status_message(const char*, ...);
void refresh_screen(void);
/* input.c */
void process_keypress(void);
/* lnklist.c */
List* new_list(void);


void init_editor()
{
  E.cursor_X = 0; /* init state */
  E.cursor_Y = 0;
  E.row_offset = 0;
  E.col_offset = 0;
  E.num_rows = 0;
  E.dirty = false;
  E.file_name = NULL;
  E.status_msg[0] = '\0';
  E.syntax = NULL;

  lines = new_list();

  set_status_message(" Quit: Ctrl + 'q' "); /* message for footer */

  enable_raw_mode();                        /* set terminal */
  atexit(disable_raw_mode);    

  refresh_screen();
}

int main(int argc, char *argv[])
{
  init_editor(); 
    
  if (argc >= 2)        /* open file */
    open_file(argv[1]);

  while (true)          /* listen input */
    process_keypress();
}