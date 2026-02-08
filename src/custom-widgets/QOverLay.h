#ifndef QOVERLAY_H
#define QOVERLAY_H
#include <QWidget>
#include <QtGui/qpainter.h>
#include "QTile.h"
#include "Utilities.h"
#include "events/EventBus.h"
struct Edge{
    enum EdgeType {
        Arrow,
        Line,
    };
    std::pair<int,int> fromTileCords;
    std::pair<int,int> toTileCords;
    EdgeType edgeType;
    int groupId;
    std::vector<int> pathIDs = {};
    bool draw = true;
    int arrowWidth = 4;
    Edge(std::pair<int,int> from, std::pair<int,int> to, EdgeType type,  int groupid, int pathId)
        : fromTileCords{from}, toTileCords{to}, edgeType(type), groupId(groupid)
    {
        pathIDs.push_back(pathId);
    };
};

class QOverlay : public QWidget {
private:
    inline static int PATHID = 10000;
    inline static int WaitTimeBetweenArrowRemovals = 50; // milliseconds
    QRect rectn;
    std::map<std::pair<int,int>, QTile*> m_QTiles = {};
    std::vector<Edge*> m_edges;
    std::map<int, QColor>   m_colorGroups; // every group has a color pen;
    std::map<std::pair<int,int>, int> m_pathIdRegister;
    bool doesEdgeExist(std::pair<int,int> from, std::pair<int,int> to, int groupId);
    void createPath(std::vector<std::pair<int,int>> cordsOfTraversedTiles);
public:
    QOverlay(QWidget* parent, std::map<std::pair<int,int>, QTile*>  QTilesRegister);
    void setRect(QRect r);
    void paintEvent(QPaintEvent*) override;
    void drawEdge(QPainter& painter, Edge* edge);
    void setQTilesRegister(std::map<std::pair<int,int>, QTile*>);
    void addEdge(std::pair<int,int> from, std::pair<int,int>  to, int groupId, int colorGroupId, Edge::EdgeType edgeType = Edge::EdgeType::Arrow, int pathId = 0);
    void addArrowPathBetweenMultipleTiles(std::vector<std::pair<int,int>> cordsOfTraversedTiles, bool flag = false); //TODO, refactor this hack
    void highlightArrowPathAndRemoveOthers(std::pair<int,int> pathTargetCords, std::pair<int,int> ignorePair);
    void removeAllArrows();
    void removeArrowsByGroupId(int groupId);
    void reset();
    void resizeEvent(QResizeEvent* event) override;

};




#endif // QOVERLAY_H
