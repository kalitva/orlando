#include <unistd.h>


/* lnklist.c */
void head_to_next(t_list *);
void head_to_previous(t_list *);
/* outptu.c */
void refresh_screen(void);


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

void cursor_to_up()
{
  if (!g_lines->head->previous) {
    return;
  }

  t_line *line;

  head_to_previous(g_lines);  
  line = g_lines->head->value;
  g_state.cursor_Y--;
  g_state.cursor_X = g_state.cursor_X > line->len
                   ? line->len
                   : g_state.cursor_X;

  if (g_state.cursor_Y < 0) {
  	top_line_to_up();
  }
}

void cursor_to_down()
{
  if (!g_lines->head->next) {
    return;
  }

  t_line *line;

  head_to_next(g_lines);
  line = g_lines->head->value;
  g_state.cursor_Y++;
  g_state.cursor_X = g_state.cursor_X > line->len
                   ? line->len
                   : g_state.cursor_X;

  if (g_state.cursor_Y > g_state.screen_rows - 1) {
  	top_line_to_down();
  }
}

void cursor_to_left()
{
  t_line *line;

  if (g_state.cursor_X > 0) {
    g_state.cursor_X--;
  } else if (g_state.cursor_Y != 0) {
    cursor_to_up();
    line = g_lines->head->value;
    g_state.cursor_X = line->len;
  }
}

void cursor_to_right()
{
  t_line *line = g_lines->head->value;

  if (g_state.cursor_X < line->len) {
    g_state.cursor_X++;
  } else if (g_lines->head != g_lines->last) {
    cursor_to_down();
    g_state.cursor_X = (g_state.cursor_Y != 0) ? 0 : g_state.cursor_X;
  }
}

void page_up()
{
  for (int i = 2; i < g_state.screen_rows; i++) {
    top_line_to_up();
    cursor_to_up();
    usleep(5000);
    refresh_screen();
  }
}

void page_down()
{

  for (int i = 2; i < g_state.screen_rows; i++) {
    top_line_to_down();
    cursor_to_down();
    usleep(5000);
    refresh_screen();
  }
}

void cursor_to_start_line()
{
	g_state.cursor_X = 0;
}

void cursor_to_end_line()
{
	t_line *line = g_lines->head->value;

	g_state.cursor_X = line->len;
}

void cursor_to_start()
{
  int delay = g_lines->size < 200 ? 5000 
            : g_lines->size < 1000 ? 1000 : 10;

	while (g_lines->head->previous) {
		cursor_to_up();
    usleep(delay);
    refresh_screen();
  }

	g_state.cursor_X = 0;
	g_state.cursor_Y = 0;
	g_lines->head = g_lines->first;
}

void cursor_to_end()
{
	t_line *line = g_lines->last->value;
  int delay = g_lines->size < 200 ? 5000 
            : g_lines->size < 1000 ? 1000 : 10;
            
	while (g_lines->head->next) {
		cursor_to_down();
    usleep(delay);
    refresh_screen();
  }

	g_state.cursor_X = line->len;
	g_lines->head = g_lines->last;
}