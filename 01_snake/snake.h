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
  Coordinates head;
  LinkedList<Coordinates> tail;
  direction_t direction;

  Coordinates previous_dead;
  bool just_eaten;

public:
  Snake();
  Snake(direction_t);
  ~Snake();

  void init(int);
  void step();
  void draw();
  bool eat_fruit(Coordinates*);
  bool check_dead(int, int);
};

