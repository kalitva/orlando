#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)


/* main.c */
void quit(void);
/* editor.c */
void delete_char(void);
void insert_line(void);
void delete_line(void);
/* cursor.c */
void cursor_to_up(void);
void cursor_to_down(void);
void cursor_to_left(void);
void cursor_to_right(void);
void page_up(void);
void page_down(void);
void cursor_to_start_line(void);
void cursor_to_end_line(void);
void cursor_to_start(void);
void cursor_to_end(void);
/* file_io.c */
void save_file(void);
/* output.c */
void print(void);
void resize_screen(void);
/* syntax.c */
void insert(int);
void insert_tab(void);
void indentation(void);


void process_keypress()
{
  int ch = wgetch(text_area);

  switch (ch) {

    case 10: // KEY_ENTER
      insert_line();
      cursor_to_down();
      break;

    case '\t':
      indentation();
      break;

    case CTRL_KEY('s'):
      save_file();
      break;

    case KEY_HOME:
    	cursor_to_start_line();
      break;

    case KEY_END:
    	cursor_to_end_line();
      break;

    case 535: //CTRL_HOME_KEY:
    	cursor_to_start();
    	break;

    case 530: //CTRL_END_KEY:
    	cursor_to_end();
    	break;	

    case CTRL_KEY('d'):
      delete_line();
      wclear(text_area);
      break;

    case KEY_BACKSPACE:
      cursor_to_left();
      delete_char();
      break;

    case 330: //KEY_DEL:
      delete_char();
      break;

    case KEY_PPAGE:
    	page_up();
    	break;

    case KEY_NPAGE:
      page_down();
      wclear(text_area);
      break;

    case KEY_UP:
      cursor_to_up();
      break;

    case KEY_DOWN:
      cursor_to_down();
      break;

    case KEY_LEFT:
      cursor_to_left();
      break;

    case KEY_RIGHT:
      cursor_to_right();
      break;

    case 410: // Resize of window
      resize_screen();
      break;

    case CTRL_KEY('q'):
      quit();

    default:
    	insert(ch);
  }

  print();
}