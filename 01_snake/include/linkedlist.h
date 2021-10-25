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
public:
  Node *   first;
  Node *   last;
  unsigned size;

  LinkedList();
  LinkedList(Node *);
  ~LinkedList();

  void  push_back(Node *);
  void  push_front(Node *);
  Node *pop();
};

#endif // _LINKEDLIST_H
