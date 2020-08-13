#include <unistd.h>

/* cursor.c */
void cursor_to_start_line(void);
void cursor_to_right(void);
/* lnklist.c */
void head_to_next(list_t *);
void head_to_previous(list_t *);
/* outptu.c */
void print(void);


void top_line_to_up()
{
	if (!g_state.top_line->previous) {
		return;
  }

	g_state.top_line = g_state.top_line->previous;
	g_state.top_line_number--;
	g_state.cursor_Y++;
}

void top_line_to_down()
{
  if (!g_state.top_line->next) {
    return;
  }

	g_state.top_line = g_state.top_line->next;
	g_state.top_line_number++;
	g_state.cursor_Y--;
}

int tab_align()
{
  int space = 8 - g_state.cursor_X % 8;
  return space == 0 ? 8 : space;
}

void move_through_tabs(int position)
{
  cursor_to_start_line();
  while (position-- > 0) {
    cursor_to_right();
  }
}

void cursor_to_up()
{
  line_t *line;

  if (!g_lines->head->previous) {
    return;
  }

  head_to_previous(g_lines);  
  line = g_lines->head->value;
  g_state.cursor_Y--;

  if (g_state.pos_X > line->len) {
    move_through_tabs(line->len);
  } else {
    move_through_tabs(g_state.pos_X);
  }

  if (g_state.cursor_Y < 0) {
  	top_line_to_up();
  }
}

void cursor_to_down()
{
  line_t *line;

  if (!g_lines->head->next) {
    wclear(text_area);
    top_line_to_down();
    return;
  }

  head_to_next(g_lines);
  line = g_lines->head->value;
  g_state.cursor_Y++;

  if (g_state.pos_X > line->len) {
    move_through_tabs(line->len);
  } else {
    move_through_tabs(g_state.pos_X);
  }

  if (g_state.cursor_Y >= g_state.screen_rows - 2) {
  	top_line_to_down();
  }
}

void cursor_to_right()
{
  line_t *line = g_lines->head->value;

  if (g_state.pos_X < line->len) {

    if (line->str[g_state.pos_X] == '\t') { /* case for tab character */
      g_state.cursor_X += tab_align();
    } else {
      g_state.cursor_X++;
    }
    g_state.pos_X++;

  } else if (g_lines->head != g_lines->last) {

    cursor_to_down();
    g_state.cursor_X = g_state.pos_X = (g_state.cursor_Y != 0) 
                                     ? 0 
                                     : g_state.cursor_X;
  }
}

void cursor_to_left()
{
  line_t *line;

  if (g_state.pos_X > 0) {

    line = g_lines->head->value;
    if (line->str[g_state.pos_X - 1] == '\t') {  /* case for tab character */
      move_through_tabs(g_state.pos_X - 1);
    } else {
      g_state.cursor_X--;
      g_state.pos_X--;
    }

  } else if (g_state.cursor_Y != 0) {

    cursor_to_up();
    line = g_lines->head->value;
    move_through_tabs(line->len);
  }
}

void page_up()
{
  for (int i = 2; i < g_state.screen_rows; i++) {
    top_line_to_up();
    cursor_to_up();
    usleep(5000);
    print();
  }
}

void page_down()
{
  for (int i = 2; i <= g_state.screen_rows; i++) {
    top_line_to_down();
    cursor_to_down();
    wclear(text_area);
    usleep(5000);
    print();
  }
}

void cursor_to_start_line()
{
  g_state.pos_X = 0;
	g_state.cursor_X = 0;
}

void cursor_to_end_line()
{
	line_t *line = g_lines->head->value;
  move_through_tabs(line->len);
}

void cursor_to_start()
{
  int delay = g_lines->size < 200 ? 1000 
            : g_lines->size < 1000 ? 300 : 10;

	while (g_lines->head->previous) {
		cursor_to_up();
    usleep(delay);
    print();
  }

	g_state.cursor_X = 0;
	g_state.cursor_Y = 0;
	g_lines->head = g_lines->first;
}

void cursor_to_end()
{
	line_t *line = g_lines->last->value;
  int delay = g_lines->size < 200 ? 1000 
            : g_lines->size < 1000 ? 300 : 10;
            
	while (g_lines->head->next) {
		cursor_to_down();
    usleep(delay);
    print();
  }

  cursor_to_end_line();
	g_lines->head = g_lines->last;
}
