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
public slots:
    void moveCharacter() override;

public:
    AttackController(Character* controlledCharacter, Level* level, LevelGraph* graph);

};

#endif // ATTACKCONTROLLER_H
