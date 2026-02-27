#ifndef ATTACKCONTROLLER_H
#define ATTACKCONTROLLER_H
#include <QtCore/qdebug.h>
#include "AbstractUI.h"
#include "Constants.h"
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
    std::vector<Coordinates> m_path;
    bool isPath = false;
    std::vector<Coordinates>::iterator it;
    std::pair<int, int> DjikstraMove();
    std::pair<int, int> move() override;
    std::pair<int,int> move(std::pair<int,int> startingTileCords);

};

#endif // ATTACKCONTROLLER_H
