#ifndef LEVELGRAPH_H
#define LEVELGRAPH_H
#include <map>
#include <vector>
#include "tile.h"

class Vertex{
private:
    Tile* m_tile;
    float m_weight;
    std::vector<Vertex*> m_neighbours;
public:
    Vertex(Tile* tile, float weight=1);
    void addNeighbour(Vertex* neighbourToAdd);
    Tile* getTile();
    void setTile(Tile* newTile);
    float getWeight();
    std::vector<Vertex*> getNeighbours();
};

class LevelGraph
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
};

#endif // LEVELGRAPH_H
