#include "defines.h"
#include "data.h"

/* lnklist.c */
void append_node(List*, void*);
bool is_empty(List*);

void update_row(Line *row)
{
}

void editor_row_append_string(Line *row, char *str, size_t len)
{
}

void insert_line(Node *node)
{
}

void editor_free_row(Line *row)
{
}

void del_row(int at)
{
}

void row_insert_char(Line *row, int at, int ch)
{
}

void append_line()
{
	Line *line = malloc(sizeof(Line));

	append_node(lines, line);
	lines->head = lines->last;

	lines->size++;
}

void insert_char(int ch)
{
	if (is_empty(lines))
		append_line();

	Line *head = lines->head->value;

	if (!head->str)
		head->str = malloc(80);

	head->str[head->len++] = ch;
}

void insert_tab()
{

}

void editor_insert_new_line()
{
}

void editor_row_del_char(Line *row, int at)
{
}

void delete_char()
{
	if (E.cursor_X == 0 && E.cursor_Y == 0) /* abort if editor is empty */
		return;

	Line *line = lines->head->value;

	for (int i = E.cursor_X - 1; i < line->len; i++) /* move chars to left */
		line->str[i] = line->str[i + 1];

	line->str[line->len - 1] = 0; /* delete char */
	line->len--;									/* decrease line length */
}