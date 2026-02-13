#include "CharacterItem.h"
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>
CharacterItem::CharacterItem() {
    m_timer = new QTimer();
    m_timer->setTimerType(Qt::PreciseTimer);
}

void CharacterItem::resize(qreal newTileLength) { m_size = newTileLength; }

void CharacterItem::animateMove(QPointF newPos) {
    update();
    if (newPos == pos()) return;
    if (m_lastStep == 1) {
        m_startingPos = pos().x();
    }
    m_counter += 1;
    setPos(pos().x() + 1, pos().y());
    // QTimer::singleShot(animateMove(newPos))
    if (m_counter == 9) {
        m_counter = 1;
    };
    QTimer::singleShot(100, this, [this, newPos]() { animateMove(newPos); });
    qDebug() << pos() << newPos;
}

void CharacterItem::assignWearable(CharacterWearables::WearableType wearableType, int wearableID) {
    QString wearablePath(CharacterWearables::WEARABLES_SPRITE_PATH_BASE.at(wearableType)
                         + QString::number(wearableID) + ".png"); // make it a util func
    QPixmap wearablePixmap(wearablePath);
    wearablePixmap = wearablePixmap.copy(0, 0, 46, 50);
    m_wearables.insert_or_assign(wearableType, wearablePixmap);
    update();
}

QRectF CharacterItem::boundingRect() const { return {0, 0, m_size, m_size}; }

void CharacterItem::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem* option,
                          QWidget* widget) {
    QPixmap p(m_texutreBase);
    p = p.copy(0, 0, 46, 50);
    painter->drawPixmap(boundingRect(), p, p.rect());
    for (auto [wearableType, wearablePixMap] : m_wearables) {
        painter->drawPixmap(boundingRect(), wearablePixMap, wearablePixMap.rect());
    }
}
