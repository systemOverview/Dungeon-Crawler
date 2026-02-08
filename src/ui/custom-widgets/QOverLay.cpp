#include "QOverLay.h"

void QOverlay::setRect(QRect r){rectn = r;setGeometry(r);}


QOverlay::QOverlay(QWidget *parent, std::map<std::pair<int,int>, QTile*> QTilesRegister) : QWidget(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    m_QTiles = QTilesRegister;
    m_colorGroups[999] = Qt::red; // for highlighting a path;
}
void QOverlay::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::red, 2));



    for (Edge* edge : m_edges ){
        p.setPen(QPen(m_colorGroups[edge->groupId], edge->arrowWidth));
        drawEdge(p, edge);
    }
    raise();

}

void QOverlay::drawEdge(QPainter &p, Edge* edge)
{
    if (edge->draw==false){
        return;
    }
    const QWidget* from = m_QTiles.at(edge->fromTileCords);
    const QWidget* to = m_QTiles.at(edge->toTileCords);
    QPoint startcords = {from->rect().width()/2, from->rect().height()/2};
    QPoint start =  mapFromGlobal(from->mapToGlobal(startcords));
    QPoint endcords = {to->rect().width()/2, to->rect().height()/2};
    QPoint end = mapFromGlobal(to->mapToGlobal(endcords));

    p.drawLine(start,end);
    if (edge->edgeType == Edge::Line){return; }


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

void QOverlay::addEdge(std::pair<int,int> from, std::pair<int,int>  to, int groupId, int colorGroupId, Edge::EdgeType edgeType, int pathId){
    if (m_colorGroups.count(colorGroupId)==0){
        m_colorGroups[colorGroupId] = QColor::fromRgb(Utilities::GenerateRandomHexColor());
    }

    for (Edge* edge : m_edges){
        if (edge->fromTileCords == from && edge->toTileCords==to){
            edge->edgeType = edgeType;
            edge->groupId = groupId;
            edge->pathIDs.push_back(pathId);

            // m_edges.push_back({edge}); // here
            return;
        }
    }
    // if it gets here, then the edge doesn't exist yet.

    Edge* edge = new Edge{from, to, edgeType, groupId, pathId};
    m_edges.push_back({edge});

    update();
}

void QOverlay::addArrowPathBetweenMultipleTiles(std::vector<std::pair<int, int> > cordsOfTraversedTiles, bool flag)
{
    int pathId = PATHID++;
    if (cordsOfTraversedTiles.empty()){
        return;
    }
    std::pair<int,int> targetTileCords = cordsOfTraversedTiles.back();
    m_pathIdRegister[targetTileCords] = pathId;
    std::pair<int,int> sentinelOutOfRangeValue = {-1,-1};
    std::pair<int,int> fromWidgetCords = sentinelOutOfRangeValue;
    std::pair<int,int> toWidgetCords = sentinelOutOfRangeValue;
    int counter = 0;
    for (std::pair<int,int> cord : cordsOfTraversedTiles ){ // from and to start at {-1,-1}
        if ((counter++)==0 && flag){continue;}
        if (fromWidgetCords==sentinelOutOfRangeValue){
            fromWidgetCords = cord; // to set the first cord, will never be true again
        }
        else{
            toWidgetCords=cord; // assign it to "to",
            Edge::EdgeType edgeType = Edge::EdgeType::Line;
            if (cord == cordsOfTraversedTiles.back()){
                edgeType = Edge::EdgeType::Arrow; // to make the last widget receive an arrow head.
            }
            addEdge(fromWidgetCords, toWidgetCords, pathId, 1000, edgeType, pathId);
            fromWidgetCords = toWidgetCords; // then it becomes the "from"
        }
        // A,B,C widgets, A:from, B:to, then B:from, C:to
    }
}
void QOverlay::createPath(std::vector<std::pair<int, int> > cordsOfTraversedTiles)
{



}

void QOverlay::removeAllArrows()
{
    for (Edge* edge : m_edges){
        delete edge;
        edge = nullptr;
    }
    m_edges.clear();
    m_pathIdRegister.clear();
    m_colorGroups.clear();
    update();

}

void QOverlay::highlightArrowPathAndRemoveOthers(std::pair<int,int> pathTargetCords, std::pair<int, int> ignorePair)
{

    try{
        int pathId = m_pathIdRegister[pathTargetCords];

        for (auto edgeIterator = m_edges.begin(); edgeIterator!=m_edges.end();){
            auto edgePathIDs = (*edgeIterator)->pathIDs;
            auto edge = (*edgeIterator);
            if ((*edgeIterator)->fromTileCords==ignorePair || std::find(edgePathIDs.begin(), edgePathIDs.end(), pathId)==edgePathIDs.end()){
                delete (*edgeIterator);
                edgeIterator = m_edges.erase(edgeIterator);
                update();
            }
            else{
                (*edgeIterator)->groupId = 999;
                (*edgeIterator)->arrowWidth = 7;
                edgeIterator++;
                update();
            }
            Utilities::QtSleepMilliSeconds(10);
        }
        Utilities::QtSleepMilliSeconds(500);
    }

    catch(std::out_of_range){
     return;
    }

}

void QOverlay::removeArrowsByGroupId(int groupId){
    for (auto edgeIterator = m_edges.begin(); edgeIterator!=m_edges.end(); ){
        if ((*edgeIterator)->groupId==groupId){
            delete (*edgeIterator);
            edgeIterator = m_edges.erase(edgeIterator);
            update();
        }
        else{
            edgeIterator++;
        }
    }
    update();

}

void QOverlay::reset()
{
    removeAllArrows();
    m_QTiles.clear();
}



void QOverlay::resizeEvent(QResizeEvent *event)
{
}


