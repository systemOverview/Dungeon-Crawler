#include "fifo.h"
template <typename T>
Queue<T>::Queue(size_t reqCapacity) {
    Element<T>* array = new Element<T>[reqCapacity];
    arrayPtr = array;
    capacity = reqCapacity;
}
template <typename T>

Queue<T>::~Queue() {
    delete[] arrayPtr;
}

template <typename T>
int Queue<T>::at(size_t index) {
    if (index>=size) {
        throw std::out_of_range("index exceeds size");
    }
    return arrayPtr[index].value;
}
template <typename T>
void Queue<T>::reserve(size_t reqCapacity) {
    if (reqCapacity>=size) {
        Element<T>* newArray = new Element<T>[reqCapacity];
        for (size_t counter = 0;counter<size;counter++) {
            newArray[counter]=arrayPtr[counter];
        }
        delete[] arrayPtr;
        arrayPtr = newArray;
        capacity = reqCapacity;
    }
    else {
        throw std::out_of_range("size exceeds capacity");
    }

}

template <typename T>
void Queue<T>::resize(size_t reqCapacity) {
    size_t counter = 0;
    Element<T>* newArray = new Element<T>[reqCapacity];
    while (counter<reqCapacity && counter<size) {
        newArray[counter]=arrayPtr[counter];
        counter++;
    }

    delete[] arrayPtr;
    arrayPtr = newArray;
    capacity = reqCapacity;
    size = counter;
}

template <typename T>
void Queue<T>::push_back(T element) {
    Element<T>* elementToAdd = new Element<T> (element);
    if (size==capacity) {
        reserve(capacity*2);
        std::cout << "done";
    }
    arrayPtr[size] = elementToAdd;
    size++;
}

template <typename T>
void Queue<T>::pop_back(){
    if (size==0) {
        throw std::out_of_range("already empty");
    }

    Element<T>* toReturn = arrayPtr[size-1];
    size--;
    // capacity = 12
    // Size = 7
    // pop_back -> size = 6
    //


    if (size<capacity/2){
        this->resize(capacity/2);
    }

}

template <typename T>
size_t Queue<T>::getCapacity() {
    return capacity;
}

template <typename T>
size_t Queue<T>::getSize() {
    return size;
}
template <typename T>
void Queue<T>::arrayInfo(){
    std::cout << "the array has " << size << " elements and capacity of " << capacity << std::endl << "the array elements are " << std::endl ;
    // for (size_t counter = 0; counter <size; counter++) {
    //     std::cout << arrayPtr[counter].value << " ";
    // }
    std::cout << std::endl;

}
