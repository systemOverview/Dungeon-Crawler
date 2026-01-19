#include "LevelGraph.h"
#include <qDebug>

Vertex::Vertex(Tile* tile, float weight) : m_tile{tile}, m_weight{weight}{};

void Vertex::addNeighbour(Vertex *neighbourToAdd, float weight)
{
    m_neighbours.push_back({neighbourToAdd, weight});
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

std::vector<std::pair<Vertex*, float>> Vertex::getNeighbours(){
    return m_neighbours;
}

// graph definitions


LevelGraph::LevelGraph()
{
    EventBus::subscribeToEvent<EventBus::TileChange>(this);
}

Vertex* LevelGraph::addVertex(Tile *tile, float weight)
{
    Vertex* newVertex = new Vertex(tile, weight);
    std::pair<int,int> cords = {tile->getRow(), tile->getColumn()};
    m_vertexes[cords] =  newVertex;
    return newVertex;
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
                    vertex->addNeighbour(neighbourVertex, 1);
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

std::vector<std::pair<int, int> > LevelGraph::getShortestsPathBetweenTwoTiles(Vertex *startingVertex, Vertex *targetVertex, std::string_view algorithmToUse)
{
    return getShortestsPathBetweenTwoTilesDjikstra(startingVertex, targetVertex);
}
std::vector<std::pair<int, int> > LevelGraph::getShortestsPathBetweenTwoTilesDjikstra(Vertex *startingVertex, Vertex *targetVertex)
{
    std::bitset<200> visited;
    auto bitsetIndex = [](Vertex* vertex) -> int {
        return (vertex->getTile()->getRow()*10+vertex->getTile()->getColumn());
    };
    std::vector<Vertex*> visitedVertexes;
    std::map<Vertex*, std::vector<std::pair<int,int>>> pathTowardsVertex;
    std::map<Vertex*, float> djikstraValues;
    visitedVertexes.push_back(startingVertex);
    djikstraValues[startingVertex] = 0;
    visited[bitsetIndex(startingVertex)]=1;
    pathTowardsVertex[startingVertex] = {};
    while (true){
        std::pair<Vertex*, float> shortestPath = {nullptr, 10000};
        Vertex* leadingVertex = nullptr; // the vertex the shortestPath originated from.
        int counter=0;
        for (auto it = visitedVertexes.begin(); it!=visitedVertexes.end();it++){
            std::vector<std::pair<Vertex*, float>> neighbours = (*it)->getNeighbours();
            for (auto nit = neighbours.begin(); nit!=neighbours.end(); nit++){
                if (!visited[bitsetIndex((*nit).first)]){
                    counter++;
                    float value = (*nit).second+djikstraValues[*it];
                    int intValue = value;
                    EventBus::transmitEvent<EventBus::AnimateTile>((*nit).first->getTile(), {AnimateTileEvent::overlayText}, std::to_string(intValue));
                    if (value<shortestPath.second){
                        shortestPath = {nit->first, value};
                        leadingVertex = (*it);
                    }
                }
            }
        }


        if (shortestPath.first==nullptr){
            return {};
        }
        visitedVertexes.push_back(shortestPath.first);
        visited[bitsetIndex(shortestPath.first)]=1;
        djikstraValues[shortestPath.first] = shortestPath.second;
        std::vector<std::pair<int,int>> leadingVertexPath = pathTowardsVertex[leadingVertex];
        int newDirectionRow  = (shortestPath.first->getTile()->getRow())-leadingVertex->getTile()->getRow();
        int newDirectionColumn  = (shortestPath.first->getTile()->getColumn()-leadingVertex->getTile()->getColumn());
        leadingVertexPath.push_back({newDirectionRow, newDirectionColumn});
        pathTowardsVertex[shortestPath.first] = leadingVertexPath;
        int v = shortestPath.second;
        std::string vv = std::to_string(v);
        EventBus::transmitEvent<EventBus::AnimateTile>((shortestPath).first->getTile(), {AnimateTileEvent::colorizeTile, AnimateTileEvent::overlayText}, vv);


        if (shortestPath.first == targetVertex){
            return pathTowardsVertex[targetVertex];
        }
    }
    return {};
}

bool LevelGraph::doesVectorHasElement(std::vector<Vertex *> vector, Vertex *element)
{
    return (std::find(vector.begin(), vector.end(), element) != vector.end());
}

void LevelGraph::onTileChange(TileChangeEvent *event)
{
    switch (event->getChangeType()){
    case (TileChangeEvent::TextureChange): {;break;}
    case (TileChangeEvent::DoorStatus): {
        Vertex* newVertex = addVertex(event->getChangedTile(), 1);
        setupEdgesForVertex(newVertex);
        for (auto neighbour : newVertex->getNeighbours()){
            neighbour.first->addNeighbour(newVertex, 1);
        }
    }
    }
}

