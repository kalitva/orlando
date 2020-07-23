#include "defines.h"


/* output.c */
void refresh_screen();
void set_status_message(const char*, ...);
/* terminal.c */
int read_key();
/* editor.c */
void insert_char(int);
void delete_char(void);
void insert_line(void);
void delete_line(void);
/* editor file_io.c */
void editor_save();
/* lnklist.c */
void head_to_next(t_list*);
void head_to_previous(t_list*);


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

void cursor_to_up()
{
  if (!g_lines->head->previous)
    return;

  t_line *line;

  head_to_previous(g_lines);  
  line = g_lines->head->value;
  g_state.cursor_Y--;
  g_state.cursor_X = g_state.cursor_X > line->len
                   ? line->len
                   : g_state.cursor_X;
}

void cursor_to_down()
{
  if (!g_lines->head->next)
    return;

  t_line *line;

  head_to_next(g_lines);
  line = g_lines->head->value;
  g_state.cursor_Y++;
  g_state.cursor_X = g_state.cursor_X > line->len
                   ? line->len
                   : g_state.cursor_X;
}

void cursor_to_left()
{
  t_line *line;

  if (g_state.cursor_X > 0) {
    g_state.cursor_X--;
  } else if (g_state.cursor_Y != 0) {
    cursor_to_up();
    line = g_lines->head->value;
    g_state.cursor_X = line->len;
  }
}

void cursor_to_right()
{
  t_line *line = g_lines->head->value;

  if (g_state.cursor_X < line->len) {
    g_state.cursor_X++;
  } else if (g_lines->head != g_lines->last) {
    cursor_to_down();
    g_state.cursor_X = (g_state.cursor_Y != 0) ? 0 : g_state.cursor_X;
  }
}

void cursor_to_start_line()
{
	g_state.cursor_X = 0;
}

void cursor_to_end_line()
{
	t_line *line = g_lines->head->value;

	g_state.cursor_X = line->len;
}

void cursor_to_start()
{
	g_state.cursor_X = 0;
	g_state.cursor_Y = 0;
	g_lines->head = g_lines->first;
}

void cursor_to_end()
{
	t_line *line = g_lines->last->value;

	g_state.cursor_Y = g_lines->size - 1;
	g_state.cursor_X = line->len;
	g_lines->head = g_lines->last;
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

void process_keypress()
{
  static int quit_times = 2;
  int ch = read_key();

  switch (ch) {

    case '\r':                                    /* next line */
      insert_line();
      cursor_to_down();
      break;

    case '\t':
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
    case PAGE_DOWN:
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
      insert_char(ch); 
      cursor_to_right();
      break;
  }

  refresh_screen();
}