#include <clocale>
#include <cmath>
#include <cstring>
#include <ctime>
#include <curses.h>
#include <snake.h>
#include <unistd.h>

#include "configs.h"

void init_window();
void game_loop();
bool scoreboard_and_restart(WINDOW *);
void reset_window(WINDOW *);
void update_window(WINDOW *, Point *);
bool handle_keypress(Snake *, int);
int  rand_range(int, int);

static int SCOREBOARD_HEIGHT;
static int SCOREBOARD_WIDTH;
static int SCOREBOARD_Y;
static int SCOREBOARD_X;

static const char *SCORE_BAR_FMT = "( Score: %03d )";
static const char *DEATH_BAR_FMT = "( Death: %03d )";

int main(void)
{
  srand(time(0));
  init_window();

  WINDOW *scoreboard =
      newwin(SCOREBOARD_HEIGHT, SCOREBOARD_WIDTH, SCOREBOARD_Y, SCOREBOARD_X);
  while (1) {
    game_loop();
    if (!scoreboard_and_restart(scoreboard)) break;
  }

  delwin(scoreboard);
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
  refresh();

  SCOREBOARD_HEIGHT = 7;
  SCOREBOARD_WIDTH  = strlen(SCOREBOARD_ECHO) + 2;
  SCOREBOARD_X      = (WINDOW_WIDTH - SCOREBOARD_WIDTH) / 2;
  SCOREBOARD_Y      = (WINDOW_HEIGHT - SCOREBOARD_HEIGHT) / 2;
}

void game_loop()
{
  WINDOW *canvas = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);

  // non blocking 'getch'.
  nodelay(stdscr, true);

  PLAYER_DEATHS = 0;
  PLAYER_SCORE  = 0;

  Snake snake;
  snake.init(PLAYER_SIZE);
  reset_window(canvas);

  Point fruit = Point();
  update_window(canvas, &fruit);

  while (handle_keypress(&snake, getch())) {
    snake.step();
    snake.render(canvas);

    if (snake.ate_fruit(&fruit)) {
      PLAYER_SCORE += 10;
      update_window(canvas, &fruit);
    }

    if (snake.is_dead(WINDOW_HEIGHT - 1, WINDOW_WIDTH - 1)) {
      PLAYER_DEATHS++;

      snake = Snake();
      snake.init(PLAYER_SIZE);
      reset_window(canvas);
      update_window(canvas, &fruit);

      continue;
    }

    // updates screen.
    wrefresh(canvas);
    usleep((unsigned)(5 * pow(10, 4)));
  }

  delwin(canvas);
}

bool scoreboard_and_restart(WINDOW *scoreboard)
{
  // blocking 'getch'.
  nodelay(stdscr, false);

  wattroff(scoreboard, A_STANDOUT);
  wclear(scoreboard);
  box(scoreboard, 0, 0);

  mvwaddstr(scoreboard, 0, (SCOREBOARD_WIDTH - 14) / 2, SCOREBOARD_TITLE);
  mvwprintw(scoreboard, 2, 4, "Score: %03d", PLAYER_SCORE);
  mvwprintw(scoreboard, 3, 4, "Death: %03d", PLAYER_DEATHS);
  wattron(scoreboard, A_STANDOUT);
  mvwaddstr(scoreboard, 5, 1, SCOREBOARD_ECHO);
  wrefresh(scoreboard);

  int ch;
  while ((ch = getch()) != ERR) {
    switch (ch) {
      case 'r': return true;
      case 'q': return false;
      default: break;
    }
  }

  // if somehow while exits with error, then exit game.
  return false;
}

bool handle_keypress(Snake *snake, int ch)
{
  switch (ch) {
    case 'q': return false;

    case 'w':
    case KEY_UP: snake->direction = Up; break;
    case 'a':
    case KEY_LEFT: snake->direction = Left; break;
    case 's':
    case KEY_DOWN: snake->direction = Down; break;
    case 'd':
    case KEY_RIGHT: snake->direction = Right; break;
    case ERR:
    default: break;
  }

  return true;
}

// refresh fruit and scoreboard.
void update_window(WINDOW *canvas, Point *fruit)
{
  // refreshing the fruit.
  fruit->y = rand_range(1, WINDOW_HEIGHT - 1);
  fruit->x = rand_range(1, WINDOW_WIDTH - 1);

  mvwaddch(canvas, fruit->y, fruit->x, FRUIT_SYM);
  mvprintw(0, WINDOW_WIDTH - strlen(DEATH_BAR_FMT) - strlen(SCORE_BAR_FMT) - 2,
           SCORE_BAR_FMT, PLAYER_SCORE);
}

void reset_window(WINDOW *canvas)
{
  wclear(canvas);
  box(canvas, 0, 0);
  mvwprintw(canvas, 0, 2, WINDOW_TITLE);
  mvwprintw(canvas, 0, WINDOW_WIDTH - strlen(DEATH_BAR_FMT) - 2, DEATH_BAR_FMT,
            PLAYER_DEATHS);
  wrefresh(canvas);
}

int rand_range(int min, int max) { return (rand() % (max - min)) + min; }
