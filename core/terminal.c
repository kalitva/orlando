#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>


/*output.c */
void refresh_screen(void);


struct termios orig_termios;
struct termios raw_mode;


void enable_screen()
{
  tcgetattr(STDIN_FILENO, &orig_termios);

  struct termios raw_mode = orig_termios;
  
  raw_mode.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw_mode.c_oflag &= ~(OPOST);
  raw_mode.c_cflag &= ~(CS8);
  raw_mode.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw_mode.c_cc[VMIN] = 0;
  raw_mode.c_cc[VTIME] = 5;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_mode);
}

void disable_screen()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void get_window_size(int *rows, int *cols)
{
  struct winsize params;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &params); /* get window parameters */

  *cols = params.ws_col; /* and save them in the global state */
  *rows = (params.ws_row - 2);
}

int handle_escape_sequence()
{
  char sequence[5];

  if (!read(STDIN_FILENO, &sequence[0], 1)
      || !read(STDIN_FILENO, &sequence[1], 1)) {
    return '\x1b';
  }

  if (sequence[1] >= '0' && sequence[1] <= '9') {

    if (!read(STDIN_FILENO, &sequence[2], 1)) {
      return '\x1b';
    }

    if (sequence[2] == '~') {

      switch (sequence[1]) {
        case '3':
          return DEL_KEY;
        case '5':
          return PAGE_UP;
        case '6':
          return PAGE_DOWN;
      }

    } else if (sequence[2] == ';') {

      read(STDIN_FILENO, &sequence[3], 2);

      switch (sequence[4]) {
        case 'H':
          return CTRL_HOME_KEY;
        case 'F': 
          return CTRL_END_KEY;
      }
    }

  } else if (sequence[1] == 'M') {
/*
    read(STDIN_FILENO, &sequence[2], 3);

    switch (sequence[2]) {
      case '`':
        return ARROW_UP;
      case 'a':
        return ARROW_DOWN;
      default:
        return '\x1b';
    }
*/
  } else {

    switch (sequence[1]) {
      case 'A':
        return ARROW_UP;
      case 'B':
        return ARROW_DOWN;
      case 'C':
        return ARROW_RIGHT;
      case 'D':
        return ARROW_LEFT;
      case 'H':
        return HOME_KEY;
      case 'F':
        return END_KEY;
    }
  }

}

void quit()
{
  disable_screen();

  write(STDOUT_FILENO, "\x1b[H", 3);
  write(STDOUT_FILENO, "\x1b[2J", 4);

  exit(0);
}