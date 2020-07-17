#include "defines.h"
#include "data.h"


int editor_syntax_to_color(int);
void get_window_size(int*, int*);


void buf_append(struct char_buffer *buffer, const char *str, int len)
{
  char *new = realloc(buffer->str, buffer->len + len);

  if (new == NULL)
    return;

  memcpy(&new[buffer->len], str, len);
  buffer->str = new;
  buffer->len += len;
}

void buf_free(struct char_buffer *buffer) 
{
  free(buffer->str);
}

void scroll()
{
    if (E.cursor_Y < E.row_offset)
        E.row_offset = E.cursor_Y;

    if (E.cursor_Y >= E.row_offset + E.screen_rows)
        E.row_offset = E.cursor_Y - E.screen_rows + 1;

    if (E.cursor_X < E.col_offset)
        E.col_offset = E.cursor_X;

    if (E.cursor_X >= E.col_offset + E.screen_cols)
        E.col_offset = E.cursor_X - E.screen_cols + 1;
}

void draw_line_numbers(struct char_buffer *buffer, int line)
{
  char line_number[6]; /* make number */
  sprintf(line_number, E.num_rows < 1000 ? "%3d " : "%4d ", line + 1);

  buf_append(buffer, "\x1b[m", 3);
  buf_append(buffer, "\x1b[30m", 5); /* color gray */
  buf_append(buffer, line_number, 5);
  buf_append(buffer, "\x1b[m", 3);
}

void draw_rows(struct char_buffer *buffer)
{
  for (Node *current = lines->first; current; current = current->next) {
  	int line_number = 0;
  	draw_line_numbers(buffer, line_number);	

  	Line *line = current->value;
  	buf_append(buffer, line->str, line->len);
  }

  for (int y = lines->size; y < E.screen_rows; y++) {

    if (y == 0)
      draw_line_numbers(buffer, 0);
    else
      buf_append(buffer, " ", 1);

    buf_append(buffer, "\x1b[K", 3);
    buf_append(buffer, "\r\n", 2);
  }
}

void draw_topbar(struct char_buffer *buffer)
{
  buf_append(buffer, "\x1b[7m", 4); /* invent colors */

  char header[20];
  int space_left_length;
  int space_right_length;

  int header_length = snprintf(header, /* make header */
                               sizeof(header),
                              "%.20s%c",
                              E.file_name ? E.file_name : "[No Name]",
                              E.dirty ? '*' : ' ');

  /* make white spaces around header */
  space_left_length = (E.screen_cols - header_length) / 2;
  space_right_length = (E.screen_cols - space_left_length - header_length);

  char space_left[space_left_length];
  char space_right[space_right_length];

  for (int i = 0; i < space_left_length; i++)
    space_left[i] = ' ';
  for (int i = 0; i < space_right_length; i++)
    space_right[i] = ' ';

  /* append all */
  buf_append(buffer, space_left, space_left_length);
  buf_append(buffer, header, header_length);
  buf_append(buffer, space_right, space_right_length);
  buf_append(buffer, "\x1b[m", 3); /* invent colors back */
  buf_append(buffer, "\r\n", 2);
}

void draw_footer(struct char_buffer *buffer)
{
  char cursor[23]; /* make string, that contents line and column nimbers */
  int cursor_len = snprintf(cursor, 
              sizeof cursor, 
              "line: %d column: %d", 
              E.cursor_Y + 1,
              E.cursor_X + 1);
  
  int msg_len = strlen(E.status_msg);
  
  int space_len = (E.screen_cols - msg_len - cursor_len); /* empty space */
  char space[space_len];
  for (int i = 0; i < space_len; i++)
    space[i] = ' ';

  buf_append(buffer, "\x1b[7m", 4); /* put footer in buffer */
  buf_append(buffer, E.status_msg, msg_len);
  buf_append(buffer, space, space_len - 1);
  buf_append(buffer, cursor, cursor_len);
  buf_append(buffer, " ", 1);
  buf_append(buffer, "\x1b[m", 4);
}

void refresh_screen()
{
  struct char_buffer buffer = BUF_INIT; /* create buffer */

  get_window_size(&E.screen_rows, &E.screen_cols); /* get screen params */

  scroll(); /* move cursor */

//  buf_append(&buffer, "\x1b[?25l", 6);  
  buf_append(&buffer, "\x1b[H", 3);

  draw_topbar(&buffer); /* put all in buffer */
  draw_rows(&buffer);
  draw_footer(&buffer);

  char buf[16];   /* find cursor position */
  snprintf(buf,
           sizeof(buf),
           "\x1b[%d;%dH",
           (E.cursor_Y - E.row_offset) + 2,
           (E.cursor_X - E.col_offset) + (E.num_rows < 1000 ? 5 : 6));

  buf_append(&buffer, buf, strlen(buf));
//  buf_append(&buffer, "\x1b[?25h", 6);

  write(STDOUT_FILENO, buffer.str, buffer.len); /* print all */
  buf_free(&buffer); /* free memory */
}

void set_status_message(const char *ftime, ...)
{
    va_list ap;
    va_start(ap, ftime);
    vsnprintf(E.status_msg, sizeof(E.status_msg), ftime, ap);
    va_end(ap);
}