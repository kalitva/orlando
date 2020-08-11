#include <string.h>

#define DEFAULT_COLOR -1
#define BLUE_TEXT 1


void init_colors()
{
  start_color();

  if (use_default_colors() == OK) {
    init_pair(BLUE_TEXT, COLOR_BLUE, DEFAULT_COLOR);
  } else {
    init_pair(BLUE_TEXT, COLOR_BLUE, COLOR_BLUE);
  }
}

void create_text_area()
{
	text_area = newwin(g_state.screen_rows - 2, g_state.screen_cols, 0, 0);
  keypad(text_area, TRUE);
}

void create_footer()
{
  footer = newwin(2, g_state.screen_cols, g_state.screen_rows - 2, 0);
}

void print_text()
{
  node_t *current;
  line_t *line;

  current = g_state.top_line;

  for (int y = 0; current; y++) {
  	line = current->value;

  	mvwprintw(text_area, y, 0, "%s\n", line->str);

    current = current->next;
  }
}

void print_footer()
{
  char *file_name;
  char cursor_position[23];

  wclear(footer);

  file_name = g_state.file_name ? g_state.file_name : "Noname"; /* move late */
  sprintf(cursor_position,
          "Line: %d Column: %d",
          g_state.top_line_number + g_state.cursor_Y,
          g_state.cursor_X);

  wattron(footer, A_BOLD);
  mvwprintw(footer, 
            0,
            g_state.screen_cols - strlen(file_name) - 1,
            "%s\n", 
            file_name);
  wattroff(footer, A_BOLD);

  wattron(footer, COLOR_PAIR(BLUE_TEXT));
  mvwprintw(footer, 1, 0, "%s", MAIN_MESSAGE);
  mvwprintw(footer,
            1,
            g_state.screen_cols - strlen(cursor_position) - 1,
            "%s",
            cursor_position);
  wattroff(footer, COLOR_PAIR(BLUE_TEXT));
}

void print()
{
  print_text();
  print_footer();

  wrefresh(text_area);
  wrefresh(footer);
  wmove(text_area, g_state.cursor_Y, g_state.cursor_X);
}