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
t_stack *new_stack(void);
void push(t_stack *, int);
int pop(t_stack *);
int peek(t_stack *);


char *C_HL_extensions[] = { ".c", ".h", ".cpp", NULL };
char *C_HL_keywords[] = {
  "switch", "if", "while", "for", "break", "continue", "return", "else",
  "struct", "union", "typedef", "static", "enum", "class", "case", 
  "#include", "#define",

  "int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|",
  "void|", "NULL|", NULL
};


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
  static t_stack *stack;
  int closing_char;
  int previous_char;

  if (!stack) {
    stack = new_stack();
  }

  previous_char = peek(stack);

  if (is_opening_char(ch) && previous_char != '"' && previous_char != '\'') {

    insert_char(ch);
    cursor_to_right();
    insert_char(closing_char = get_closing_char(ch));
    push(stack, closing_char);

  } else if (ch == previous_char) {

    cursor_to_right();
    pop(stack);

  } else {  

    insert_char(ch); 
    cursor_to_right();
  }
}

void insert_tab()
{
  int tab_size = g_config.tab_size;

  while (tab_size--) {
    insert_char(' ');
    cursor_to_right();
  }
}
  
void indentation()
{
  t_line *previous;

  int offset = 0;
  int i = 0;

  if (!g_lines->head->previous) {
    return;
  }

  previous = ((t_node *) g_lines->head->previous)->value;
 
  while (previous->str[offset] == ' ') {
    offset++;
  }

  while (i++ < offset) {
    insert_char(' ');
    cursor_to_right();
  }

  if (previous->str[previous->len - 1] == '{') {

    insert_line();
    insert_tab();

    cursor_to_down();
    cursor_to_start_line();

    while (--i > 0) {
      insert_char(' ');
    }

    cursor_to_up();
    cursor_to_end_line();
  }
}