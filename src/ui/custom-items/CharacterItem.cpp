#include "CharacterItem.h"
#include <QFile>
#include <QFinalState>
#include <QMenu>
#include <QSignalTransition>
#include <QStateMachine>
#include <QtCore/qpropertyanimation.h>
#include <QtCore/qsequentialanimationgroup.h>
#include <QtCore/qtimeline.h>
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpicture.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qgraphicssceneevent.h>
#include "CharacterAnimation.h"
#include "CharacterTile_UI_PlacementMediator.h"
#include "SpriteManager.h"
#include "TileItem.h"
#include <FightAnimation.h>
#include <Utilities.h>
#include <qapplication.h>
#include <qdrag.h>
void CharacterItem::playNextAnimation() {
    if (m_animationsQueue.empty()) {
        return;
    }

    CharacterAnimation* nextAnimation = m_animationsQueue.at(0);
    m_animationsQueue.erase(m_animationsQueue.begin());

    m_currentAnimation = nextAnimation;

    nextAnimation->start();
}

void CharacterItem::updateOrientation(QPointF oldPos, QPointF newPos) {
    if (newPos.x() > oldPos.x()) {
        m_characterOrientation.horizontalFlip = 1;
    }
    else if (newPos.x() < oldPos.x()) {
        m_characterOrientation.horizontalFlip = -1;
    }
    else {
        m_characterOrientation.horizontalFlip = 1;
    }

    if (newPos.y() > oldPos.y()) {
        m_characterOrientation.verticalRotation = 90;
    }
    else if (newPos.y() < oldPos.y()) {
        m_characterOrientation.verticalRotation = -90;
    }
    else {
        m_characterOrientation.verticalRotation = 0;
    }
}

int CharacterItem::getCharacterID() const { return m_characterID; }

void CharacterItem::setDefaultParts() {
    for (int i = 0; i < int(CharacterItem::CharacterPart::PAST_ENUM_END); i++) {
        m_partsGraphicOptions.insert({CharacterItem::CharacterPart(i), 0});
    }
}

QPixmap CharacterItem::getPixmap() const {
    QPixmap base = QPixmap();

    for (const auto& [part, partOption] : m_partsGraphicOptions) {
        QPixmap pixmap = SpriteManager::GetFrameFromSprite(m_characterType,
                                                           part,
                                                           partOption,
                                                           m_currentFrameId);
        if (pixmap.isNull()) {
            continue;
        }
        if (part == CharacterPart::Base) {
            base = pixmap;
            Utilities::SaveToFile(pixmap, "base");
        }
        else {
            QPainter painter = QPainter(&base);
            painter.drawPixmap(base.rect(), pixmap);
        }
    }
    return base;
}

void CharacterItem::assignPart(CharacterItem::CharacterPart partType, int whichGraphicsOption) {
    m_partsGraphicOptions.insert_or_assign(partType, whichGraphicsOption);
}

void CharacterItem::addAnimationToQueue(CharacterAnimation* animation) {
    m_animationsQueue.push_back(animation);

    if (m_animationsQueue.size() == 1) {
        playNextAnimation();
    }
    connect(animation, &CharacterAnimation::finished, [this, animation]() {
        delete animation;
        m_currentFrameId = STANDING_FRAME;
        m_characterOrientation.reset();
        update();
        playNextAnimation();
    });
}

CharacterItem::CharacterItem(Types::CharacterType characterType,
                             int characterID,
                             Mode mode,
                             bool isHealthbarShown)
    : m_characterID{characterID}
    , m_characterType{characterType}
    , m_isHealthbarShown{isHealthbarShown}
    , GameItem(mode) {
    setDefaultParts();
}

Types::CharacterType CharacterItem::getCharacterType() const { return m_characterType; }

TileItem* CharacterItem::getTile() const { return m_tile; }

void CharacterItem::setTile(TileItem* tile) { m_tile = tile; }

void CharacterItem::fixMyPosition() {}

void CharacterItem::updatePosition(QPointF newPosition) {
    updateOrientation(pos(), newPosition);
    setPos(newPosition);
    update();
}

void CharacterItem::paint(QPainter* painter,
                          const QStyleOptionGraphicsItem* option,
                          QWidget* widget) {
    QTransform flip;
    flip.scale(m_characterOrientation.horizontalFlip, 1);
    flip.rotate(m_characterOrientation.verticalRotation);

    QPixmap pixmap = getPixmap();
    QImage img = pixmap.toImage();
    SpriteManager::TrimTransparent(img);
    pixmap = QPixmap::fromImage(img);

    if (m_flipOnPositionUpdate) {
        pixmap = pixmap.transformed(flip);
    }

    float characterHeight = m_sideLength;

    if (m_isHealthbarShown) {
        characterHeight = m_sideLength * 80 / 100;
        QPixmap healthBarBackground(GUIPaths::HealthBarBackground);
        QPixmap healthBarInner(GUIPaths::HealthBarInner);

        healthBarBackground = healthBarBackground.transformed(flip);
        healthBarInner = healthBarInner.transformed(flip);

        QPainter healthPainter(&healthBarBackground);

        QRectF fillableHealthBarBackground = healthBarBackground.rect();
        fillableHealthBarBackground.setWidth(healthBarBackground.width() * m_healthPercentage / 100);
        healthPainter.drawPixmap(fillableHealthBarBackground, healthBarInner, healthBarInner.rect());
        painter->drawPixmap(QRectF(0, 0, boundingRect().width(), m_sideLength - characterHeight),
                            healthBarBackground,
                            healthBarBackground.rect());
    }

    painter->drawPixmap(QRectF{0,
                               m_sideLength - characterHeight,
                               boundingRect().width(),
                               characterHeight},
                        pixmap,
                        pixmap.rect());
}

std::map<CharacterItem::CharacterPart, int> CharacterItem::getPartsGraphicsOptions() const {
    return m_partsGraphicOptions;
}

void CharacterItem::setPartsGraphics(
    std::map<CharacterItem::CharacterPart, int> partsGraphicsOptions) {
    m_partsGraphicOptions = partsGraphicsOptions;
}

int CharacterItem::getCurrentFrameID() const { return m_currentFrameId; }

void CharacterItem::setCurrentFrameID(int frameId) {
    m_currentFrameId = frameId;
    update();
}

void CharacterItem::setHealthPercentage(float newHealthPercentage) {
    m_healthPercentage = newHealthPercentage;
    update();
}

//Debugging

CharacterItem::~CharacterItem() {
    delete m_currentAnimation;
    for (auto it = m_animationsQueue.begin(); it != m_animationsQueue.end(); it++) {
        delete *it;
        *it = nullptr;
        it = m_animationsQueue.erase(it);
    }
}

void CharacterItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (m_mode == Mode::DragAndDropInitiator) {
            event->setAccepted(true);
            return;
        }
    }
    QGraphicsItem::mousePressEvent(event);
}
