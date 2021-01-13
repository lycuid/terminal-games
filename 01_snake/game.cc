#include <unistd.h> // usleep().
#include <string.h>
#include <time.h>
#include <locale.h>
#include <math.h>
#include <curses.h>

#include "snake.h"

static const  char* WINDOW_TITLE = "( EPIC SNAKE GAME )";
static        int   WINDOW_HEIGHT;
static        int   WINDOW_WIDTH;

static const  char* SCOREBOARD_TITLE  = "( SCOREBOARD )";
static const  char* SCOREBOARD_ECHO   = "  press 'q' to quit or 'r' to restart...  ";
static        int SCOREBOARD_HEIGHT;
static        int SCOREBOARD_WIDTH;
static        int SCOREBOARD_Y;
static        int SCOREBOARD_X;

static int PLAYER_DEATHS  = 0;
static int PLAYER_SCORE   = 0;
static int PLAYER_SIZE    = 5;

static wchar_t FRUIT_SYM[]        = { 0x2588, 0 };
static const char* DEATH_BAR_FMT  = "( Death: %03d )";
static const char* SCORE_BAR_FMT  = "( Score: %03d )";

static int PADDING_DEATH_BAR = strlen(DEATH_BAR_FMT) + 2;
static int PADDING_SCORE_BAR = strlen(SCORE_BAR_FMT) + 2;

void init_window(); // initialize ncurses window.
void reset_window(const char*);
void game_loop();
bool scoreboard_and_restart(WINDOW*);

void update_extras(Snake*, Coordinates*);
bool handle_keypress(Snake*, int);
int rand_range(int, int);

int main(void)
{
  // setting up seed for generating pseudo-random value.
  srand(time(0));
  init_window();

  WINDOW* scoreboard = newwin(SCOREBOARD_HEIGHT, SCOREBOARD_WIDTH, SCOREBOARD_Y, SCOREBOARD_X);
  for (;;) {
    game_loop();
    if (!scoreboard_and_restart(scoreboard))
      break;
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
  // set WINDOW_HEIGHT and WINDOW_WIDTH.
  getmaxyx(stdscr, WINDOW_HEIGHT, WINDOW_WIDTH);

  SCOREBOARD_HEIGHT = 7;
  SCOREBOARD_WIDTH  = strlen(SCOREBOARD_ECHO) + 2;
  SCOREBOARD_X      = (WINDOW_WIDTH - SCOREBOARD_WIDTH) / 2;
  SCOREBOARD_Y      = (WINDOW_HEIGHT - SCOREBOARD_HEIGHT) / 2;
}

void reset_window(const char* title)
{
  clear();
  box(stdscr, 0, 0);
  mvprintw(0, 2, title);
  mvprintw(0, WINDOW_WIDTH-PADDING_DEATH_BAR, DEATH_BAR_FMT, PLAYER_DEATHS);
  refresh();
}

int rand_range(int min, int max) { return (rand() % (max - min)) + min; }

void game_loop()
{
  // non blocking 'getch'.
  nodelay(stdscr, true);

  PLAYER_DEATHS  = 0;
  PLAYER_SCORE   = 0;

  Snake snake;
  snake.init(PLAYER_SIZE);
  reset_window(WINDOW_TITLE);

  Coordinates fruit = Coordinates();
  update_extras(&snake, &fruit);

  while (handle_keypress(&snake, getch())) {
    snake.step();
    snake.draw();

    if (snake.eat_fruit(&fruit)) {
      PLAYER_SCORE+=10;
      update_extras(&snake, &fruit);
    }

    if (snake.is_dead(WINDOW_HEIGHT - 1, WINDOW_WIDTH - 1)) {
      PLAYER_DEATHS++;

      snake = Snake();
      snake.init(PLAYER_SIZE);
      reset_window(WINDOW_TITLE);

      update_extras(&snake, &fruit);

      continue;
    }

    // updates screen.
    refresh();
    usleep((unsigned) (5 * pow(10, 4)));
  }
}

bool scoreboard_and_restart(WINDOW* scoreboard)
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
      default:  break;
    }
  }

  // if somehow while exits with error, then exit game.
  return false;
}

bool handle_keypress(Snake* snake, int ch)
{
  switch(ch) {
    case 'q': return false;

    case 'w':
    case KEY_UP:    snake->direction = Up;     break;
    case 'a':
    case KEY_LEFT:  snake->direction = Left;   break;
    case 's':
    case KEY_DOWN:  snake->direction = Down;   break;
    case 'd':
    case KEY_RIGHT: snake->direction = Right;  break;

    case ERR:
    default:  break;
  }

  return true;
}

void update_extras(Snake* snake, Coordinates* fruit)
{
  // refreshing the fruit.
  fruit->y = rand_range(1, WINDOW_HEIGHT - 1);
  fruit->x = rand_range(1, WINDOW_WIDTH - 1);

  mvaddwstr(fruit->y, fruit->x, FRUIT_SYM);
  mvprintw(0, WINDOW_WIDTH-PADDING_DEATH_BAR-PADDING_SCORE_BAR, SCORE_BAR_FMT, PLAYER_SCORE);
}

