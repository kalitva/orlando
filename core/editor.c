#include <stdlib.h>
#include <string.h>

/* input.c */
void cursor_to_start_line(void);
void cursor_to_end_line(void);
void cursor_to_up(void);
/* lnklist.c */
void append_node(t_list *, void *);
void insert_node(t_list *, void *);
void remove_first(t_list *);
void remove_last(t_list *);
void remove_head(t_list *);
bool is_empty_list(t_list *);
void head_to_next(t_list *);
/* syntax.c */
bool is_pair(int);
int find_pair(int);


t_line *new_line()
{
  t_line *line = malloc(sizeof(t_line));

  line->str = malloc(sizeof(int) * 80);
  line->capacity = 80;
  line->len = 0;

  return line;
}

void free_line(t_line *line)
{
  free(line->str);
  line->str = NULL;
  line->len = 0;
}

void insert_line()
{
  t_line *line;
  t_line *new = new_line();

  if (is_empty_list(g_lines)) {  /* first insertion */
    append_node(g_lines, new);
    return;
  }

  line = g_lines->head->value;

  new->len = (line->len - g_state.cursor_X);

  for (int i = 0; i < new->len; i++) {
    new->str[i] = line->str[g_state.cursor_X + i];
    line->str[g_state.cursor_X + i] = 0;
    line->len--;
  }

  cursor_to_start_line();
  insert_node(g_lines, new);
}

void delete_line()
{
  t_line *line = g_lines->head->value;

  free_line(line);
  g_state.cursor_X = 0;

  if (g_lines->size == 1) {
    return;
  }

  if (g_state.top_line == g_lines->last) {
    cursor_to_up();
  }

  if (g_lines->head == g_lines->first) {

    remove_first(g_lines);
    g_lines->head = g_lines->first;
    g_state.top_line = g_lines->first;

  } else if (g_lines->head == g_lines->last) {

    remove_last(g_lines);
    g_lines->head = g_lines->last;
    g_state.cursor_Y--;
    cursor_to_end_line();

  } else {

    if (g_state.top_line == g_lines->head) {
      g_state.top_line = g_lines->first;

      for (int i = 0; i < g_state.top_line_number; i++)
        g_state.top_line = g_state.top_line->next;
    }

    remove_head(g_lines);
    g_lines->head = g_lines->first;

    for (int i = 1; i < (g_state.cursor_Y + g_state.top_line_number); i++) {
      g_lines->head = g_lines->head->next;
    }

  }
}

void insert_char(int ch)
{
  t_line *line = g_lines->head->value;

  if (!line->str) {
    line->str = malloc(sizeof(int) * line->capacity);
  } else if (line->len > line->capacity - 1) {
    line->capacity *= 1.5;
    line->str = realloc(line->str, line->capacity * sizeof(int));
  }

  if (g_state.cursor_X == line->len) {
    line->str[line->len++] = ch;
    return;
  }

  for (int i = line->len - 1; i >= g_state.cursor_X; i--) {
    line->str[i + 1] = line->str[i];
  }

  line->str[g_state.cursor_X] = ch;
  line->len++;
}

void delete_char()
{
  t_line *line;
  t_line *next_line;

  line = g_lines->head->value;

  if (line->len == g_state.cursor_X && g_lines->head == g_lines->last) {
    return;
  }

  if (line->len == 0) {
    delete_line();
    return;
  }

  if (line->len == g_state.cursor_X) {

    head_to_next(g_lines);
    next_line = g_lines->head->value;

    strcat(line->str, next_line->str);
    line->len += next_line->len;

    remove_head(g_lines);

    g_lines->head = g_lines->first;

    for (int i = 1; i < (g_state.cursor_Y + g_state.top_line_number); i++) {
      g_lines->head = g_lines->head->next;
    }

    return;
  }

  for (int i = g_state.cursor_X; i < line->len; i++) { /* move chars to left */
    line->str[i] = line->str[i + 1];
  }

  line->str[line->len - 1] = 0; /* delete char */
  line->len--;
}