#include <termios.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "data.h"


void die(char *);
void disable_raw_mode();
void enable_raw_mode();
int get_window_size(int*, int*);
void editor_open(char*);
void editor_refresh_screen();
void editor_set_status_message(const char*, ...);
void editor_process_keypress();


void init_editor()
{
    E.cx = 0;
    E.cy = 0;
    E.rx = 0;
    E.row_offset = 0;
    E.col_offset = 0;
    E.num_rows = 0;
    E.row = NULL;
    E.dirty = 0;
    E.file_name = NULL;
    E.status_msg[0] = '\0';
    E.status_msg_time = 0;
    E.syntax = NULL;

    if (get_window_size(&E.screen_rows, &E.screen_cols) == -1)
        die("get_window_size");

    E.screen_rows -= 2;
}

int main(int argc, char *argv[])
{
  enable_raw_mode();
  atexit(disable_raw_mode);    

  init_editor();  
    
  if (argc >= 2)
    editor_open(argv[1]);

  editor_set_status_message("HELP: Ctrl-S = save | Ctrl-Q = quit");

  while (true) {
      editor_refresh_screen();
      editor_process_keypress();
  }
  
}