#include "LevelGraph.h"
#include <algorithm>
#include <qDebug>
#include <random>
#include <ranges>

Vertex::Vertex(Tile* tile, float weight)
    : m_tile{tile}
    , m_weight{weight} {};

void Vertex::addNeighbour(Vertex* neighbourToAdd, float weight) {
    m_neighbours.push_back({neighbourToAdd, weight});
}

Tile* Vertex::getTile() { return m_tile; }

void Vertex::setTile(Tile* newTile) { m_tile = newTile; }

float Vertex::getWeight() { return m_weight; }

std::vector<std::pair<Vertex*, float>> Vertex::getNeighbours() { return m_neighbours; }

bool Vertex::isNeighbour(Vertex* vertex) {
    for (auto neighbour : m_neighbours) {
        if (neighbour.first == vertex) {
            return true;
        }
    }
    return false;
}

float Vertex::getNeighbourWeight(Vertex* vertex) {
    for (auto neighbour : m_neighbours) {
        if (neighbour.first == vertex) {
            return neighbour.second;
        }
    }
    assert("Checking for unexisting neighbour in Djikstra.");
    return 0;
}

// graph definitions

LevelGraph::LevelGraph() { EventBus::subscribeToEvent<EventBus::TileChange>(this); }

Vertex* LevelGraph::addVertex(Tile* tile, float weight) {
    Vertex* newVertex = new Vertex(tile, weight);
    Coordinates cords = tile->getCoordinates();
    m_vertexes[cords] = newVertex;
    return newVertex;
}
void LevelGraph::setupAlldges() {
    for (auto it = m_vertexes.begin(); it != m_vertexes.end(); it++) {
        setupEdgesForVertex((*it).second);
    }
}

void LevelGraph::setupEdgesForVertex(Vertex* vertex) {
    Tile* tile = vertex->getTile();
    std::vector<Coordinates> diagonalNeighbours;
    std::vector<Coordinates> cardinalNeighbours;
    bool isDiagonal = false;

    for (int i = tile->getRow() - 1; i <= tile->getRow() + 1; i++) {
        for (int j = tile->getColumn() - 1; j <= tile->getColumn() + 1; j++) {
            isDiagonal = !isDiagonal;

            if ((i != tile->getRow() || j != tile->getColumn()) && m_vertexes.count({i, j})) {
                if (isDiagonal) {
                    diagonalNeighbours.push_back({i, j});
                }
                else {
                    cardinalNeighbours.push_back({i, j});
                }
            }
        }
    }

    for (Coordinates cords : diagonalNeighbours) {
        Vertex* neighbourVertex = m_vertexes[cords];
        Tile* neighbourTile = neighbourVertex->getTile();
        if (isEdgeBetweenTilesPossible(tile->getTexture(), neighbourTile->getTexture())) {
            vertex->addNeighbour(neighbourVertex,
                                 1.4); // very precise square root of 2
        }
    }

    for (Coordinates cords : cardinalNeighbours) {
        Vertex* neighbourVertex = m_vertexes[cords];
        Tile* neighbourTile = neighbourVertex->getTile();

        if (isEdgeBetweenTilesPossible(tile->getTexture(), neighbourTile->getTexture())) {
            vertex->addNeighbour(neighbourVertex, 1);
        }
    }

    Portal* portal = dynamic_cast<Portal*>(tile);
    if (portal) {
        vertex->addNeighbour(m_vertexes[portal->getSiblingPortal()->getCoordinates()], 1);
    }
}

bool LevelGraph::isEdgeBetweenTilesPossible(char firstTileTexture, char secondTileTexture) const {
    if (secondTileTexture == '#' || secondTileTexture == '$'
        || secondTileTexture == 'X') { // treat level changer and closed doors as not a possible path
        return false;
    }
    else if (firstTileTexture == '_'
             && (secondTileTexture != '<'
                 && secondTileTexture != '_')) { // character at pit can only move to pit or ramp
        return false;
    }
    return true;
}

Vertex* LevelGraph::getVertex(Coordinates cords) {
    if (m_vertexes.count(cords)) {
        return m_vertexes[cords];
    }
    else {
        return nullptr;
    }
}

std::vector<Coordinates> LevelGraph::getShortestsPathBetweenTwoTiles(
    Vertex* startingVertex, Vertex* targetVertex, std::string_view algorithmToUse) {
    return getShortestsPathBetweenTwoTilesDjikstra(startingVertex, targetVertex);
}

