#include "list.h"
template<typename T>
Node<T>::Node(T value) {
    m_value = value;
}

template<typename T>
T Node<T>::getValue() {
    return m_value;
}

template<typename T>
void Node<T>::setNextNode(Node *nextNode){
    m_nextNode = nextNode;
}

template<typename T>
void Node<T>::setPrevNode(Node *prevNode){
    m_prevNode = prevNode;
}

template<typename T>
Node<T>* Node<T>::getNextNode() {
    return m_nextNode;
}

template<typename T>
Node<T>* Node<T>::getPrevNode() {
    return m_prevNode;
}

template<typename T>
std::size_t List<T>::size() {

}

template<typename T>
bool List<T>::isEmpty() {
    return (m_lead==nullptr);
}

template<typename T>
void List<T>::pop_front() {
    if (m_lead) {
        m_lead = m_lead->getNextNode();
        delete(m_lead->getPrevNode());
        m_lead->setPrevNode(nullptr);
        m_size--;
    }
    else {
        throw("list_already_empty");
    }
}

template<typename T>
void List<T>::push_back(T elementToAdd) {
    Node<T>* newNode = new Node<T>(elementToAdd);
    if (m_tail) {
        newNode->setPrevNode(m_tail);
        m_tail->setNextNode(newNode);
        m_tail = newNode;
        m_size++;
    }
    else {
        m_lead = newNode;
        m_tail = newNode;


    }
}

template<typename T>
void List<T>::push_front(T elementToAdd) {
    Node<T>* newNode = new Node<T>(elementToAdd);
    if (m_lead) {
        newNode->setNextNode(m_lead);
        m_lead->setPrevNode(newNode);
        m_lead = newNode;
        m_size++;

    }

    else {
        m_lead = newNode;
        m_tail = newNode;
    }
}

template<typename T>
void List<T>::pop_back() {
    if (m_tail) {
        m_tail = m_tail->getPrevNode();
        delete(m_tail->getNextNode());
        m_tail->setNextNode(nullptr);
        m_size--;
    }
    else {
        throw("list_already_empty");
    }
}

template<typename T>
void List<T>::print() {
    Node<T>* currentNode = m_lead;

    while (currentNode) {
        std::cout << currentNode->getValue() << " ";
        currentNode = currentNode->getNextNode();
    }
    std::cout << '\n';
}
