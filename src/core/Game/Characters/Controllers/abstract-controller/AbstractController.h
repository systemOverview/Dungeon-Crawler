#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>

struct Coordinates;
class Character;
class Level;

class AbstractController : public QObject

{
    Q_OBJECT

protected:
    Character* m_controlledCharacter = nullptr;

    virtual Coordinates getNextMove() = 0;

public slots:
    virtual void moveCharacter();

public:
    AbstractController(Character* controlledCharacter);
    static AbstractController* CreateCharacterController(Character* character);
};

#endif // ABSTRACTCONTROLLER_H