std::vector<Coordinates> LevelGraph::getShortestsPathBetweenTwoTilesDjikstra(Vertex* startingVertex,
                                                                             Vertex* targetVertex) {
    // utils and non-djikstra related (events).
    std::vector<DjikstraSearchEvent::Loop> loops;
    std::vector<Coordinates> startingSearchRange; // The cords of the vertexes we are going to search
    // through, which is basically cords of m_vertexes,
    // but to keep encapsulation and sepearation of
    // concerns, other components don't need to know
    // about the vertexes themselves.
    constexpr Coordinates UNEXISTING_TILE = {-1, 1};
    // initialization phase
    std::map<Vertex*, float> distanceRegister;
    auto compare = [&distanceRegister](Vertex* leftVertex, Vertex* rightVertex) {
        return (distanceRegister[leftVertex] < distanceRegister[rightVertex]);
    };
    std::map<Coordinates, Coordinates>
        previousRegister; // Tile N2 is the neighbour that led to finding Tile N1.
    std::vector<Vertex*> queue;

    for (auto const& [position, vertex] : m_vertexes) {
        startingSearchRange.push_back(position); // utils.
        distanceRegister[vertex] = std::numeric_limits<float>::infinity();
        previousRegister[vertex->getTile()->getCoordinates()] = UNEXISTING_TILE; // sentinel value
        // UNEXISTING_TILE is {-1,-1} and signifies an out of range Tile, ie : no
        // previous vertex
        //(if this doesn't get updated by the end of the algorithm, it is either its
        //a starting vertex, or one that is not reached).
        queue.push_back(vertex);
    }
    distanceRegister[startingVertex] = 0;

    // node selection phase

    while (!queue.empty()) {
        DjikstraSearchEvent::Loop loop;
        auto minimumVertexIterator = std::min_element(queue.begin(),
                                                      queue.end(),
                                                      compare); // iterator
        Vertex* minimumVertex = *(minimumVertexIterator);
        queue.erase(minimumVertexIterator);
        loop.setExtractedTile(minimumVertex->getTile()->getCoordinates()); // EVENT RELATED.

        if (minimumVertex == targetVertex) {
            loop.setPreviousRegisterAtLoopEnd(previousRegister); // EVENT RELATED
            loops.push_back(loop);                               // EVENT RELATED.
            break;
        }
        // distance calculation and relaxation
        for (auto vertex : queue) {
            if (minimumVertex->isNeighbour(vertex)) {
                float newDjikstraValue = distanceRegister[minimumVertex]
                                         + minimumVertex->getNeighbourWeight(vertex)
                                         + vertex->getTile()->getDjikstraExtraCost();
                if (newDjikstraValue < distanceRegister[vertex]) {
                    distanceRegister[vertex] = newDjikstraValue;
                    loop.addNeighbourTile({vertex->getTile()->getCoordinates(),
                                           newDjikstraValue,
                                           true}); // EVENT RELATED.
                    previousRegister[vertex->getTile()->getCoordinates()] = minimumVertex->getTile()
                                                                                ->getCoordinates();
                }
                else {
                    loop.addNeighbourTile({vertex->getTile()->getCoordinates(),
                                           newDjikstraValue,
                                           false}); // EVENT RELATED.
                }
            }
        }
        loop.setPreviousRegisterAtLoopEnd(previousRegister);
        loops.push_back(loop); // EVENT RELATED.
    }

    std::vector<Tile*> GIVETOGUI;
    EventBus::transmitEvent<EventBus::DjikstraSearch>(loops,
                                                      startingSearchRange,
                                                      startingVertex->getTile()->getCoordinates(),
                                                      targetVertex->getTile()->getCoordinates());
    return generatePathFromPreviousRegister(previousRegister,
                                            targetVertex->getTile()->getCoordinates());
}

std::vector<Coordinates> LevelGraph::generatePathFromPreviousRegister(
    std::map<Coordinates, Coordinates>& previousRegister,
    Coordinates itemToLookupPath,
    PathDirection pathDirection,
    PathCoordinateSystem pathCoordinateSystem) {
    constexpr Coordinates UNEXISTING_TILE = {-1, 1}; // sentinel value
    std::vector<Coordinates> path;
    Coordinates currentVertexCords = itemToLookupPath;
    Coordinates previousVertexCords = previousRegister[currentVertexCords];
    if (pathCoordinateSystem == Absolute) {
        path.push_back(currentVertexCords);
    }
    while (previousVertexCords != UNEXISTING_TILE) {
        switch (pathCoordinateSystem) {
        case Relative: {
            Tile* currentTile = m_vertexes[currentVertexCords]->getTile();
            Tile* previousTile = m_vertexes[previousVertexCords]->getTile();
            if (dynamic_cast<Portal*>(currentTile) && dynamic_cast<Portal*>(previousTile)
                && currentTile->getTexture() == previousTile->getTexture()) {
                path.push_back({0, 0});
                break;
            }
            int newDirectionRow = (currentVertexCords.row - previousVertexCords.row);
            int newDirectionColumn = (currentVertexCords.column - previousVertexCords.column);
            path.push_back({newDirectionRow, newDirectionColumn});
            break;
        }
        case Absolute: {
            path.push_back(previousVertexCords);
            break;
        }
        default:
            throw std::invalid_argument("Unhandled path coordinate system. ");
            break;
        }
        currentVertexCords = previousVertexCords;
        previousVertexCords = previousRegister[previousVertexCords];
    }
    // The register stores the previous tile cords, so if starting is 0 and target
    // is n, n refers to n-1, n-1 refers to n-2 and so on, which means that the
    // path from the while loop would by default be from target to starting,
    // depending on the pathDirection flag, we either reverse it or leave it as it
    // is.

    switch (pathDirection) {
    case FromTargetToStarting:
        return path;
    case FromStartingToTarget:
        std::reverse(path.begin(), path.end());
        return path;
    default:
        throw std::invalid_argument("Unhandled direction path. ");
    }
}

bool LevelGraph::doesVectorHasElement(std::vector<Vertex*> vector, Vertex* element) {
    return (std::find(vector.begin(), vector.end(), element) != vector.end());
}

void LevelGraph::onTileChange(TileChangeEvent* event) {
    switch (event->getChangeType()) {
    case (TileChangeEvent::TextureChange): {
        ;
        break;
    }
    case (TileChangeEvent::DoorStatus): {
        Vertex* newVertex = addVertex(event->getChangedTile(), 1);
        setupEdgesForVertex(newVertex);
        for (auto neighbour : newVertex->getNeighbours()) {
            neighbour.first->addNeighbour(newVertex, 1);
        }
    }
    }
}

LevelGraph::~LevelGraph() {
    for (auto [cords, vertex] : m_vertexes) {
        delete vertex;
        vertex = nullptr;
    }
    m_vertexes.clear();
}
