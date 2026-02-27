#ifndef ATTACKCONTROLLER_H
#define ATTACKCONTROLLER_H
#include <QtCore/qdebug.h>
#include "AbstractController.h"
class Level;
class LevelGraph;
class AttackController : public AbstractController
{
    Q_OBJECT
private:
    Level* m_level;
    LevelGraph* m_graph;
    Coordinates getNextMove() override;

public:
    AttackController(Character* controlledCharacter);
};

#endif // ATTACKCONTROLLER_H
