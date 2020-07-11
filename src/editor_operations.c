#include "defines.h"
#include "data.h"


void editor_update_row(erow*);
/* row_operations.c */
void insert_row(int, char*,size_t);
void editor_del_row(int);
void editor_row_insert_char(erow*, int, int);
void editor_row_append_string(erow*, char*, size_t);
void editor_row_del_char(erow*, int);


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
        editor_update_row(row);
    }
    E.cy++;
    E.cx = 0;
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
        editor_del_row(E.cy);
        E.cy--;
    }
}
