#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>
class Character;
class AbstractController : public QObject

{
    Q_OBJECT

protected:
    Character* m_controlledCharacter;

public slots:
    virtual void moveCharacter() = 0;

public:
    AbstractController(Character* controlledCharacter);
};

#endif // ABSTRACTCONTROLLER_H
