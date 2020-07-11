#include "defines.h"
#include "data.h"


/* output.c */
void refresh_screen();
void set_status_message(const char*, ...);
/* terminal.c */
int editor_read_key();
/* editor_operations.c */
void editor_insert_new_line();
void insert_char(int);
void editor_del_char();
/* editor file_io.c */
void editor_save();


char *editor_prompt(char *prompt, void (*callback)(char *, int))
{
    size_t buf_size = 128;
    char *buf = malloc(buf_size);

    size_t buf_len = 0;
    buf[0] = '\0';

    while (1) {
        set_status_message(prompt, buf);
        refresh_screen();

        int ch = editor_read_key();
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

void editor_move_cursor(int key)
{
    erow *row = (E.cy >= E.num_rows) ? NULL : &E.row[E.cy];

    switch (key) {
        case ARROW_LEFT:
            if (E.cx != 0) {
                E.cx--;
            } else if (E.cy > 0) {
                E.cy--;
                E.cx = E.row[E.cy].size;
            }
            break;
        case ARROW_RIGHT:
            if (row && E.cx < row->size) {
                E.cx++;
            } else if (row && E.cx == row->size) {
                E.cy++;
                E.cx = 0;
            }
            break;
        case ARROW_UP:
            if (E.cy != 0)
                E.cy--;
            break;
        case ARROW_DOWN:
            if (E.cy < E.num_rows)
                E.cy++;
            break;
    }

    row = (E.cy >= E.num_rows) ? NULL : &E.row[E.cy];
    int row_len = row ? row->size : 0;
    if (E.cx > row_len)
        E.cx = row_len;
}

void process_keypress()
{
  static int quit_times = KILO_QUIT_TIMES;

  int ch = editor_read_key();

  switch (ch) {

    case '\r':

      editor_insert_new_line();
      break;

    case CTRL_KEY('q'):

      if (E.dirty && quit_times > 0) {
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

      E.cx = 0;
      break;

    case END_KEY:

      if (E.cy < E.num_rows)
        E.cx = E.row[E.cy].size;
      break;

    case BACKSPACE:
    case DEL_KEY:

      if (ch == DEL_KEY)
        editor_move_cursor(ARROW_RIGHT);
      editor_del_char();
      break;

    case PAGE_UP:
    case PAGE_DOWN:

      {
        if (ch == PAGE_UP) {
          E.cy = E.row_offset;
        } else if (ch == PAGE_DOWN) {
          E.cy = E.row_offset + E.screen_rows - 1;
            if (E.cy > E.num_rows)
              E.cy = E.num_rows;
        }

        int times = E.screen_rows;
        while (times--)
          editor_move_cursor(ch == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editor_move_cursor(ch);
      break;

    case CTRL_KEY('l'):
    case '\x1b':
      break;

    default:
      insert_char(ch);
      editor_move_cursor(ARROW_RIGHT);
      break;
  }

  refresh_screen();
}