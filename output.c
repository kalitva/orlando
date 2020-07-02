#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "data.h"


int editor_row_cx_to_rx(erow*, int);
int editor_syntax_to_color(int);


                                /*** char buffer ***/


void buf_append(struct char_buffer *buffer, const char *s, int len)
{
    char *new = realloc(buffer->str, buffer->len + len);

    if (new == NULL)
        return;

    memcpy(&new[buffer->len], s, len);
    buffer->str = new;
    buffer->len += len;
}

void buf_free(struct char_buffer *buffer) {
    free(buffer->str);
}
                                /*** ***/

void editor_scroll()
{
    E.rx = 0;
    if (E.cy < E.num_rows)
        E.rx = editor_row_cx_to_rx(&E.row[E.cy], E.cx);

    if (E.cy < E.row_offset)
        E.row_offset = E.cy;

    if (E.cy >= E.row_offset + E.screen_rows)
        E.row_offset = E.cy - E.screen_rows + 1;

    if (E.rx < E.col_offset)
        E.col_offset = E.rx;

    if (E.rx >= E.col_offset + E.screen_cols)
        E.col_offset = E.rx - E.screen_cols + 1;
}

void editor_draw_rows(struct char_buffer *buffer)
{
    int y;
    for (y = 0; y < E.screen_rows; y++) {
        int file_row = y + E.row_offset;

        if (file_row >= E.num_rows) {

            if (E.num_rows == 0 && y == E.screen_rows / 3) {
                char welcome[80];
                int welcome_len = snprintf(welcome,
                                           sizeof(welcome),
                                           "Kilo -- version %s", KILO_VERSION);
                if (welcome_len > E.screen_cols)
                    welcome_len = E.screen_cols;

                int padding = (E.screen_cols - welcome_len) / 2;
                if (padding) {
                    buf_append(buffer, "~", 1);
                    padding--;
                }
                while (padding--)
                    buf_append(buffer, " ", 1);
                buf_append(buffer, welcome, welcome_len);
            } else {
                buf_append(buffer, "~", 1);
            }

        } else {
            int len = E.row[file_row].rsize - E.col_offset;
            if (len < 0)
                len = 0;
            if (len > E.screen_cols)
                len = E.screen_cols;
            char *ch = &E.row[file_row].render[E.col_offset];
            unsigned char *hl = &E.row[file_row].hl[E.col_offset];
            int current_color = -1;
            int j;
            for (j = 0; j < len; j++) {
                if (iscntrl(ch[j])) {
                    char sym = (ch[j] <= 26) ? '@' + ch[j] : '?';
                    buf_append(buffer, "\x1b[7m", 4);
                    buf_append(buffer, &sym, 1);
                    buf_append(buffer, "\x1b[m", 3);
                    if (current_color != -1) {
                        char buf[16];
                        int clen = snprintf(buf,
                                            sizeof(buf),
                                            "\x1b[%dm",
                                            current_color);
                        buf_append(buffer, buf, clen);
                    }
                } else if (hl[j] == HL_NORMAL) {
                    if (current_color != -1) {
                        buf_append(buffer, "\x1b[39m", 5);
                        current_color = -1;
                    }
                    buf_append(buffer, &ch[j], 1);
                } else {
                    int color = editor_syntax_to_color(hl[j]);
                    if (color != current_color) {
                        current_color = color;
                        char buf[16];
                        int clen = snprintf(buf, sizeof(buf),
                                            "\x1b[%dm",
                                            color);
                    buf_append(buffer, buf, clen);
                    }
                    buf_append(buffer, &ch[j], 1);
                }
            }
            buf_append(buffer, "\x1b[39m", 5);
        }

        buf_append(buffer, "\x1b[K", 3);
        buf_append(buffer, "\r\n", 2);
    }
}

void editor_draw_status_bar(struct char_buffer *buffer)
{
    buf_append(buffer, "\x1b[7m", 4);

    char status[80];
    char rstatus[80];

    int len = snprintf(status,
                       sizeof(status),
                       "%.20s - %d lines %s",
                       E.file_name ? E.file_name : "[No Name]",
                       E.num_rows,
                       E.dirty ? "(modified)" : "");
    int rlen = snprintf(rstatus,
                        sizeof(rstatus),
                        "%s | %d/%d",
                        E.syntax ? E.syntax->file_type : "no ft",
                        E.cy + 1,
                        E.num_rows);

    if (len > E.screen_cols)
        len = E.screen_cols;

    buf_append(buffer, status, len);

    while (len < E.screen_cols) {
        if (E.screen_rows - len == rlen) {
            buf_append(buffer, status, rlen);
            break;
        } else {
            buf_append(buffer, " ", 1);
            len++;
        }
    }

    buf_append(buffer, "\x1b[m", 3);
    buf_append(buffer, "\r\n", 2);
}

void editor_draw_message_bar(struct char_buffer *buffer)
{
    buf_append(buffer, "\x1b[K", 3);
    int msg_len = strlen(E.status_msg);
    if (msg_len > E.screen_cols)
        msg_len = E.screen_cols;
    if (msg_len && time(NULL) - E.status_msg_time < 5)
        buf_append(buffer, E.status_msg, msg_len);
}

void editor_refresh_screen()
{
    editor_scroll();

    struct char_buffer buffer = BUF_INIT;

    buf_append(&buffer, "\x1b[?25l", 6);
    buf_append(&buffer, "\x1b[H", 3);

    editor_draw_rows(&buffer);
    editor_draw_status_bar(&buffer);
    editor_draw_message_bar(&buffer);

    char buf[32];
    snprintf(buf,
             sizeof(buf),
             "\x1b[%d;%dH",
             (E.cy - E.row_offset) + 1,
             (E.rx - E.col_offset) + 1);
    buf_append(&buffer, buf, strlen(buf));

    buf_append(&buffer, "\x1b[?25h", 6);

    write(STDOUT_FILENO, buffer.str, buffer.len);
    buf_free(&buffer);
}

void editor_set_status_message(const char *ftime, ...)
{
    va_list ap;
    va_start(ap, ftime);
    vsnprintf(E.status_msg, sizeof(E.status_msg), ftime, ap);
    va_end(ap);
    E.status_msg_time = time(NULL);
}
