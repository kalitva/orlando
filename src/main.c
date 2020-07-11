#include "defines.h"
#include "data.h"


/* terminal.c */
void disable_raw_mode();
void enable_raw_mode();
/* file_io.c */
void open_file(char*);
/* output.c */
void set_status_message(const char*, ...);
/* input.c */
void process_keypress();


void init_editor()
{
  E.cx = 0; /* init state */
  E.cy = 0;
  E.rx = 0;
  E.row_offset = 0;
  E.col_offset = 0;
  E.num_rows = 0;
  E.row = NULL;
  E.dirty = false;
  E.file_name = NULL;
  E.status_msg[0] = '\0';
  E.syntax = NULL;

  set_status_message(" Quit: Ctrl + 'q' "); /* message for footer */
  enable_raw_mode();                        /* set terminal */
  atexit(disable_raw_mode);    
}

int main(int argc, char *argv[])
{
  init_editor(); 
    
  if (argc >= 2)        /* open file */
    open_file(argv[1]);

  while (true)          /* listen input */
    process_keypress();
}