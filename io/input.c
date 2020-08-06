#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include <termios.h>

#define CTRL_KEY(k) ((k) & 0x1f)


/* terminal.c */
void get_window_size(int *, int *);
int handle_escape_sequence(void);
int handle_mouse_event();
void quit(void);
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


int read_key()
{
  char ch;
  int screen_rows;
  int screen_cols;

  while (!read(STDIN_FILENO, &ch, 1)) { /* listen to input */

    get_window_size(&screen_rows, &screen_cols);

    if (g_state.screen_rows != screen_rows 
        || g_state.screen_cols != screen_cols) {
      refresh_screen();
    }
  }

  if (ch == '\x1b') {
    return handle_escape_sequence();
  } else {
    return ch;
  }

}

void process_keypress()
{
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

    case CTRL_KEY('s'):
      save_file();
      break;

    case HOME_KEY:
    	cursor_to_start_line();
      break;

    case END_KEY:
    	cursor_to_end_line();
      break;

    case CTRL_HOME_KEY:
    	cursor_to_start();
    	break;

    case CTRL_END_KEY:
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

    case CTRL_KEY('q'):                           /* quit */
      quit();

    case '\x1b':
      break;

    default:
    	insert(ch);
  }

  refresh_screen();
}