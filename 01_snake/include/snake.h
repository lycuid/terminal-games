#include <linkedlist.h>

#ifndef _SNAKE_H
#define _SNAKE_H

enum direction_t { Up, Down, Left, Right };

class Snake
{
public:
  Snake();
  Snake(direction_t);
  ~Snake();

public:
  Point       head;
  LinkedList  tail;
  direction_t direction;

  void init(int);
  void step();
  void render();
  bool eat_fruit(Point *);
  bool is_dead(int, int);

private:
  Point dead_trail;
  bool  just_eaten;
};

#endif
