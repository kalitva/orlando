#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <ncurses.h>


/*output.c */
void refresh_screen(void);


void get_window_size(int *rows, int *cols)
{
  struct winsize params;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &params); /* get window parameters */

  *cols = params.ws_col; /* and save them in the global state */
  *rows = (params.ws_row - 2);
}

void quit()
{
  endwin();
  exit(0);
}