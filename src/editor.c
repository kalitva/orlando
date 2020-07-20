#include "defines.h"
#include "data.h"

/* lnklist.c */
void append_node(t_list*, void*);
void insert_node(t_list*, void*);
bool is_empty(t_list*);

void update_row(t_line *row)
{
}

t_line* new_line()
{
  t_line *line = malloc(sizeof(t_line));
  line->capacity = 80;
  line->len = 0;
  line->str = malloc(sizeof(int) * line->capacity);

  return line;
}

void insert_line()
{
  t_line *line = new_line();

  if (is_empty(g_lines)) {
    append_node(g_lines, line);
    return;
  }

  append_node(g_lines, line);
}

void insert_char(int ch)
{
	t_line *line = g_lines->head->value;

	if (line->len > line->capacity - 1) { /* increase memory */
		line->capacity *= 1.5;
		line->str = realloc(line->str, line->capacity * sizeof(int));
	}

	if (g_state.cursor_X == line->len) {	/* append char and return */
		line->str[line->len++] = ch;
		return;
	}

	for (int i = line->len - 1; i >= g_state.cursor_X; i--) /* or move chars to right */
		line->str[i + 1] = line->str[i];

	line->str[g_state.cursor_X] = ch;							/* and insert char */
  line->len++;								/*increase sring length */
}

void insert_tab()
{

}

void delete_char()
{
	t_line *line = g_lines->head->value;

  if (g_state.cursor_Y == 0 && line->len == 0 || line->len <= g_state.cursor_X)
    return;

	for (int i = g_state.cursor_X; i < line->len; i++) /* move chars to left */
		line->str[i] = line->str[i + 1];

	line->str[line->len - 1] = 0; /* delete char */
	line->len--;									/* decrease line length */
}