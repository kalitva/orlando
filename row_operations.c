#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "data.h"


void editor_update_syntax(erow*);


int editor_row_cx_to_rx(erow *row, int cx)
{
    int rx = 0;

    int j;
    for (j = 0; j < cx; j++) {
        if (row->chars[j] == '\t')
            rx += (KILO_TAB_STOP - 1) - (rx % KILO_TAB_STOP);
        rx++;
    }

    return rx;
}

int editor_row_rx_to_cx(erow *row, int rx)
{
    int cur_rx = 0;
    int cx;
    for (cx = 0; cx < row->size; cx++) {

        if (row->chars[cx] == '\t')
            cur_rx += (KILO_TAB_STOP - 1) - (cur_rx % KILO_TAB_STOP);
        cur_rx++;

        if (cur_rx > rx)
            return cx;
    }

    return cx;
}

void editor_update_row(erow *row)
{
    int tabs = 0;
    int j;
    for (j = 0; j < row->size; j++) {

        if (row->chars[j] == '\t')
            tabs++;
    }

    free(row->render);
    row->render = malloc(row->size + 1 + tabs * (KILO_TAB_STOP - 1) + 1);

    int index = 0;
    for (j = 0; j < row->size; j++) {
        if (row->chars[j] == '\t') {
            row->render[index++] = ' ';
            while (index % KILO_TAB_STOP != 0)
                row->render[index++] = ' ';
        } else {
            row->render[index++] = row->chars[j];
        }
    }

    row->render[index] = '\0';
    row->rsize = index;

    editor_update_syntax(row);
}

void editor_insert_row(int at, char *str, size_t len)
{
    if (at < 0 || at > E.num_rows)
        return;

    E.row = realloc(E.row, sizeof(erow) * (E.num_rows + 1));
    memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.num_rows - at));

    for (int j = at + 1; j < E.num_rows; j++)
        E.row[j].idx++;

    E.row[at].idx = at;

    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, str, len);
    E.row[at].chars[len] = '\0';

    E.row[at].rsize = 0;
    E.row[at].render = NULL;
    E.row[at].hl = NULL;
    E.row[at].hl_open_comment = 0;
    editor_update_row(&E.row[at]);

    E.num_rows++;
    E.dirty++;
}

void editor_free_row(erow *row)
{
    free(row->render);
    free(row->chars);
    free(row->hl);
}

void editor_del_row(int at)
{
    if (at < 0 || at >= E.num_rows)
        return;
    editor_free_row(&E.row[at]);
    memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.num_rows - at - 1));
    for (int j = at; j < E.num_rows - 1; j++)
        E.row[j].idx--;
    E.num_rows--;
    E.dirty++;
}

void editor_row_insert_char(erow *row, int at, int ch)
{
    if (at < 0 || at > row->size)
       at = row->size;

    row->chars = realloc(row->chars, row->size + 2);
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->size++;
    row->chars[at] = ch;
    editor_update_row(row);
    E.dirty++;
}

void editor_row_append_string(erow *row, char *s, size_t len)
{
    row->chars = realloc(row->chars, row->size + len + 1);
    memcpy(&row->chars[row->size], s, len);
    row->size += len;
    row->chars[row->size] = '\0';
    editor_update_row(row);
    E.dirty++;
}

void editor_row_del_char(erow *row, int at)
{
    if (at < 0 || at >= row->size)
        return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size -at);
    row->size--;
    editor_update_row(row);
    E.dirty++;
}