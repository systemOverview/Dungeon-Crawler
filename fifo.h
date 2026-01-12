#ifndef FIFO_H
#define FIFO_H
#include <iostream>
template<typename T>
class Element
{
public:
    T value;
    explicit Element(T i)
        : value(i) {};
    ~Element() {}
};
// typedef Element T;

template<typename T>

class Queue
{
private:
    Element<T> *arrayPtr;
    size_t size{0};
    size_t capacity;

public:
    Queue(size_t reqCapacity = 0);

    ~Queue();

    int at(size_t index);
    void reserve(size_t reqCapacity);

    void resize(size_t reqCapacity);

    void push_back(T element);

    void pop_back();
    size_t getCapacity();

    size_t getSize();

    void arrayInfo();
};

#endif // FIFO_H
