#include "defines.h"


/* input.c */
void cursor_to_down(void);
void cursor_to_end_line(void);
void cursor_to_start(void);
/* editor.c */
void insert_line(void);
/* cursor.c */
void top_line_to_up(void);


void open_file(char *file_name)
{
  char ch;
  FILE *fp;
  t_line *line;

  g_state.file_name = file_name;
  fp = fopen(file_name, "r");
  line = g_lines->head->value;

  if (!fp) {
    fp = fopen(file_name, "w"); /* create file */
    return;
  }
  
  while ((ch = getc(fp)) != EOF) {

    if (ch != '\n') {
      line->str[line->len++] = ch;
    } else {
      cursor_to_end_line();
      insert_line();
      cursor_to_down();
      line = g_lines->head->value;
    }
  }

  cursor_to_start();
  fclose(fp);
}

void editor_save()
{
}