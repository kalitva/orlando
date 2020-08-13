#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


/* editor.c */
void insert_char(int);
void insert_line(void);
/* cursor.c */
void cursor_to_up(void);
void cursor_to_down(void);
void cursor_to_right(void);
void cursor_to_start_line(void);
void cursor_to_end_line(void);
/* stack.c */
stack_t *new_stack(void);
void push(stack_t *, int);
int pop(stack_t *);
int peek(stack_t *);


static stack_t *brackets;


void init_brackets_stack()
{
  brackets = new_stack();
}

int is_separator(int ch)
{
  return isspace(ch) || ch == '\0' || strchr(",.()+-/*=~%<>[];", ch) != NULL;
}

bool is_opening_char(int ch)
{
  return ch == '{' || ch == '(' || ch == '[' || ch == '"' || ch == '\'';
}

int get_closing_char(int ch)
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

void insert(int ch)
{
  int closing_char;
  int previous_char;

  previous_char = peek(brackets);

  if (is_opening_char(ch) && previous_char != '"' && previous_char != '\'') {

    insert_char(ch);
    cursor_to_right();
    insert_char(closing_char = get_closing_char(ch));
    push(brackets, closing_char);

  } else if (ch == previous_char) {

    cursor_to_right();
    pop(brackets);

  } else {  

    insert_char(ch); 
    cursor_to_right();
  }
}

void insert_tab()
{
  int tab_size;
  line_t *line;

  if (g_config.is_spaces_instead_tab) {

		tab_size = g_config.tab_size;
	  while (tab_size--) {
 	    insert_char(' ');
    	cursor_to_right();
	  }

  } else {

  	line = g_lines->head->value;
  	insert_char('\t');
  	cursor_to_right();
  }
}

void indentation()
{
  line_t *previous;
  int offset = 0;

  if (!g_lines->head->previous || !g_config.is_spaces_instead_tab) {
  	insert_tab();
    return;
  }

  previous = ((node_t *) g_lines->head->previous)->value;
 
  while (previous->str[offset] == ' ') {
  	offset++;
  }

  if (offset == 0) {
  	insert_tab();
  	return;
  }

  while (offset--) {
    insert_char(' ');
    cursor_to_right();
  }
}











/*
char *keywords[] = {
  "switch", "if", "while", "for", "break", "continue", "return", "else",
  "struct", "union", "typedef", "static", "enum", "class", "case", 
  "#include", "#define",

  "int", "long", "double", "float", "char", "unsigned", "signed",
  "void", "NULL", NULL
};
*/