#include "defines.h"


void insert_char(int);
void cursor_to_right(void);


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
  static int closing_char;

  if (is_opening_char(ch)) {

    insert_char(ch);
    cursor_to_right();
    insert_char(closing_char = get_closing_char(ch));

  } else if (ch == closing_char) {

    cursor_to_right();
    closing_char = 0;

  } else {  

    insert_char(ch); 
    cursor_to_right();
  }
}

int editor_syntax_to_color(int hl)
{
  switch (hl) {
    
    case HL_COMMENT:
      return 30;

    case HL_MLCOMMENT:
      return 30;

    case HL_KEYWORD1:
      return 33;

    case HL_KEYWORD2:
      return 34;

    case HL_STRING:
      return 36;

    case HL_NUMBER:
      return 33;

    case HL_MATCH:
      return 34;

    default:
      return 37;
  }
}

void editor_select_syntax_highlight()
{
}
