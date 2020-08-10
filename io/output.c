void create_text_area()
{
	text_area = newwin(g_state.screen_rows - 2, g_state.screen_cols, 0, 0);
  keypad(text_area, TRUE);
}

void print()
{
  node_t *current;
  line_t *line;

  current = g_state.top_line;

  for (int y = 0; y < g_state.screen_rows -2 && current; y++) {
  	line = current->value;

  	mvwprintw(text_area, y, 0, "%s\n", line->str);

    current = current->next;
  }

  wmove(text_area, g_state.cursor_Y, g_state.cursor_X);
  wrefresh(text_area);
}