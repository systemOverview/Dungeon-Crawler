#ifndef QOVERLAY_H
#define QOVERLAY_H
#include <QWidget>
#include <QtGui/qpainter.h>
#include "EventBus.h"
#include "QTile.h"
#include "Utilities.h"
struct ArrowBetweenTiles{
    QWidget* from;
    QWidget* to;
    int colorId;
};

class QOverlay : public QWidget {
private:
    QRect rectn;
    std::map<std::pair<int,int>, QTile*> m_QTiles = {};
    std::vector<ArrowBetweenTiles> arrows;
    std::map<int, std::vector<ArrowBetweenTiles>> m_arrowGroups; // arrows belong to a group id, this simplifies mass but specific deletion of arrows.
    std::map<int, QColor>   m_colorGroups; // every group has a color pen;
public:
    QOverlay(QWidget* parent, std::map<std::pair<int,int>, QTile*>  QTilesRegister);
    void setRect(QRect r);
    void paintEvent(QPaintEvent*) override;
    void drawArrow(QPainter& p, QPointF from, QPointF to);
    void setQTilesRegister(std::map<std::pair<int,int>, QTile*>);
    void addArrow(QWidget* from, QWidget* to,  int groupId, int colorGroupId);
    void addArrowFromCords(std::pair<int,int> from, std::pair<int,int> to, int groupId, int colorGroupId);
    void removeArrowsByGroupId(int groupId);
    bool doesPairExist(QWidget* first, QWidget* second);
    void resizeEvent(QResizeEvent* event) override;

};




#endif // QOVERLAY_H
