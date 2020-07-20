#include "defines.h"
#include "data.h"


/* output.c */
void refresh_screen();
void set_status_message(const char*, ...);
/* terminal.c */
int read_key();
/* editor.c */
void insert_tab();
void editor_insert_new_line();
void insert_char(int);
void delete_char(void);
void del_row(int);
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

void move_cursor(int key)
{
  t_line *line = (g_lines->head) ? g_lines->head->value : NULL;

  switch (key) {
    
    case ARROW_RIGHT:
      g_state.cursor_X++;
      break;

    case ARROW_LEFT:
      if (g_state.cursor_X > 0)
        g_state.cursor_X--;
      break;
  }
}

bool is_pair(int ch)
{
  return ch == '{' || ch == '(' || ch == '[' || ch == '"' || ch == '\'';
}

int find_pair(int ch)
{
  switch (ch) {
    case '{':
      return '}';
    case '(':
      return ')';
    case '[':
      return ']';
    case '"':
    case '\'':
      return ch;
  }
}

void print_content()
{
  for (t_node *current = g_lines->first; current; current = current->next) {
    t_line *line = current->value;
    printf("%s %lu\n", line->str, strlen(line->str));
  }
}

void process_keypress()
{
  static int quit_times = KILO_QUIT_TIMES;

  int ch = read_key();

  switch (ch) {

    case '\r':                                    /* next line */

      editor_insert_new_line();
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
//      print_content();
      exit(0);
      break;

    case CTRL_KEY('s'):

      editor_save();                              /* save */
      break;

    case HOME_KEY:                                /* to start line */
      break;

    case END_KEY:                                 /* to end line */
      break;

    case CTRL_KEY('d'):
      del_row(g_state.cursor_Y);

    case BACKSPACE:                               /* del character */

      move_cursor(ARROW_LEFT);
      delete_char();
      break;
      
    case DEL_KEY:                                 /* del character */

      delete_char();
      break;

    case PAGE_UP:                                 /* pageUp pageDown */
    case PAGE_DOWN:
      break;

    case ARROW_UP:                                /* move cursor */
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      move_cursor(ch);
      break;

    case '\x1b':
      break;

    default:                                     /* insert character */
      insert_char(ch); 
      move_cursor(ARROW_RIGHT);

      break;
  }

  refresh_screen();
}