#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "data.h"

char *editor_prompt(char *prompt, void (*callback)(char *, int));
void die(char*);
void editor_select_syntax_highlight();
void editor_insert_row(int, char*,size_t);
void editor_set_status_message(const char*, ...);


char *editor_rows_to_string(int *buf_len)
{
    int total_len = 0;
    int j;

    for (j = 0; j < E.num_rows; j++)
        total_len += E.row[j].size + 1;

    *buf_len = total_len;

    char *buf = malloc(total_len);
    char *p = buf;

    for (j = 0; j < E.num_rows; j++) {
        memcpy(p, E.row[j].chars, E.row[j].size);
        p += E.row[j].size;
        *p = '\n';
        p++;
    }

    return buf;
}

void editor_open(char *file_name)
{
    free(E.file_name);
    E.file_name = strdup(file_name);

    editor_select_syntax_highlight();

    FILE *fp = fopen(file_name, "r");
    if (!fp)
        die("fopen");

    char* line = NULL;
    size_t line_cap = 0;
    ssize_t line_len;

    while ((line_len = getline(&line, &line_cap, fp)) != -1) {

        while (line_len > 0
                 && (line[line_len - 1] == '\n'
                    || line[line_len - 1] == '\r'))
            line_len--;

        editor_insert_row(E.num_rows, line, line_len);
    }

    free(line);
    fclose(fp);
    E.dirty = 0;
}

void editor_save()
{
    if (E.file_name == NULL) {
        E.file_name = editor_prompt("Save as: %s" "(ESC to cancel)", NULL);
        if (E.file_name == NULL) {
            editor_set_status_message("Save aborted");
            return;
        }
        editor_select_syntax_highlight();
    }

    int len;
    char *buf = editor_rows_to_string(&len);

    int fd = open(E.file_name, O_RDWR | O_CREAT, 0644);
    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) == len) {;
                close(fd);
                free(buf);
                E.dirty = 0;
                editor_set_status_message("%d bytes written to disk", len);
                return;
            }
        }
        close(fd);
    }

    free(buf);
    editor_set_status_message("Can't save! I/O error: %s", strerror(errno));
}
