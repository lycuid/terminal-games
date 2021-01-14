#include <unistd.h>
#include <math.h>
#include <locale.h>
#include <curses.h>

#include <pong.h>

static int WINDOW_HEIGHT;
static int WINDOW_WIDTH;

void init_window();
void game_loop();

bool handle_keypress(Paddle*, int);
void reset_window(const char*);

int main(void)
{
  init_window();
  game_loop();
  endwin();

  return 0;
}

void init_window()
{
  setlocale(LC_ALL, "");
  initscr();
  // colors.
  start_color();
  // dont echo keys unprogrammatically after press.
  noecho();
  // get a character at a time (whatever that means).
  cbreak();
  // enabling arrow keys.
  keypad(stdscr, true);
  // invisible cursor.
  curs_set(0);
  // set WINDOW_HEIGHT and WINDOW_WIDTH.
  getmaxyx(stdscr, WINDOW_HEIGHT, WINDOW_WIDTH);
}

void reset_window(const char* title)
{
  clear();
  box(stdscr, 0, 0);
  mvprintw(0, 2, title);
  refresh();
}


void game_loop()
{
  nodelay(stdscr, true);
  reset_window("| EPIC PONG GAME |");

  Paddle paddle (1, WINDOW_HEIGHT - 1);

  while (handle_keypress(&paddle, getch())) {
    paddle.render();
    mvvline(1, WINDOW_WIDTH / 2, '|', WINDOW_HEIGHT - 2);
    usleep(1 * pow(10, 4));
  }
}

bool handle_keypress(Paddle* paddle, int ch)
{
  switch (ch) {
    case 'q': return false;

    case 'w':
    case KEY_UP:
      paddle->direction = Backward;
      paddle->step();
      break;

    case 's':
    case KEY_DOWN:
      paddle->direction = Forward;
      paddle->step();
      break;

    case ERR:
    default: break;
  }
  return true;
}

