#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

template <typename T>
struct Node
{
  T value;
  Node* next;
  Node(T val) : value(val), next(nullptr) {};
  Node(T val, Node* next) : value(val), next(next) {};
};

template <typename T>
class LinkedList
{
  Node<T>* _first;
  Node<T>* _last;
  unsigned _size;

  public:
    LinkedList();
    LinkedList(Node<T>*);
    ~LinkedList();

    Node<T>* first();
    Node<T>* last();
    unsigned size();

    void push_back(Node<T>*);
    void push_front(Node<T>*);
    Node<T>* pop();
};


template <typename T> LinkedList<T>::LinkedList() : _first(nullptr), _last(nullptr), _size(0) {}
template <typename T> LinkedList<T>::LinkedList(Node<T>* n) : _first(n), _last(n), _size(1) {}
template <typename T> LinkedList<T>::~LinkedList() {}

template <typename T> Node<T>* LinkedList<T>::first() { return this->_first; }
template <typename T> Node<T>* LinkedList<T>::last() { return this->_last; }
template <typename T> unsigned LinkedList<T>::size() { return this->_size; }

template <typename T> void LinkedList<T>::push_back(Node<T>* node)
{
  if (this->_first == nullptr) this->_first = node;
  if (this->_last != nullptr) this->_last->next = node;

  this->_last = node;
  this->_size++;
}

template <typename T> void LinkedList<T>::push_front(Node<T>* node)
{
  if (this->_last == nullptr) this->_last = node;
  if (this->_first != nullptr) node->next = this->_first;

  this->_first = node;
  this->_size++;
}

template <typename T> Node<T>* LinkedList<T>::pop()
{
  Node<T>* first = this->_first;
  if (first != nullptr)
    this->_first = first->next;
  this->_size--;
  return first;
}

#endif
