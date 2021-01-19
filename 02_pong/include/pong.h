#ifndef _PONG_H
#define _PONG_H

#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MIN(a, b) (a) < (b) ? (a) : (b)

#define PADDLE_START_Y  1
#define PADDLE_HEIGHT   10

enum bi_direction_t   { Forward, Backward };
enum diag_direction_t { NE, SE, SW, NW };

struct Coordinates
{
  int x, y;
  Coordinates();
  Coordinates(int, int);
};

bool operator==(const Coordinates&, const Coordinates&);
bool operator!=(const Coordinates&, const Coordinates&);

class Paddle
{
  int replicate_frames = 0;

  public:
    int score = 0;
    int max_replicate_frames = 0;
    int height = PADDLE_HEIGHT;

    bi_direction_t direction;

  public:
    Paddle();
    Paddle(Coordinates, int, int);

    void step();
    void render();

  Coordinates top;
  int lower_bound, upper_bound;
};

class Ball
{
  int replicate_frames        = 0;
  int max_replicate_frames    = 10;

  public:
    Coordinates point, dead_trail;
    int upper_bound, lower_bound;
    diag_direction_t direction  = SE;

  public:
    Ball();
    Ball(int, int);
    Ball(Coordinates, int, int);

    int wall_collision(Paddle*, Paddle*);
    void step();
    void render();
};

#endif
