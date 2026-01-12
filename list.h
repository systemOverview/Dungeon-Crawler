#ifndef LIST_H
#define LIST_H
#include <iostream>
template<typename T>
class Node
{
    T m_value{};
    Node *m_nextNode{nullptr};
    Node *m_prevNode{nullptr};

public:
    Node(T value);
    T getValue();
    void setNextNode(Node *nextNode);

    void setPrevNode(Node *prevNode);

    Node *getNextNode();

    Node *getPrevNode();
};

template<typename T>
class List
{
    Node<T> *m_lead = nullptr;
    Node<T> *m_tail = nullptr;
    std::size_t m_size{0};

public:
    struct Iterator
    {
    private:
         Node<T>* m_ptr;

    public:
        Iterator( Node<T>* ptr=nullptr)
            : m_ptr(ptr) {};

        Iterator &operator++()
        {
            m_ptr = m_ptr->getNextNode();
            return *this;
        }
        T operator*() const { return (m_ptr->getValue()); }
        Node<T>* operator->() { return (m_ptr->getValue()); }
        friend bool operator==(const Iterator &a, const Iterator &b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.m_ptr != b.m_ptr; };
    };

    Iterator begin() { return Iterator(m_lead); }
    Iterator end() { return Iterator(m_tail->getNextNode()); }

    void push_back(T elementToAdd);

    void push_front(T elementToAdd);

    void pop_back();

    void pop_front();

    void print();

    bool isEmpty();

    std::size_t size();
};

#endif // LIST_H
