#include "LevelGraph.h"
#include <qDebug>;

Vertex::Vertex(Tile* tile, float weight) : m_tile{tile}, m_weight{weight}{};

void Vertex::addNeighbour(Vertex *neighbourToAdd)
{
    m_neighbours.push_back(neighbourToAdd);
}

Tile* Vertex::getTile()
{
    return m_tile;
}

void Vertex::setTile(Tile *newTile)
{
    m_tile = newTile;
}

float Vertex::getWeight(){
    return m_weight;
}

std::vector<Vertex*> Vertex::getNeighbours(){
    return m_neighbours;
}

// graph definitions


LevelGraph::LevelGraph()
{

}

void LevelGraph::addVertex(Tile *tile, float weight)
{
    Vertex* newVertex = new Vertex(tile, weight);
    std::pair<int,int> cords = {tile->getRow(), tile->getColumn()};
    m_vertexes[cords] =  newVertex;
}
void LevelGraph::setupAlldges()
{
    for (auto it = m_vertexes.begin(); it!=m_vertexes.end(); it++){
        setupEdgesForVertex((*it).second);
    }
}

void LevelGraph::setupEdgesForVertex(Vertex *vertex)
{

    Tile* tile = vertex->getTile();
    for (int i=tile->getRow()-1; i<=tile->getRow()+1 ; i++){
        for (int j=tile->getColumn()-1; j<=tile->getColumn()+1; j++){
            if ((i!=tile->getRow() || j!=tile->getColumn()) && m_vertexes.count({i,j})){
                Vertex* neighbourVertex = m_vertexes[{i,j}];
                Tile* neighbourTile = neighbourVertex->getTile();
                if (isEdgeBetweenTilesPossible(tile->getTexture(), neighbourTile->getTexture())){
                    vertex->addNeighbour(neighbourVertex);
                }
            }
        }
    }
}

bool LevelGraph::isEdgeBetweenTilesPossible(std::string_view firstTileTexture, std::string_view secondTileTexture)
{
    if (secondTileTexture=="#" || secondTileTexture=="$" || secondTileTexture=="X" ){ // treat level changer and closed doors as not a possible path
        return false;
    }
    else if (firstTileTexture=="_" && (secondTileTexture!="<" && secondTileTexture!="_")){
        return false;
    }
    return true;
}

Vertex *LevelGraph::getVertex(std::pair<int, int> cords)
{
    if (m_vertexes.count(cords)){
        return m_vertexes[cords];
    }
    else{
        return nullptr;
    }
}


