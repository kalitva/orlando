#include <termios.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "data.h"


void disable_raw_mode();
void enable_raw_mode();
int get_window_size(int*, int*);
void editor_open(char*);
void refresh_screen();
void editor_set_status_message(const char*, ...);
void process_keypress();

void editor_move_cursor(char);

void init_editor()
{
  E.cx = 0;
  E.cy = 0;
  E.rx = 0;
  E.row_offset = 0;
  E.col_offset = 0;
  E.num_rows = 0;
  E.row = NULL;
  E.dirty = false;
  E.file_name = NULL;
  E.status_msg[0] = '\0';
  E.status_msg_time = 0;
  E.syntax = NULL;

  get_window_size(&E.screen_rows, &E.screen_cols);
  editor_set_status_message(" Quit: Ctrl + 'q' ");
}

int main(int argc, char *argv[])
{
  enable_raw_mode();
  atexit(disable_raw_mode);    

  init_editor();  
    
  if (argc >= 2)
    editor_open(argv[1]);

  while (true) {
    refresh_screen(); /* output.c */
    process_keypress(); /* input.c */
  }
  
}
