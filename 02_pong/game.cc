#include <unistd.h>
#include <math.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <curses.h>
#include <pong.h>

#define FRAME_RATE  300
#define TIME_SECOND pow(10, 6)

static const char* WINDOW_TITLE = "( EPIC PONG GAME )";
static int WINDOW_HEIGHT;
static int WINDOW_WIDTH;

static const  char* SCOREBOARD_TITLE  = "( SCOREBOARD )";
static const  char* SCOREBOARD_ECHO   = "  press 'q' to quit or 'r' to restart...  ";
static        int SCOREBOARD_HEIGHT;
static        int SCOREBOARD_WIDTH;
static        int SCOREBOARD_Y;
static        int SCOREBOARD_X;

static int P1_SCORE                 = 0;
static int P2_SCORE                 = 0;
static int P2_MAX_REPLICATE_FRAMES  = 20;

void init_window();
void game_loop();

bool handle_keypress(Paddle*, int);
void reset_window();
void reset_players(Paddle*, Paddle*, Ball*);
int rand_range(int, int);
bool scoreboard_and_restart(WINDOW*);

int main(void)
{
  srand(time(0));
  init_window();
  WINDOW* scoreboard = newwin(SCOREBOARD_HEIGHT, SCOREBOARD_WIDTH, SCOREBOARD_Y, SCOREBOARD_X);
  while (1) {
    game_loop();
    if (!scoreboard_and_restart(scoreboard))
      break;
  }
  endwin();
  return 0;
}

void init_window()
{
  setlocale(LC_ALL, "");
  // init screen.
  initscr();
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

void reset_window()
{
  clear();
  box(stdscr, 0, 0);
  mvprintw(0, 2, WINDOW_TITLE);

  char p1_score[10], p2_score[10];
  sprintf(p1_score, "%d", P1_SCORE);
  sprintf(p2_score, "%d", P2_SCORE);

  mvaddstr(5, WINDOW_WIDTH / 4, p1_score);
  mvaddstr(5, (3 * WINDOW_WIDTH) / 4, p2_score);

  refresh();
}

void reset_players(Paddle* player, Paddle* computer, Ball* ball)
{
  *player = Paddle (Coordinates(1, PADDLE_START_Y), 1, WINDOW_HEIGHT - 1);
  *computer = Paddle(Coordinates(WINDOW_WIDTH - 2, PADDLE_START_Y),
      1, WINDOW_HEIGHT - 1);
  computer->max_replicate_frames = P2_MAX_REPLICATE_FRAMES;

  Coordinates ball_coords = Coordinates(2,
      rand_range(PADDLE_START_Y, PADDLE_START_Y + PADDLE_HEIGHT - 1));
  *ball = Ball(ball_coords, 1, WINDOW_HEIGHT - 1);
}

void game_loop()
{
  nodelay(stdscr, true);

  Paddle player, computer;
  Ball ball;

  reset_window();
  reset_players(&player, &computer, &ball);

  int score = 0;

  while (handle_keypress(&player, getch())) {
    // divider line.
    mvvline(1, WINDOW_WIDTH / 2, '|', WINDOW_HEIGHT - 2);

    // computer movement ai (naive).
    computer.direction = ball.point.y >= (computer.top.y + (computer.height / 2))
      ? Forward : Backward;


    computer.step();
    ball.step();

    player.render();
    computer.render();
    ball.render();

    if ((score = ball.wall_collision(&player, &computer)) != 0) {
      P1_SCORE += ((score + 1) / 2);
      P2_SCORE -= ((score - 1) / 2);
      reset_window();
      reset_players(&player, &computer, &ball);
    }

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
    case KEY_UP: {
      player->direction = Backward;
      player->step();
      break;
    }

    case 'd':
    case KEY_RIGHT:
    case 's':
    case KEY_DOWN: {
      player->direction = Forward;
      player->step();
      break;
    }

    case ERR:
    default: break;
  }
  return true;
}

bool scoreboard_and_restart(WINDOW* scoreboard)
{
  // blocking 'getch'.
  nodelay(stdscr, false);

  wattroff(scoreboard, A_STANDOUT);
  wclear(scoreboard);
  box(scoreboard, 0, 0);

  mvwaddstr(scoreboard, 0, (SCOREBOARD_WIDTH - 14) / 2, SCOREBOARD_TITLE);
  mvwprintw(scoreboard, 2, 4, "Player 1: %03d", P1_SCORE);
  mvwprintw(scoreboard, 3, 4, "Player 2: %03d", P2_SCORE);

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

int rand_range(int min, int max)
{
  return (rand() % (min + max)) + min;
}

