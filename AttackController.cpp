#include "AttackController.h"

AttackController::AttackController(Level *level, LevelGraph *graph): m_level{level}, m_graph{graph}
{

}

std::pair<int, int> AttackController::DjikstraMove()
{
    std::pair<int,int> move = {0,0};
    if (!isPath){
        std::pair<int,int> attackerCords = m_character->getTile()->getCordsAsPair();
        std::pair<int,int> humanCords = m_level->getPlayableCharacter()->getTile()->getCordsAsPair();
        Vertex* attackerVertex = m_graph->getVertex(attackerCords);
        Vertex* humanVertex = m_graph->getVertex(humanCords);
        qDebug() << "textures " << attackerVertex->getTile()->getCharacter()->getTexture() << humanVertex->getTile()->getCharacter()->getTexture();
        qDebug() << "cords " << attackerCords << humanCords;
        m_path= m_graph->getShortestsPathBetweenTwoTiles(attackerVertex, humanVertex);

        qDebug() << m_path;
        it = m_path.begin();
        isPath = true;
        move = *it;
        it++;
    }
    else{
        if (it!=m_path.end()){
            move=*it;
            it++;
        }
        else{
            qDebug() << "endpath";
        }
    }
    qDebug() << move;
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
