#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "data.h"


int editor_row_cx_to_rx(erow*, int);
int editor_syntax_to_color(int);
void get_window_size(int*, int*);


                                /*** char buffer ***/


void buf_append(struct char_buffer *buffer, const char *s, int len)
{
    char *new = realloc(buffer->str, buffer->len + len);

    if (new == NULL)
        return;

    memcpy(&new[buffer->len], s, len);
    buffer->str = new;
    buffer->len += len;
}

void buf_free(struct char_buffer *buffer) {
    free(buffer->str);
}
                                /*** ***/

void editor_scroll()
{
    E.rx = 0;
    if (E.cy < E.num_rows)
        E.rx = editor_row_cx_to_rx(&E.row[E.cy], E.cx);

    if (E.cy < E.row_offset)
        E.row_offset = E.cy;

    if (E.cy >= E.row_offset + E.screen_rows)
        E.row_offset = E.cy - E.screen_rows + 1;

    if (E.rx < E.col_offset)
        E.col_offset = E.rx;

    if (E.rx >= E.col_offset + E.screen_cols)
        E.col_offset = E.rx - E.screen_cols + 1;
}

void draw_line_numbers(struct char_buffer *buffer, int line)
{
  char line_number[5]; /* make number */
  sprintf(line_number, "%3d ", line + 1);

  buf_append(buffer, "\x1b[35m", 5); /* color gray */
  buf_append(buffer, line_number, 4);
  buf_append(buffer, "\x1b[39m", 5);
}

void draw_rows(struct char_buffer *buffer)
{
  int file_row;

  for (int y = 0; y < E.screen_rows; y++) {
    file_row = y + E.row_offset;

    if (file_row >= E.num_rows) { /* if rows don't fill screen, insert space */

      if (y == 0) 
        draw_line_numbers(buffer, 0); /* number of line for first row */

      buf_append(buffer, " ", 1);

    } else { 
          draw_line_numbers(buffer, file_row); /* insert number of line */

          int len = E.row[file_row].rsize - E.col_offset;
          if (len < 0)
              len = 0;
          if (len > E.screen_cols)
              len = E.screen_cols;
          
          char *ch = &E.row[file_row].render[E.col_offset];
          unsigned char *hl = &E.row[file_row].hl[E.col_offset];
          int current_color = -1;

          for (int j = 0; j < len; j++) {

              if (iscntrl(ch[j])) {
                  char sym = (ch[j] <= 26) ? '@' + ch[j] : '?';
                  buf_append(buffer, &sym, 1);

                  if (current_color != -1) {
                      char buf[16];
                      int clen = snprintf(buf,
                                          sizeof(buf),
                                          "\x1b[%dm",
                                          current_color);
                      buf_append(buffer, buf, clen);
                  }

              } else if (hl[j] == HL_NORMAL) {

                  if (current_color != -1) {
                      buf_append(buffer, "\x1b[39m", 5);
                      current_color = -1;
                  }
                  buf_append(buffer, &ch[j], 1);

              } else {

                  int color = editor_syntax_to_color(hl[j]);

                  if (color != current_color) {
                      current_color = color;
                      char buf[16];
                      int clen = snprintf(buf, sizeof(buf),
                                          "\x1b[%dm",
                                          color);
                  buf_append(buffer, buf, clen);
                  }

                  buf_append(buffer, &ch[j], 1);
              }
          }
          buf_append(buffer, "\x1b[39m", 5);
      }

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
                     E.file_name ? E.file_name : "[Untitled]",
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
  char cursor[23]; /* make string, that contents line an column nimbers */
  int cursor_len = snprintf(cursor, 
              sizeof cursor, 
              "line: %d column: %d", 
              E.cy + 1,
              E.rx + 1);
  
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

  editor_scroll(); /* move cursor */

    buf_append(&buffer, "\x1b[?25l", 6);  
    buf_append(&buffer, "\x1b[H", 3);

  draw_topbar(&buffer); /* put all in buffer */
  draw_rows(&buffer);
  draw_footer(&buffer);

    char buf[32];
    snprintf(buf,
             sizeof(buf),
             "\x1b[%d;%dH",
             (E.cy - E.row_offset) + 2,
             (E.rx - E.col_offset) + 5); /* !!!!!!!! replace 5 with offset */
    buf_append(&buffer, buf, strlen(buf));

    buf_append(&buffer, "\x1b[?25h", 6);

  write(STDOUT_FILENO, buffer.str, buffer.len); /* print all */
  buf_free(&buffer); /* free memory */
}

void editor_set_status_message(const char *ftime, ...)
{
    va_list ap;
    va_start(ap, ftime);
    vsnprintf(E.status_msg, sizeof(E.status_msg), ftime, ap);
    va_end(ap);
    E.status_msg_time = time(NULL);
}
