#ifndef ATTACKCONTROLLER_H
#define ATTACKCONTROLLER_H
#include "AbstractUI.h"
#include <QtCore/qdebug.h>
class LevelGraph;
class AttackController : public AbstractController
{
private:
    Level* m_level;
    LevelGraph* m_graph;
public:
    AttackController(Level *level, LevelGraph *graph);
    std::vector<std::pair<int,int>> m_path;
    bool isPath = false;
    std::vector<std::pair<int,int>>::iterator it;
    std::pair<int, int> DjikstraMove();
    std::pair<int, int> move() override;
    std::pair<int,int> move(std::pair<int,int> startingTileCords);

};

#endif // ATTACKCONTROLLER_H
