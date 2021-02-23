#include "pong.h"
#include <curses.h>

wchar_t PADDLE_SYM[] = {0x2592, 0};
wchar_t BALL_SYM[]   = {0x2592, 0};

Coordinates::Coordinates() : x(-1), y(-1) {}
Coordinates::Coordinates(int x, int y) : x(x), y(y) {}

bool operator==(const Coordinates &lhs, const Coordinates &rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Coordinates &lhs, const Coordinates &rhs) { return !(lhs == rhs); }

Coordinates nullcrds = Coordinates();

Paddle::Paddle() {}
Paddle::Paddle(Coordinates top, int l, int u)
    : direction(Backward), top(top), lower_bound(l), upper_bound(u)
{
}

void Paddle::render()
{
  // rendering the paddle.
  for (int i = 0; i < this->height; ++i) {
    mvaddwstr(this->top.y + i, this->top.x, PADDLE_SYM);
  }

  // erasing dead trail.
  int dead_trail = this->direction == Forward
                       ? MAX(this->lower_bound, this->top.y - 1)
                       : MIN(this->upper_bound, this->top.y + this->height);
  mvaddch(dead_trail, this->top.x, ' ');
}

void Paddle::step()
{
  if (this->replicate_frames++ < this->max_replicate_frames) return;
  this->replicate_frames = 0;

  this->top.y = this->direction == Backward
                    ? MAX(this->lower_bound, this->top.y - 1)
                    : MIN(this->upper_bound - this->height, this->top.y + 1);
}

Ball::Ball() {}
Ball::Ball(int u, int l) : point(Coordinates(2, 1)), upper_bound(u), lower_bound(l) {}
Ball::Ball(Coordinates point, int u, int l) : point(point), upper_bound(u), lower_bound(l)
{
}

void Ball::step()
{
  if (this->replicate_frames++ < this->max_replicate_frames) return;
  this->replicate_frames = 0;

  this->dead_trail = this->point;
  switch (this->direction) {
  case NE: {
    this->point.x++;
    this->point.y--;
    break;
  }
  case SE: {
    this->point.x++;
    this->point.y++;
    break;
  }
  case SW: {
    this->point.x--;
    this->point.y++;
    break;
  }
  case NW: {
    this->point.x--;
    this->point.y--;
    break;
  }
  default: break;
  }
}

void Ball::render()
{
  if (this->dead_trail != nullcrds) mvaddch(this->dead_trail.y, this->dead_trail.x, ' ');
  mvaddwstr(this->point.y, this->point.x, BALL_SYM);
}

int Ball::wall_collision(Paddle *player, Paddle *computer)
{
  // not using 'if/else' here as to solve the corner problem.
  // maximum two of these conditions can be true at a time.

  // handle top wall.
  if (this->point.y == this->upper_bound + 1) {
    switch (this->direction) {
    case NE: this->direction = SE; break;
    case NW: this->direction = SW; break;
    default: break;
    }
  }

  // handle bottom wall.
  if (this->point.y == this->lower_bound - 1) {
    switch (this->direction) {
    case SE: this->direction = NE; break;
    case SW: this->direction = NW; break;
    default: break;
    }
  }

  // handle player paddle.
  if (this->point.x == (player->top.x + 1)) {
    if (this->point.y >= player->top.y &&
        this->point.y <= (player->top.y + player->height)) {
      switch (this->direction) {
      case NW: this->direction = NE; break;
      case SW: this->direction = SE; break;
      default: break;
      }
    } else
      return -2;
  }

  // handle computer paddle.
  if (this->point.x == (computer->top.x - 1)) {
    if (this->point.y >= computer->top.y &&
        this->point.y <= (computer->top.y + computer->height)) {
      switch (this->direction) {
      case NE: this->direction = NW; break;
      case SE: this->direction = SW; break;
      default: break;
      }
    } else
      return 2;
  }

  return 0;
}
