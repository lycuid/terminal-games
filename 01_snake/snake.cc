#include <stdlib.h>
#include <curses.h>
#include "snake.h"

// wchar_t SNAKE_HEAD_SYM[]  = { 0x0254 };
wchar_t SNAKE_HEAD_SYM[]  = { 0x2593, 0 };
wchar_t SNAKE_BODY_SYM[]  = { 0x2592, 0 };
wchar_t EMPTY_SYM[]       = { 0x0020, 0 };

/*
 * Snake structure:
 * [tail.first, ........, tail.last](head)
 */

bool operator==(const Coordinates& a, const Coordinates& b)
{
  return a.y == b.y && a.x == b.x;
}

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
  this->previous_dead = Coordinates();
}

void Snake::step()
{

  // pop from top, to reuse in leading steps.
  Node<Coordinates>* head_node = this->just_eaten
    ? (Node<Coordinates>*) malloc(sizeof(Coordinates)) : (this->tail).pop();

  // keeping track of the lost coordinates, in order to paint blank on next render.
  this->previous_dead = this->just_eaten ? Coordinates() : head_node->value;

  // stack the previously popped block, at the bottom like a stacking a pencil.
  // reusing the block, instead of creating/destroying.
  head_node->value = this->head;
  head_node->next = nullptr;

  (this->tail).push_back(head_node);

  // updating head to new coordinates with respect to 'direction_t'.
  switch (this->direction) {
    case Left:  (this->head).x--; break;
    case Right: (this->head).x++; break;
    case Up:    (this->head).y--; break;
    default:    (this->head).y++;
  }
}

void Snake::draw()
{
  if ((this->previous_dead).y >= 0 && (this->previous_dead).x >= 0
      && !this->just_eaten)
    mvaddwstr((this->previous_dead).y, (this->previous_dead).x, EMPTY_SYM);

  Coordinates snake_head = this->head;
  Node<Coordinates>* snake_tail = (this->tail).first();

  // drawing head.
  mvaddwstr(snake_head.y, snake_head.x, SNAKE_HEAD_SYM);

  // replacing last of snake body (tail.first) with empty block, which helps
  // with not clearing and redrawing the entire screen on every render.
  mvaddwstr((snake_tail->value).y, (snake_tail->value).x, SNAKE_BODY_SYM);

  // drawing the rest of the tail.
  while ((snake_tail = snake_tail->next) != nullptr) {
    mvaddwstr((snake_tail->value).y, (snake_tail->value).x, SNAKE_BODY_SYM);
  }
}

bool Snake::check_dead(int height, int width)
{
  // check self destruction.
  Node<Coordinates>* snake_tail = (this->tail).first();
  while (snake_tail != nullptr) {
    if (this->head == snake_tail->value)
      return true;
    snake_tail = snake_tail->next;
  }

  // check wall collision.
  return !((this->head).y > 0 && (this->head).y < height
    && (this->head).x > 0 && (this->head).x < width);
}

bool Snake::eat_fruit(Coordinates* fruit)
{
  return this->just_eaten = fruit != nullptr ? this->head == *fruit : false;
}

