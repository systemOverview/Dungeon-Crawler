#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>
#include <utility>
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
    // TODO : fix the move function across subclasses
    virtual std::pair<int, int> move() = 0;
    virtual std::pair<int, int> translateMove(int step);
};

#endif // ABSTRACTCONTROLLER_H
