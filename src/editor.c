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
}

void insert_char(int ch)
{
	if (is_empty(lines))
		append_line();

	Line *head = lines->head->value;

	if (head->len == 0)
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

void editor_del_char()
{
}