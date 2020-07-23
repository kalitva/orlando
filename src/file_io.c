#include "defines.h"


/* input.c */
char *editor_prompt(char *prompt, void (*callback)(char *, int));
void cursor_to_down();
/* editor.c */
void insert_line();
void delete_line();
/* output.c */
void set_status_message(const char*, ...);


char *editor_rows_to_string(int *buf_len)
{
}

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
  		g_state.cursor_X = line->len;
  		insert_line();
  		head_to_next();
	  	line = g_lines->head->value;
  	}
  }

  delete_line();
  cursor_to_start();
  g_state.dirty = false;

  fclose(fp);
}

void editor_save()
{
    if (g_state.file_name == NULL) {
        g_state.file_name = editor_prompt("Save as: %s" "(ESC to cancel)", NULL);
        if (g_state.file_name == NULL) {
            set_status_message("Save aborted");
            return;
        }
        editor_select_syntax_highlight();
    }

    int len;
    char *buf = editor_rows_to_string(&len);

    int fd = open(g_state.file_name, O_RDWR | O_CREAT, 0644);
    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) == len) {;
                close(fd);
                free(buf);
                g_state.dirty = 0;
                set_status_message("%d bytes written to disk", len);
                return;
            }
        }
        close(fd);
    }

    free(buf);
    set_status_message("Can't save! I/O error: %s", strerror(errno));
}