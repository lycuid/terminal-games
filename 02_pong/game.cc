#include <unistd.h>
#include <math.h>
#include <locale.h>
#include <curses.h>
#include <time.h>
#include <pong.h>

#define FRAME_RATE  120
#define TIME_SECOND pow(10, 6)

static int WINDOW_HEIGHT;
static int WINDOW_WIDTH;

void init_window();
void game_loop();

bool handle_keypress(Paddle*, int);
void reset_window(const char*);

int rand_range(int, int);

int main(void)
{
  srand(time(0));

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

  Paddle player (Coordinates(1, PADDLE_START_Y), 1, WINDOW_HEIGHT - 1);
  Paddle computer (Coordinates(WINDOW_WIDTH - 2, PADDLE_START_Y), 1, WINDOW_HEIGHT - 1);

  Coordinates ball_coords = Coordinates(2,
      rand_range(PADDLE_START_Y, PADDLE_START_Y + PADDLE_HEIGHT - 1));
  Ball ball (ball_coords, 1, WINDOW_HEIGHT - 1);

  while (handle_keypress(&player, getch())) {
    // divider line.
    mvvline(1, WINDOW_WIDTH / 2, '|', WINDOW_HEIGHT - 2);

    computer.render();
    player.render();

    ball.step();
    ball.render();

    if (ball.wall_collision(&player, &computer))
      break;

    usleep(TIME_SECOND / FRAME_RATE);
  }
}

bool handle_keypress(Paddle* player, int ch)
{
  switch (ch) {
    case 'q': return false;

    case 'a':
    case KEY_LEFT:
    case 'w':
    case KEY_UP:
      player->direction = Backward;
      player->step();
      break;

    case 'd':
    case KEY_RIGHT:
    case 's':
    case KEY_DOWN:
      player->direction = Forward;
      player->step();
      break;

    case ERR:
    default: break;
  }
  return true;
}

int rand_range(int min, int max)
{
  return (rand() % (min + max)) + min;
}

