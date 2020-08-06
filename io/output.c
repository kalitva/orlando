#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_INIT { NULL, 0 }


/* terminal.c */
void get_window_size(int *, int *);


void buf_append(struct s_buffer *buffer, const char *str, int len)
{
  char *new = realloc(buffer->str, buffer->len + len);

  if (new == NULL) {
    return;
  }

  memcpy(&new[buffer->len], str, len);
  buffer->str = new;
  buffer->len += len;
}

void buf_free(struct s_buffer *buffer)
{
  free(buffer->str);
}

void print_line_numbers(struct s_buffer *buffer, int line)
{
  char line_number[7]; /* make number */
  sprintf(line_number, g_lines->size < 1000 ? "%3d| " : "%4d| ", line);

  buf_append(buffer, "\x1b[2m", 4);
  buf_append(buffer, line_number, g_lines->size < 1000 ? 5 : 6);
  buf_append(buffer, "\x1b[m", 3);
}

void print_lines(struct s_buffer *buffer)
{
  node_t *current;
  line_t *line;

  current = g_state.top_line;

  for (int y = 0; y < g_state.screen_rows && current; y++) {
  	print_line_numbers(buffer, g_state.top_line_number + y);	

  	line = current->value;
  	buf_append(buffer, line->str, line->len);

    buf_append(buffer, "\x1b[K", 3);
    buf_append(buffer, "\r\n", 4);

    current = current->next;
  }

  /* fill empty space */
  for (int y = g_lines->size; 
  		 y < g_state.screen_rows + g_state.top_line_number - 1; 
  		 y++) {
    buf_append(buffer, "\x1b[K", 3);
    buf_append(buffer, "\r\n", 2);
  }
}

void draw_topbar(struct s_buffer *buffer)
{
  buf_append(buffer, "\x1b[1m", 4); /* invent colors */

  char header[20];
  int space_left_length;
  int space_right_length;

  int header_length = snprintf(header, /* make header */
                               sizeof(header),
                               "%c%.20s ",
                               g_state.dirty ? '*' : ' ',
                               g_state.file_name ? g_state.file_name : "[No Name]");

  /* make white spaces around header */
  space_left_length = (g_state.screen_cols - header_length);

  char space_left[space_left_length];

  for (int i = 0; i < space_left_length; i++) {
    space_left[i] = ' ';
  }

  /* append all */
  buf_append(buffer, space_left, space_left_length);
  buf_append(buffer, header, header_length);
  buf_append(buffer, "\x1b[m", 3); /* invent colors back */
  buf_append(buffer, "\r\n", 2);
}

void draw_footer(struct s_buffer *buffer)
{
  char cursor[23]; /* make string, that contents line and column nimbers */
  int cursor_len; sprintf(cursor, 
              "line: %d column: %d", 
              g_state.cursor_Y + g_state.top_line_number,
              g_state.cursor_X + 1);
  
  int msg_len = strlen(g_state.status_msg);
  
  int space_len = (g_state.screen_cols - msg_len - strlen(cursor)); /* empty space */
  char space[space_len];
  for (int i = 0; i < space_len; i++) {
    space[i] = ' ';
  }

  buf_append(buffer, "\x1b[34m", 5); /* put footer in buffer */
  buf_append(buffer, g_state.status_msg, msg_len);
  buf_append(buffer, space, space_len - 1);
  buf_append(buffer, cursor, strlen(cursor));
  buf_append(buffer, " ", 1);
  buf_append(buffer, "\x1b[m", 4);

  g_state.status_msg = MAIN_MESSAGE;
}

void refresh_screen()
{
  struct s_buffer buffer = BUF_INIT;

  get_window_size(&g_state.screen_rows, &g_state.screen_cols);

  buf_append(&buffer, "\x1b[H", 3);

  print_lines(&buffer);
  draw_topbar(&buffer);
  draw_footer(&buffer);

  char buf[16];   /* set cursor position */
  sprintf(buf,
           "\x1b[%d;%dH",
           g_state.cursor_Y + 1,
           g_state.cursor_X + (g_lines->size < 1000 ? 6 : 7));

  buf_append(&buffer, buf, strlen(buf));

  write(STDOUT_FILENO, buffer.str, buffer.len); /* print all */
  buf_free(&buffer);
}

void set_status_message(const char *ftime, ...)
{
    va_list ap;
    va_start(ap, ftime);
    vsnprintf(g_state.status_msg, sizeof(g_state.status_msg), ftime, ap);
    va_end(ap);
}
