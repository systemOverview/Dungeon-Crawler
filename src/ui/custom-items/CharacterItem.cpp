#include "CharacterItem.h"
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>
#include "SpriteManager.h"
CharacterItem::CharacterItem() {
    m_timer = new QTimer();
    m_timer->setTimerType(Qt::PreciseTimer);
    QImage base = SpriteManager::GetImageFromSprite(CharacterGraphics::CharacterPart::Base);
    m_parts.insert({CharacterGraphics::CharacterPart::Base, QPixmap::fromImage(base)});

    QImage outfit = SpriteManager::GetImageFromSprite(CharacterGraphics::CharacterPart::Outfit);
    m_parts.insert({CharacterGraphics::CharacterPart::Outfit, QPixmap::fromImage(outfit)});

    QImage head = SpriteManager::GetImageFromSprite(CharacterGraphics::CharacterPart::Head);
    m_parts.insert({CharacterGraphics::CharacterPart::Head, QPixmap::fromImage(head)});
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

void CharacterItem::assignPart(CharacterGraphics::CharacterPart partType, int partID) {
    QPixmap partPixmap = QPixmap::fromImage(SpriteManager::GetImageFromSprite(partType, partID));
    m_parts.insert_or_assign(partType, partPixmap);
    update();
}

QRectF CharacterItem::boundingRect() const { return {0, 0, m_size, m_size}; }

void CharacterItem::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem* option,
                          QWidget* widget) {
    for (const auto& [wearableType, wearablePixMap] : m_parts) {
        painter->drawPixmap(boundingRect(), wearablePixMap, wearablePixMap.rect());
    }
}
