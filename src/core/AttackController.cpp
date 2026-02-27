#include "AttackController.h"
#include "LevelGraph.h"
#include "Level.h"

void AttackController::moveCharacter() {
    Coordinates newCords
        = Coordinates::CoordinatesSumClamped(m_controlledCharacter->getTile()->getCoordinates(),
                                             {0, 1},
                                             9,
                                             9);
    m_controlledCharacter->setTile(Level::GetTile(newCords));
}

AttackController::AttackController(Character* controlledCharacter, Level* level, LevelGraph* graph)
    : AbstractController(controlledCharacter)
    , m_level{level}
    , m_graph{graph} {}

std::pair<int, int> AttackController::DjikstraMove()
{
    Coordinates move = {0, 0};
    if (!isPath){
        Coordinates attackerCords = m_controlledCharacter->getTile()->getCoordinates();
        Coordinates humanCords = m_level->getPlayableCharacter()->getTile()->getCoordinates();
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
    return std::pair{move.row, move.column};
}


std::pair<int, int> AttackController::move()
{
    return DjikstraMove();
}
