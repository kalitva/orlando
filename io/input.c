#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)


/* terminal.c */
int read_key(void);
/* editor.c */
void insert_char(int);
void delete_char(void);
void insert_line(void);
void delete_line(void);
/* cursor.c */
void cursor_to_up(void);
void cursor_to_down(void);
void cursor_to_left(void);
void cursor_to_right(void);
void page_up(void);
void page_down(void);
void cursor_to_start_line(void);
void cursor_to_end_line(void);
void cursor_to_start(void);
void cursor_to_end(void);
/* file_io.c */
void save_file(void);
/* output.c */
void refresh_screen(void);
void set_status_message(const char *, ...);
/* syntax.c */
void insert(int);
void insert_tab(void);
void indentation(void);


void process_keypress()
{
  static int quit_times = 2;
  int ch = read_key();

  switch (ch) {

    case '\r':
      insert_line();
      cursor_to_down();
      indentation();
      break;

    case '\t':
      insert_tab();
      break;

    case CTRL_KEY('q'):                           /* quit */
      if (g_state.dirty && quit_times > 0) {
        set_status_message("WARNING! File has unsaved changes!", quit_times);
          quit_times--;
          return;
      }
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;

    case CTRL_KEY('s'):
      save_file();
      break;

    case HOME_KEY:
    	cursor_to_start_line();
      break;

    case END_KEY:
    	cursor_to_end_line();
      break;

    case CTRL_KEY('h'):
    	cursor_to_start();
    	break;

    case CTRL_KEY('e'):
    	cursor_to_end();
    	break;	

    case CTRL_KEY('d'):
      delete_line();
      break;

    case BACKSPACE:
      cursor_to_left();
      delete_char();
      break;

    case DEL_KEY:
      delete_char();
      break;

    case PAGE_UP:
    	page_up();
    	break;

    case PAGE_DOWN:
      page_down();
      break;

    case ARROW_UP:
      cursor_to_up();
      break;

    case ARROW_DOWN:
      cursor_to_down();
      break;

    case ARROW_LEFT:
      cursor_to_left();
      break;

    case ARROW_RIGHT:
      cursor_to_right();
      break;

    case '\x1b':
      break;

    default:
    	insert(ch);
      break;
  }

  refresh_screen();
}