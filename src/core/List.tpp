#ifndef LIST_H
#define LIST_H
#include <cstddef>
#include <stdexcept>

template<typename T>
class List
{
    struct Node
    {
        T value;
        Node* next;
        Node* prev;
        Node(T value, Node* next, Node* prev)
            : value{value}
            , next{next}
            , prev{prev} {}
    };
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
    std::size_t m_size = 0;

public:
    class Iterator
    {
        friend class List;
        Node* m_nodePointer;

    private:
        Node* getNode() { return m_nodePointer; }

    public:
        Iterator& operator++() {
            m_nodePointer = m_nodePointer->next;
            return *this;
        }

        Iterator& operator--() {
            m_nodePointer = m_nodePointer->prev;
            return *this;
        }

        T& operator*() { return m_nodePointer->value; }
        bool operator==(const Iterator& rhs) { return (m_nodePointer == rhs.m_nodePointer); }

        bool operator!=(const Iterator& rhs) { return (m_nodePointer != rhs.m_nodePointer); }

        Iterator(Node* nodePointer)
            : m_nodePointer{nodePointer} {};
    };
    void push_back(T element) {
        if (!m_head) {
            Node* newNode = new Node{element, nullptr, nullptr};
            m_head = newNode, m_tail = newNode;
            return;
        }
        m_tail->next = new Node{element, nullptr, m_tail};
        m_tail = m_tail->next;
        m_size++;
    }
    void push_front(T element) {
        if (!m_head) {
            m_head = new Node{element, nullptr, nullptr};
            return;
        }
        Node* newNode = new Node(element, m_head, nullptr);
        m_head->prev = newNode;
        m_head = newNode;
        m_size++;
    }
    void pop_back() {
        if (m_size <= 0) {
            throw(std::out_of_range("Array already empty"));
        }
        else {
            Node* secondToLastNode = m_tail->prev;
            delete m_tail;
            secondToLastNode->next = nullptr;
            m_tail = secondToLastNode;
            m_size -= 1;
        }
    }
    void pop_front() {
        if (m_size <= 0) {
            throw(std::out_of_range("Array already empty"));
        }
        else {
            Node* secondNode = m_head->next;
            delete m_head;
            m_head = nullptr;
            secondNode->prev = nullptr;
            m_head = secondNode;
            m_size -= 1;
        }
    }
    void remove(T element) {
        for (List<T>::Iterator it = begin(); it != end();) {
            if (*it == element) {
                Node* elementNode = it.getNode();
                if (elementNode->prev) {
                    (elementNode->prev)->next = elementNode->next;
                }
                else {
                    m_head = elementNode->next; // the next if block will update the new head
                }

                if (elementNode->next) {
                    (elementNode->next)->prev = elementNode->prev;
                }
                else {
                    m_tail = elementNode->prev;
                }
                it = elementNode->next;
                delete elementNode;
                elementNode = nullptr;
            }
            else {
                ++it;
            }
        }
    }
    bool empty() { return m_size <= 0; }
    void print();
    Iterator begin() { return Iterator(m_head); }
    Iterator end() { return Iterator(nullptr); }
    std::size_t size() { return m_size; }

public:
    List<T>() {}
};

#endif // LIST_H
