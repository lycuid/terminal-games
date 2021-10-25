#include <cstdlib>
#include <curses.h>

#include "snake.h"

/*
 * Snake structure:
 * [tail.first, ........, tail.last](head)
 */

static const char SNAKE_HEAD_SYM = '#';
static const char SNAKE_BODY_SYM = '#';
static Point      nullpnt        = Point();

bool operator==(const Point &lhs, const Point &rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Point &lhs, const Point &rhs) { return !(lhs == rhs); }

Snake::Snake() : direction(Right), just_ate(false) {}
Snake::Snake(direction_t d) : direction(d), just_ate(false) {}
Snake::~Snake() {}

void Snake::init(int length)
{
  for (int i = 1; i < length + 1; ++i) {
    Node *n  = (Node *)malloc(sizeof(Node));
    n->value = Point(i, 1);
    n->next  = nullptr;

    this->tail.push_back(n);
  }
  this->head  = Point(length + 1, 1);
  this->trail = Point();
}

void Snake::step()
{
  Node *node;

  if (this->just_ate) {
    node        = (Node *)malloc(sizeof(Node));
    this->trail = Point();
  } else {
    // pop from top, to reuse in the following steps.
    node = this->tail.pop();
    // keeping track of the lost coordinates, in order to paint blank on next
    // render.
    this->trail = node->value;
  }
  // stack the previously popped 'Node', at the bottom like a stacking a
  // pencil. reusing the block, instead of creating/destroying.
  node->value = this->head;
  node->next  = nullptr;

  this->tail.push_back(node);

  // updating head to new coordinates with respect to 'direction_t'.
  switch (this->direction) {
    case Left: this->head.x--; break;
    case Right: this->head.x++; break;
    case Up: this->head.y--; break;
    case Down: this->head.y++; break;
    default: break;
  }
}

void Snake::render(WINDOW *canvas)
{
  if (this->trail != nullpnt && !this->just_ate)
    mvwaddch(canvas, this->trail.y, this->trail.x, ' ');

  // render head.
  mvwaddch(canvas, this->head.y, this->head.x, SNAKE_HEAD_SYM);

  // render tail.
  Node *snake_tail;
  for (snake_tail = this->tail.first; snake_tail != nullptr;
       snake_tail = snake_tail->next)
    mvwaddch(canvas, snake_tail->value.y, snake_tail->value.x, SNAKE_BODY_SYM);
}

bool Snake::is_dead(int height, int width)
{
  // self collision.
  Node *snake_tail;
  for (snake_tail = this->tail.first; snake_tail != nullptr;
       snake_tail = snake_tail->next) {
    if (this->head == snake_tail->value) return true;
  }

  // wall collision.
  return !(this->head.y > 0 && this->head.y<height &&this->head.x> 0 &&
           this->head.x < width);
}

bool Snake::ate_fruit(const Point *fruit)
{
  return this->just_ate = fruit != nullptr ? this->head == *fruit : false;
}
