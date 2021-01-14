#ifndef _PONG_H
#define _PONG_H

#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MIN(a, b) (a) < (b) ? (a) : (b)

enum direction_t { Forward, Backward };

class Paddle
{
  public:
    direction_t direction;

  public:
    Paddle(int, int);

    void step();
    void render();

  int top, height, lower_bound, upper_bound;
};

#endif
