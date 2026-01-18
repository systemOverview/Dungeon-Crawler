#ifndef LEVELGRAPH_H
#define LEVELGRAPH_H
#include <map>
#include <vector>
#include "tile.h"
#include "EventBus.h"

class Vertex{
private:
    Tile* m_tile;
    float m_weight;
    std::vector<std::pair<Vertex*, float>> m_neighbours; // vertex/weight
public:
    Vertex(Tile* tile, float weight=1);
    void addNeighbour(Vertex* neighbourToAdd, float weights);
    Tile* getTile();
    void setTile(Tile* newTile);
    float getWeight();
    std::vector<std::pair<Vertex*, float>>  getNeighbours();

};

class LevelGraph : public Subject
{
private:
    // std::vector<Vertex*> m_vertexes;
    std::map<std::pair<int,int>, Vertex*> m_vertexes; // position : vertex

public:
    LevelGraph();
    void addVertex(Tile* tile, float weight);
    void setupAlldges();
    void setupEdgesForVertex(Vertex* vertex);
    bool isEdgeBetweenTilesPossible(std::string_view firstTileTexture, std::string_view SecondTileTexture);
    Vertex* getVertex(std::pair<int,int> cords);
    std::vector<std::pair<int,int>> getShortestsPathBetweenTwoTiles(Vertex* startingVertex, Vertex* targetVertex, std::string_view algorithmToUse="");
    std::vector<std::pair<int,int>> getShortestsPathBetweenTwoTilesDjikstra(Vertex* startingVertex, Vertex* targetVertex);
    bool doesVectorHasElement(std::vector<Vertex*> vector, Vertex* element);
};

#endif // LEVELGRAPH_H
