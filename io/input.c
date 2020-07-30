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
void editor_save(void);
/* output.c */
void refresh_screen(void);
void set_status_message(const char *, ...);
/* syntax.c */
void insert(int);
void insert_tab();
void indentation();


char *editor_prompt(char *prompt, void (*callback)(char *, int))
{
    size_t buf_size = 128;
    char *buf = malloc(buf_size);

    size_t buf_len = 0;
    buf[0] = '\0';

    while (1) {
        set_status_message(prompt, buf);
        refresh_screen();

        int ch = read_key();
        if (ch == DEL_KEY || ch == CTRL_KEY('h') || ch == BACKSPACE) {
            if (buf_len != 0)
                buf[--buf_len] = '\0';
        } else if (ch == '\x1b') {
            set_status_message(prompt, buf);
            if (callback)
                callback(buf, ch);
            free(buf);
            return NULL;
        } else if (ch == '\r') {
            if (buf_len != 0) {
                set_status_message("");
                if (callback)
                    callback(buf, ch);
                return buf;
            }
        } else if (!iscntrl(ch) && ch < 128) {
            if (buf_len == buf_size - 1) {
                buf_size *= 2;
                buf = realloc(buf, buf_size);
            }
            buf[buf_len++] = ch;
            buf[buf_len] = '\0';
        }

        if (callback)
            callback(buf, ch);
    }
}

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
      editor_save();
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