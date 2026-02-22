#ifndef QOVERLAY_H
#define QOVERLAY_H
#include <QWidget>
#include <QtGui/qpainter.h>
#include "QTile.h"
#include "Utilities.h"
struct Edge{
    enum EdgeType {
        Arrow,
        Line,
    };
    Coordinates fromTileCords;
    Coordinates toTileCords;
    EdgeType edgeType;
    int groupId;
    std::vector<int> pathIDs = {};
    bool draw = true;
    int arrowWidth = 4;
    Edge(Coordinates from, Coordinates to, EdgeType type, int groupid, int pathId)
        : fromTileCords{from}
        , toTileCords{to}
        , edgeType(type)
        , groupId(groupid) {
        pathIDs.push_back(pathId);
    };
};

class QOverlay : public QWidget {
private:
    inline static int PATHID = 10000;
    inline static int WaitTimeBetweenArrowRemovals = 50; // milliseconds
    QRect rectn;
    std::map<Coordinates, QTile*> m_QTiles = {};
    std::vector<Edge*> m_edges;
    std::map<int, QColor>   m_colorGroups; // every group has a color pen;
    std::map<Coordinates, int> m_pathIdRegister;
    bool doesEdgeExist(Coordinates from, Coordinates to, int groupId);
    void createPath(std::vector<Coordinates> cordsOfTraversedTiles);

public:
    QOverlay(QWidget* parent, std::map<Coordinates, QTile*> QTilesRegister);
    void setRect(QRect r);
    void paintEvent(QPaintEvent*) override;
    void drawEdge(QPainter& painter, Edge* edge);
    void setQTilesRegister(std::map<Coordinates, QTile*>);
    void addEdge(Coordinates from,
                 Coordinates to,
                 int groupId,
                 int colorGroupId,
                 Edge::EdgeType edgeType = Edge::EdgeType::Arrow,
                 int pathId = 0);
    void addArrowPathBetweenMultipleTiles(std::vector<Coordinates> cordsOfTraversedTiles,
                                          bool flag = false); //TODO, refactor this hack
    void highlightArrowPathAndRemoveOthers(Coordinates pathTargetCords, Coordinates ignorePair);
    void removeAllArrows();
    void removeArrowsByGroupId(int groupId);
    void reset();
    void resizeEvent(QResizeEvent* event) override;

};




#endif // QOVERLAY_H
