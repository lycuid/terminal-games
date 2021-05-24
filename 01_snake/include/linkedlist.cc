#include "linkedlist.h"

Point::Point() : x(-1), y(-1) {}
Point::Point(int x, int y) : x(x), y(y) {}

LinkedList::LinkedList() : _first(nullptr), _last(nullptr), _size(0) {}
LinkedList::LinkedList(Node *n) : _first(n), _last(n), _size(1) {}
LinkedList::~LinkedList() {}

Node *   LinkedList::first() { return this->_first; }
Node *   LinkedList::last() { return this->_last; }
unsigned LinkedList::size() { return this->_size; }

void LinkedList::push_back(Node *node)
{
  if (this->_first == nullptr) this->_first = node;
  if (this->_last != nullptr) this->_last->next = node;

  this->_last = node;
  this->_size++;
}

void LinkedList::push_front(Node *node)
{
  if (this->_last == nullptr) this->_last = node;
  if (this->_first != nullptr) node->next = this->_first;

  this->_first = node;
  this->_size++;
}

Node *LinkedList::pop()
{
  Node *first = this->_first;
  if (first != nullptr) this->_first = first->next;
  this->_size--;
  return first;
}
