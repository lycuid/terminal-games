#include <curses.h>
#include "pong.h"

wchar_t PADDLE_SYM[] = { 0x2593, 0 };

Paddle::Paddle(int l, int u)
  : direction(Backward), top(1), height(10), lower_bound(l), upper_bound(u) {}

void Paddle::render()
{
  // rendering the player.
  for (int i = 0; i < this->height; ++i) {
    mvaddwstr(this->top + i, 1, PADDLE_SYM);
  }

  // erasing dead trail.
  int dead_trail = this->direction == Forward
    ? MAX(this->lower_bound, this->top - 1)
    : MIN(this->upper_bound, this->top + this->height);
  mvaddch(dead_trail, 1, ' ');
}

void Paddle::step()
{
  this->top = this->direction == Backward
    ? MAX(this->lower_bound, this->top - 1)
    : MIN(this->upper_bound - this->height, this->top + 1);
}

