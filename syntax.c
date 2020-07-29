#include <ctype.h>
#include <string.h>
#include <stdbool.h>


/* editor.c */
void insert_char(int);
/* cursor.c */
void cursor_to_right(void);
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

void editor_select_syntax_highlight()
{
}
