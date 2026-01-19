#include "Subject.h"
#include <qDebug>
Subject::Subject() {
}

void Subject::registerObserver(Observer * newObserver)
{
    m_observersList.push_back(newObserver);
}

void Subject::removeObserver(Observer *observerToRemove)
{
    for (auto it = m_observersList.begin(); it!= m_observersList.end();it++){
        if (*it == observerToRemove){
            m_observersList.erase(it);
            return;
        }
    }
}

void Subject::notifyObservers(std::string changedMemberName){
    for (auto it = m_observersList.begin(); it!= m_observersList.end();it++){
        (*it)->reactToChange(changedMemberName);
    }
}
