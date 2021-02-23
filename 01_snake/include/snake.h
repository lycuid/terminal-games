#include <linkedlist.h>

#ifndef _SNAKE_H
#define _SNAKE_H

enum direction_t { Up, Down, Left, Right };

struct Coordinates
{
  int x, y;
  Coordinates();
  Coordinates(int x, int y);
};

bool operator==(const Coordinates &, const Coordinates &);
bool operator!=(const Coordinates &, const Coordinates &);

class Snake
{
public:
  Snake();
  Snake(direction_t);
  ~Snake();

public:
  Coordinates             head;
  LinkedList<Coordinates> tail;
  direction_t             direction;

  void init(int);
  void step();
  void render();
  bool eat_fruit(Coordinates *);
  bool is_dead(int, int);

private:
  Coordinates dead_trail;
  bool        just_eaten;
};

#endif
