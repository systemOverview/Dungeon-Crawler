#ifndef ATTACKCONTROLLER_H
#define ATTACKCONTROLLER_H
#include "AbstractUI.h"
#include "LevelGraph.h"
#include "Level.h"
#include "Observer.h"
#include <QtCore/qdebug.h>
class AttackController : public AbstractController, public Observer
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
    void reactToChange(std::string changedMemberName="") override;

};

#endif // ATTACKCONTROLLER_H
