#include "defines.h"
#include "data.h"


/* input.c */
char *editor_prompt(char *prompt, void (*callback)(char *, int));
/* highlighting.c */
void editor_select_syntax_highlight();
/* editor.c */
void insert_line(t_node *node);
/* output.c */
void set_status_message(const char*, ...);


char *editor_rows_to_string(int *buf_len)
{
}

void open_file(char *file_name)
{
  g_state.file_name = strdup(file_name); /* write file-name to global state */

  editor_select_syntax_highlight();

  FILE *fp = fopen(file_name, "r"); /* open file */

  if (!fp)
    fp = fopen(file_name, "w"); /* create file, if don't exit */
  
  /* print content */
  char* line = NULL;
  size_t line_cap;
  ssize_t line_len;

  while ((line_len = getline(&line, &line_cap, fp)) != -1) {

    while ((line_len--) > 0
        && (line[line_len - 1] == '\n' || line[line_len - 1] == '\r'));

//    insert_row(g_state.num_rows, line, line_len);
  }

  free(line); /* close stream */
  fclose(fp);
  g_state.dirty = false;
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
