#include "defines.h"


struct termios orig_termios;
struct termios raw_mode;


void die(const char *s)
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

void enable_screen()
{
  tcgetattr(STDIN_FILENO, &orig_termios);

  struct termios raw_mode = orig_termios;
  
  raw_mode.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw_mode.c_oflag &= ~(OPOST);
  raw_mode.c_cflag &= ~(CS8);
  raw_mode.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw_mode.c_cc[VMIN] = 0;
  raw_mode.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_mode);
}

void disable_screen()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

int read_key()
{
  char ch;
  while(!read(STDIN_FILENO, &ch, 1));

  if (ch == '\x1b') {
    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) != 1)
      return '\x1b';
    if(read(STDIN_FILENO, &seq[1], 1) != 1)
      return '\x1b';

    if (seq[0] == '[') {

      if (seq[1] >= '0' && seq[1] <= '9') {

        if (read(STDIN_FILENO, &seq[2], 1) != 1)
          return '\x1b';

        if (seq[2] == '~') {

          switch (seq[1]) {
            case '1':
              return HOME_KEY;
            case '3':
              return DEL_KEY;
            case '4':
              return END_KEY;
            case '5':
              return PAGE_UP;
            case '6':
              return PAGE_DOWN;
            case '7':
              return HOME_KEY;
            case '8':
              return END_KEY;
          }
        }

      } else {

        switch (seq[1]) {
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

      return '\x1b';

    } else {
      
        return ch;
    }
}

void get_window_size(int *rows, int *cols)
{
  struct winsize params;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &params); /* get window parameters */

  *cols = params.ws_col; /* and save them in the global state */
  *rows = (params.ws_row - 2);
}