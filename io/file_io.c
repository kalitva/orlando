#include <stdio.h>
#include <string.h>

/* input.c */
void cursor_to_down(void);
void cursor_to_end_line(void);
void cursor_to_start(void);
/* output */
void print(void);
/* editor.c */
void insert_line(void);
/* cursor.c */
void top_line_to_up(void);


void open_file(char *file_name)
{
  char ch;
  FILE *fp;
  line_t *line;

  g_state.file_name = file_name;
  fp = fopen(file_name, "r");
  line = g_lines->head->value;

  if (!fp) {
    fp = fopen(file_name, "w"); /* create file */
    g_state.is_new_file = true;
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
  cursor_to_end_line();
  print();
  fclose(fp);

  g_state.dirty = false;
}

void save_file()
{
  FILE *fp;

  if (!g_state.file_name) {
    g_state.file_name = "Noname";
  }

  fp = fopen(g_state.file_name, "w");

  for (node_t *current = g_lines->first; 
       current->next; 
       current = current->next) {
    fprintf(fp, "%s\n", ((line_t *) (current->value))->str);
  }
  fprintf(fp, "%s", ((line_t *) (g_lines->last->value))->str);

  g_state.status_msg = " File saved";
  g_state.dirty = false;

  fclose(fp);
}