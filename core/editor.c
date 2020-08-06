#include <stdlib.h>
#include <string.h>

/* input.c */
void cursor_to_start_line(void);
void cursor_to_end_line(void);
void cursor_to_up(void);
/* lnklist.c */
void append_node(list_t *, void *);
void insert_node(list_t *, void *);
void remove_first(list_t *);
void remove_last(list_t *);
void remove_head(list_t *);
bool is_empty_list(list_t *);
void head_to_next(list_t *);
/* syntax.c */
bool is_pair(int);
int find_pair(int);


line_t *new_line()
{
  line_t *line = malloc(sizeof(line_t));

  line->str = calloc(80, sizeof(int));
  line->capacity = 80;
  line->len = 0;

  return line;
}

void free_line(line_t *line)
{
  free(line->str);
  line->str = NULL;
  line->len = 0;
}

void insert_line()
{
  line_t *line;
  line_t *new = new_line();

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

  g_state.dirty++;
}

void delete_line()
{
  line_t *line = g_lines->head->value;

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

  g_state.dirty++;
}

void insert_char(int ch)
{
  line_t *line = g_lines->head->value;

  if (line->len > line->capacity - 1) {
    line->capacity *= 1.5;
    line->str = realloc(line->str, line->capacity * sizeof(int));
  }

  if (g_state.cursor_X == line->len) {
    line->str[line->len++] = ch;
    g_state.dirty++;
    return;
  }

  for (int i = line->len - 1; i >= g_state.cursor_X; i--) {
    line->str[i + 1] = line->str[i];
  }

  line->str[g_state.cursor_X] = ch;
  line->len++;

  g_state.dirty++;
}

void delete_char()
{
  line_t *line;
  line_t *nexline_t;

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
    nexline_t = g_lines->head->value;

    strcat(line->str, nexline_t->str);
    line->len += nexline_t->len;

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

  g_state.dirty++;
}