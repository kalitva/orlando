#include "defines.h"
#include "data.h"

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
