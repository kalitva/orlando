#include "defines.h"
#include "data.h"


/* highlighting.c */
void editor_update_syntax(erow*);


void update_row(erow *row)
{
  int tabs = 0;
  int index = 0;

  for (int j = 0; j < row->size; j++) /* count tabs */
    if (row->chars[j] == '\t')
      tabs++;

  /* replace tabs with spaces */
  free(row->render);
  row->render = malloc(row->size + 1 + tabs * (TAB - 1) + 1);

  for (int j = 0; j < row->size; j++) { 

    if (row->chars[j] == '\t') {
      row->render[index++] = ' ';
        while (index % TAB != 0)
          row->render[index++] = ' ';
    } else {
      row->render[index++] = row->chars[j];
    }
  }

  row->render[index] = '\0';
  row->rsize = index;

  editor_update_syntax(row);
}

void editor_row_append_string(erow *row, char *str, size_t len)
{
    row->chars = realloc(row->chars, row->size + len + 1);
    memcpy(&row->chars[row->size], str, len);
    row->size += len;
    row->chars[row->size] = '\0';
    update_row(row);
    E.dirty++;
}

void insert_row(int at, char *str, size_t len)
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
    update_row(&E.row[at]);

    E.num_rows++;
    E.dirty++;
}

void editor_free_row(erow *row)
{
    free(row->render);
    free(row->chars);
    free(row->hl);
}

void del_row(int at)
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

    update_row(row);
    E.dirty++;
}

void insert_char(int ch)
{
  if (E.cy == E.num_rows)
    insert_row(E.num_rows, "", 0);

  editor_row_insert_char(&E.row[E.cy], E.cx, ch);
}

void editor_insert_new_line()
{
    if (E.cx == 0) {
        insert_row(E.cy, "", 0);
    } else {
        erow *row = &E.row[E.cy];
        insert_row(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
        row = &E.row[E.cy];
        row->size = E.cx;
        row->chars[row->size] = '\0';
        update_row(row);
    }
    E.cy++;
    E.cx = 0;
}

void editor_row_del_char(erow *row, int at)
{
    if (at < 0 || at >= row->size)
        return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size -at);
    row->size--;
    update_row(row);
    E.dirty++;
}

void editor_del_char()
{
    if (E.cy == E.num_rows)
        return;

    if (E.cx == 0 && E.cy == 0)
        return;

    erow *row = &E.row[E.cy];
    if (E.cx > 0) {
        editor_row_del_char(row, E.cx - 1);
        E.cx--;
    } else {
        E.cx = E.row[E.cy - 1].size;
        editor_row_append_string(&E.row[E.cy - 1], row->chars, row->size);
        del_row(E.cy);
        E.cy--;
    }
}

int editor_row_cx_to_rx(erow *row, int cx)
{
    int rx = 0;

    int j;
    for (j = 0; j < cx; j++) {
        if (row->chars[j] == '\t')
            rx += (TAB - 1) - (rx % TAB);
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
            cur_rx += (TAB - 1) - (cur_rx % TAB);
        cur_rx++;

        if (cur_rx > rx)
            return cx;
    }

    return cx;
}