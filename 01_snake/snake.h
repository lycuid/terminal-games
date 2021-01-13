#ifndef __SNAKE_H
#define __SNAKE_H

#include <linkedlist.h>

enum direction_t { Up, Down, Left, Right };

struct Coordinates
{
  int x;
  int y;
  Coordinates() : x(-1), y(-1) {};
  Coordinates(int x, int y) : x(x), y(y) {};
};

bool operator==(const Coordinates&, const Coordinates&);

class Snake
{
  public:
    Snake();
    Snake(direction_t);
    ~Snake();

  public:
    Coordinates head;
    LinkedList<Coordinates> tail;
    direction_t direction;

    void init(int);
    void step();
    void draw();
    bool eat_fruit(Coordinates*);
    bool is_dead(int, int);

  private:
    Coordinates dead_trail;
    bool just_eaten;
};

#endif

