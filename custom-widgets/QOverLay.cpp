#include "QOverLay.h"

void QOverlay::setRect(QRect r){rectn = r;setGeometry(r);}

bool QOverlay::doesEdgeExist(std::pair<int, int> from, std::pair<int, int> to, int groupId)
{
    try{
        std::vector<EdgeBetweenTiles> edgeGroup = m_edgeGroups.at(groupId);
        for (EdgeBetweenTiles edge : edgeGroup){
            if (edge.fromTileCords == from && edge.toTileCords == to){return true;}
        }
        return false;
    } catch(std::out_of_range){
        return false;
    }
}

QOverlay::QOverlay(QWidget *parent, std::map<std::pair<int,int>, QTile*> QTilesRegister) : QWidget(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    m_QTiles = QTilesRegister;
}
void QOverlay::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::red, 2));


    for (auto& edgeGroup : m_edgeGroups){
        for (auto edge : edgeGroup.second ){
            p.setPen(QPen(m_colorGroups[edge.colorId], 2));
            drawEdge(p, edge);
        }
    }
    raise();

}

void QOverlay::drawEdge(QPainter &p, EdgeBetweenTiles edge)
{
    const QWidget* from = m_QTiles.at(edge.fromTileCords);
    const QWidget* to = m_QTiles.at(edge.toTileCords);
    QPoint startcords = {from->rect().width()/2, from->rect().height()/2};
    QPoint start =  mapFromGlobal(from->mapToGlobal(startcords));
    QPoint endcords = {to->rect().width()/2, to->rect().height()/2};
    QPoint end = mapFromGlobal(to->mapToGlobal(endcords));

    p.drawLine(start,end);
    if (edge.edgeType == EdgeBetweenTiles::Line){return; }


    QLineF line(start, end);
    const qreal arrowSize = 12.0;
    QLineF left  = line;
    QLineF right = line;

    left.setLength(arrowSize);
    right.setLength(arrowSize);

    left.setAngle(line.angle() + 150);
    right.setAngle(line.angle() - 150);

    p.drawLine(end, end + left.p2() - left.p1());
    p.drawLine(end, end + right.p2() - right.p1());

}

void QOverlay::setQTilesRegister(std::map<std::pair<int, int>, QTile *> reg)
{
    m_QTiles = reg;
}

void QOverlay::addEdge(std::pair<int,int> from, std::pair<int,int>  to, int groupId, int colorGroupId, EdgeBetweenTiles::EdgeType edgeType){
    if (doesEdgeExist(from, to, groupId)){
        // if the edge already exists, it only updates its edge type, this is for the final calculated tiles paths.
        // ex : draw A - > B - > C path, arrow head at C, then draw A - > B - > C -> D, it will have two arrow heads
        // one at C and one at D, so by updating B->C edge type instead of re-adding it as a line, the arrow head is removed.
        for (auto& edge : m_edgeGroups.at(groupId)){
            if (edge.fromTileCords==from && edge.toTileCords==to){
                edge.edgeType = edgeType;
            }
        }
    }
    else{
    m_edgeGroups[groupId].push_back({from, to, colorGroupId, edgeType});
    }
    if (m_colorGroups.count(colorGroupId)==0){
        m_colorGroups[colorGroupId] = QColor::fromRgb(Utilities::GenerateRandomHexColor());
    }
    update();
}

void QOverlay::addArrowBetweenMultipleTiles(std::vector<std::pair<int, int> > cordsOfTraversedTiles)
{
    if (cordsOfTraversedTiles.empty()){
        return;
    }
    std::pair<int,int> sentinelOutOfRangeValue = {-1,-1};
    std::pair<int,int> fromWidgetCords = sentinelOutOfRangeValue;
    std::pair<int,int> toWidgetCords = sentinelOutOfRangeValue;
    for (std::pair<int,int> cord : cordsOfTraversedTiles ){ // from and to start at {-1,-1}
        if (fromWidgetCords==sentinelOutOfRangeValue){
            fromWidgetCords = cord; // to set the first cord, will never be true again
        }
        else{
            toWidgetCords=cord; // assign it to "to",
            EdgeBetweenTiles::EdgeType edgeType = EdgeBetweenTiles::EdgeType::Line;
            if (cord == cordsOfTraversedTiles.back()){
                edgeType = EdgeBetweenTiles::EdgeType::Arrow; // to make the last widget receive an arrow head.
            }

            addEdge(fromWidgetCords, toWidgetCords, 1000, 1000, edgeType);
            fromWidgetCords = toWidgetCords; // then it becomes the "from"
        }
        // A,B,C widgets, A:from, B:to, then B:from, C:to
    }
}

void QOverlay::removeArrowsByGroupId(int groupId){
    for (auto arrowIterator = m_edgeGroups.begin(); arrowIterator!=m_edgeGroups.end(); arrowIterator++ ){
        if ((*arrowIterator).first==groupId){
            m_edgeGroups.erase(arrowIterator);
            break;
        }
    }
    update();

}

void QOverlay::resizeEvent(QResizeEvent *event)
{

}


