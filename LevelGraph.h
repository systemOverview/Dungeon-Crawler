#ifndef LEVELGRAPH_H
#define LEVELGRAPH_H
#include <limits.h>
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
    bool isNeighbour(Vertex* vertex);
    float getNeighbourWeight(Vertex* vertex);

};

class LevelGraph :public EventListener
{
public:
    enum PathCoordinateSystem {
        Absolute,
        Relative
    };
    enum PathDirection {
        FromStartingToTarget,
        FromTargetToStarting,
    };
private:
    // std::vector<Vertex*> m_vertexes;
    std::map<std::pair<int,int>, Vertex*> m_vertexes; // position : vertex

public:
    LevelGraph();
    Vertex* addVertex(Tile* tile, float weight);
    void setupAlldges();
    void setupEdgesForVertex(Vertex* vertex);
    bool isEdgeBetweenTilesPossible(char firstTileTexture, char SecondTileTexture) const;
    Vertex* getVertex(std::pair<int,int> cords);
    std::vector<std::pair<int,int>> getShortestsPathBetweenTwoTiles(Vertex* startingVertex, Vertex* targetVertex, std::string_view algorithmToUse="");
    std::vector<std::pair<int,int>> getShortestsPathBetweenTwoTilesDjikstra(Vertex* startingVertex, Vertex* targetVertex);
    std::vector<std::pair<int,int>> generatePathFromPreviousRegister
        (std::map<std::pair<int,int>, std::pair<int,int>>& previousRegister,
        std::pair<int,int> itemToLookupPath,PathDirection pathDirection = PathDirection::FromStartingToTarget,
        PathCoordinateSystem pathCoordinateSystem = PathCoordinateSystem::Relative
        );

    bool doesVectorHasElement(std::vector<Vertex*> vector, Vertex* element);
    void onTileChange(TileChangeEvent* event) override;
    ~LevelGraph();

};

#endif // LEVELGRAPH_H
