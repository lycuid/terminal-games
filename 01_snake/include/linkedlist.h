#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

struct Point {
  int x, y;
  Point();
  Point(int x, int y);
};

bool operator==(const Point &, const Point &);
bool operator!=(const Point &, const Point &);

struct Node {
  Point value;
  Node *next;
  Node(Point val) : value(val), next(nullptr){};
  Node(Point val, Node *next) : value(val), next(next){};
};

class LinkedList
{
  Node *   _first;
  Node *   _last;
  unsigned _size;

public:
  LinkedList();
  LinkedList(Node *);
  ~LinkedList();

  Node *   first();
  Node *   last();
  unsigned size();

  void  push_back(Node *);
  void  push_front(Node *);
  Node *pop();
};

#endif
