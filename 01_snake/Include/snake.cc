#include <stdlib.h>
#include <curses.h>
#include "snake.h"

/*
 * Snake structure:
 * [tail.first, ........, tail.last](head)
 */

Coordinates::Coordinates() : x(-1), y(-1) {};
Coordinates::Coordinates(int x, int y) : x(x), y(y) {};

wchar_t SNAKE_HEAD_SYM[]  = { 0x2593, 0 };
wchar_t SNAKE_BODY_SYM[]  = { 0x2592, 0 };
wchar_t EMPTY_SYM[]       = { 0x0020, 0 };

bool operator==(const Coordinates& lhs, const Coordinates& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Coordinates& lhs, const Coordinates& rhs) {
  return !(lhs == rhs);
}

Coordinates NULLCRDS = Coordinates();

Snake::Snake() : direction(Right), just_eaten(false) {}
Snake::Snake(direction_t d) : direction(d), just_eaten(false) {}
Snake::~Snake() {}

void Snake::init(int length)
{
  for (int i = 1; i < length + 1; ++i) {
    Node<Coordinates>* n = (Node<Coordinates>*) malloc(sizeof(Coordinates));
    n->value = Coordinates(i, 1);
    n->next = nullptr;

    (this->tail).push_back(n);
  }
  this->head = Coordinates(length + 1, 1);
  this->dead_trail = Coordinates();
}

void Snake::step()
{
  Node<Coordinates>* stack_node;

  if (this->just_eaten) {
    stack_node = (Node<Coordinates>*) malloc(sizeof(Coordinates));
    this->dead_trail = Coordinates();
  } else {
    // pop from top, to reuse in the following steps.
    stack_node = (this->tail).pop();
    // keeping track of the lost coordinates, in order to paint blank on next render.
    this->dead_trail = stack_node->value;
  }
  // stack the previously popped 'Node', at the bottom like a stacking a pencil.
  // reusing the block, instead of creating/destroying.
  stack_node->value = this->head;
  stack_node->next = nullptr;

  (this->tail).push_back(stack_node);

  // updating head to new coordinates with respect to 'direction_t'.
  switch (this->direction) {
    case Left:  (this->head).x--; break;
    case Right: (this->head).x++; break;
    case Up:    (this->head).y--; break;
    case Down:  (this->head).y++; break;
    default:    break;
  }
}

void Snake::render()
{
  if (this->dead_trail != NULLCRDS && !this->just_eaten)
    mvaddwstr((this->dead_trail).y, (this->dead_trail).x, EMPTY_SYM);

  // rendering head.
  Coordinates snake_head = this->head;
  mvaddwstr(snake_head.y, snake_head.x, SNAKE_HEAD_SYM);

  // replacing last of snake body (tail.first) with empty block, which helps
  // with not clearing and re-rendering the entire screen on every render.
  for (Node<Coordinates>* snake_tail = (this->tail).first();
      snake_tail != nullptr; snake_tail = snake_tail->next)
  {
    mvaddwstr((snake_tail->value).y,
        (snake_tail->value).x, SNAKE_BODY_SYM);
  }
}

bool Snake::is_dead(int height, int width)
{
  // check self destruction.
  for (Node<Coordinates>* snake_tail = (this->tail).first();
      snake_tail != nullptr; snake_tail = snake_tail->next)
  {
    if (this->head == snake_tail->value)
      return true;
  }

  // check wall collision.
  return !((this->head).y > 0 && (this->head).y < height
    && (this->head).x > 0 && (this->head).x < width);
}

bool Snake::eat_fruit(Coordinates* fruit)
{
  return this->just_eaten = fruit != nullptr ? this->head == *fruit : false;
}

