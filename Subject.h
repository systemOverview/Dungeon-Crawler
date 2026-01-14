#ifndef SUBJECT_H
#define SUBJECT_H
#include "Observer.h"
#include <vector>

class Subject
{
private:
    std::vector<Observer*> m_observersList;
public:
    Subject();
    void registerObserver(Observer* newObserver);
    void removeObserver(Observer* observerToRemove);
    void notifyObservers(std::string changedMemberName);
};

#endif // SUBJECT_H
