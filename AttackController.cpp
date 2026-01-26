#include "AttackController.h"
#include "LevelGraph.h"
#include "Level.h"

AttackController::AttackController(Level *level, LevelGraph *graph): m_level{level}, m_graph{graph}
{

}

std::pair<int, int> AttackController::DjikstraMove()
{
    qDebug() << m_path;
    std::pair<int,int> move = {0,0};
    if (!isPath){
        std::pair<int,int> attackerCords = m_character->getTile()->getCordsAsPair();
        std::pair<int,int> humanCords = m_level->getPlayableCharacter()->getTile()->getCordsAsPair();
        Vertex* attackerVertex = m_graph->getVertex(attackerCords);
        Vertex* humanVertex = m_graph->getVertex(humanCords);
        m_path= m_graph->getShortestsPathBetweenTwoTiles(attackerVertex, humanVertex);
        if (m_path.empty()){
            return {0,0};
        }
        it = m_path.begin();
        move = *it;
        it++;
    }
    else{
        if (m_path.empty()){
            return {0,0};
        }
        if (it!=m_path.end()){
            move=*it;
            it++;
        }
    }
    // return {0,0};
    return move;
}


std::pair<int, int> AttackController::move()
{
    return DjikstraMove();
}

void AttackController::reactToChange(std::string changedMemberName)
{
    return;
}
