#include "linkedlist.h"

Point::Point() : x(-1), y(-1) {}
Point::Point(int x, int y) : x(x), y(y) {}

LinkedList::LinkedList() : first(nullptr), last(nullptr), size(0) {}
LinkedList::LinkedList(Node *n) : first(n), last(n), size(1) {}
LinkedList::~LinkedList() {}

void LinkedList::push_back(Node *node)
{
  if (this->first == nullptr) this->first = node;
  if (this->last != nullptr) this->last->next = node;

  this->last = node;
  this->size++;
}

void LinkedList::push_front(Node *node)
{
  if (this->last == nullptr) this->last = node;
  if (this->first != nullptr) node->next = this->first;

  this->first = node;
  this->size++;
}

Node *LinkedList::pop()
{
  Node *first = this->first;
  if (first != nullptr) this->first = first->next;
  this->size--;
  return first;
}
