#ifndef QOVERLAY_H
#define QOVERLAY_H
#include <QWidget>
#include <QtGui/qpainter.h>
#include "EventBus.h"
#include "QTile.h"
#include "Utilities.h"
struct EdgeBetweenTiles{
    enum EdgeType {
        Arrow,
        Line,
    };
    std::pair<int,int> fromTileCords;
    std::pair<int,int> toTileCords;
    int colorId;
    EdgeType edgeType;
};

class QOverlay : public QWidget {
private:
    QRect rectn;
    std::map<std::pair<int,int>, QTile*> m_QTiles = {};
    std::vector<EdgeBetweenTiles> m_edges;
    std::map<int, std::vector<EdgeBetweenTiles>> m_edgeGroups; // arrows belong to a group id, this simplifies mass but specific deletion of arrows.
    std::map<int, QColor>   m_colorGroups; // every group has a color pen;
        bool doesEdgeExist(std::pair<int,int> from, std::pair<int,int> to, int groupId);
public:
    QOverlay(QWidget* parent, std::map<std::pair<int,int>, QTile*>  QTilesRegister);
    void setRect(QRect r);
    void paintEvent(QPaintEvent*) override;
    void drawEdge(QPainter& painter, EdgeBetweenTiles edge);
    void setQTilesRegister(std::map<std::pair<int,int>, QTile*>);
    void addEdge(std::pair<int,int> from, std::pair<int,int>  to, int groupId, int colorGroupId, EdgeBetweenTiles::EdgeType edgeType = EdgeBetweenTiles::EdgeType::Arrow);
    void addArrowBetweenMultipleTiles(std::vector<std::pair<int,int>> cordsOfTraversedTiles);
    void removeArrowsByGroupId(int groupId);
    void resizeEvent(QResizeEvent* event) override;

};




#endif // QOVERLAY_H
