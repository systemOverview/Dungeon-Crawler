#include "QOverLay.h"

void QOverlay::setRect(QRect r){rectn = r;setGeometry(r);}

QOverlay::QOverlay(QWidget *parent, std::map<std::pair<int,int>, QTile*> QTilesRegister) : QWidget(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    m_QTiles = QTilesRegister;
}
void QOverlay::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::red, 2));

    for (auto& arrowGroup : m_arrowGroups){
        for (auto arrow : arrowGroup.second ){
            p.setPen(QPen(m_colorGroups[arrow.colorId], 2));
            const QWidget* from = arrow.from;
            const QWidget* to = arrow.to;
            QPoint startcords = {from->rect().width()/2, from->rect().height()/2};
            QPoint start =  mapFromGlobal(from->mapToGlobal(startcords));
            QPoint endcords = {to->rect().width()/2, to->rect().height()/2};
            QPoint end = mapFromGlobal(to->mapToGlobal(endcords));
            drawArrow(p, start,end);
        }

    }

    // QPoint end = mapFromGlobal(to->mapToGlobal(to->rect().topLeft() +  QPoint(0, to->height()/2)));
    // QPoint start =  from->mapToGlobal(from->rect().topRight() +  QPoint(0, from->height()/2));


    // if (m_latestDjikstraEvent){
    //     auto map = (*m_latestDjikstraEvent).getTileToCheckedNeighboursMap();
    //     for (auto pair : map){
    //         auto from = m_QTiles[{pair.first->getCordsAsPair()}];
    //         for (auto child : pair.second){
    //             auto to = m_QTiles[{child->getCordsAsPair()}];
    //             QPoint start = from->mapTo(this, QPoint(from->width(), from->height()/2));
    //             QPoint end = to->mapTo(this, QPoint(0, to->height()/2));
    //             from->setStyleSheet("background: rgba(0,255,0,120);");
    //             to->setStyleSheet("background: rgba(0,0,255,120);");
    //             qDebug() << "Drawing arrow from" << start << "to" << end;
    //             qDebug() << "Overlay rect:" << rect();
    //             qDebug() << "Are points inside overlay?"
    //                      << rect().contains(start)
    //                      << rect().contains(end);
    //             drawArrow(p, start,end);
    //         }
    //     }

    // }
    raise();

}

void QOverlay::drawArrow(QPainter &p, QPointF from, QPointF to)
{
    p.drawLine(from, to);

    QLineF line(from, to);
    const qreal arrowSize = 12.0;
    QLineF left  = line;
    QLineF right = line;

    left.setLength(arrowSize);
    right.setLength(arrowSize);

    left.setAngle(line.angle() + 150);
    right.setAngle(line.angle() - 150);

    p.drawLine(to, to + left.p2() - left.p1());
    p.drawLine(to, to + right.p2() - right.p1());

}

void QOverlay::setQTilesRegister(std::map<std::pair<int, int>, QTile *> reg)
{
    m_QTiles = reg;
}

void QOverlay::addArrow(QWidget *from, QWidget *to, int groupId, int colorGroupId){
    m_arrowGroups[groupId].push_back({from, to, colorGroupId});
    if (m_colorGroups.count(colorGroupId)==0){
        m_colorGroups[colorGroupId] = QColor::fromRgb(Utilities::GenerateRandomHexColor());
    }
    update();
}

void QOverlay::addArrowFromCords(std::pair<int, int> from, std::pair<int, int> to, int groupId, int colorGroupId)
{
    addArrow(m_QTiles.at(from), m_QTiles.at(to), groupId, colorGroupId);

}

void QOverlay::removeArrowsByGroupId(int groupId){
    for (auto arrowIterator = m_arrowGroups.begin(); arrowIterator!=m_arrowGroups.end(); arrowIterator++ ){
        if ((*arrowIterator).first==groupId){
            m_arrowGroups.erase(arrowIterator);
            break;
        }
    }
    update();

}

bool QOverlay::doesPairExist(QWidget *first, QWidget *second)
{
    for (auto arrow : arrows){
        if (arrow.from==first && arrow.to==second){
            return true;
        }
    }
    return false;
}

void QOverlay::resizeEvent(QResizeEvent *event)
{

}


