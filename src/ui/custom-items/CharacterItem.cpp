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
CharacterItem::CharacterItem(Character::CharacterType characterType, int characterID)
    : m_characterID{characterID}
    , m_characterType{characterType}
    , GameItem(QPixmap()) {
    setDefaultParts();
    d_createMenu();
}

void CharacterItem::setHealthPercentage(float newHealthPercentage) {
    m_healthPercentage = newHealthPercentage;
    update();
}

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

void CharacterItem::setDefaultParts() {
    for (int i = 0; i < int(CharacterItem::CharacterPart::PAST_END); i++) {
        m_partsGraphicOptions.insert({CharacterItem::CharacterPart(i), 0});
    }
}

Character::CharacterType CharacterItem::getCharacterType() const { return m_characterType; }

std::map<CharacterItem::CharacterPart, int> CharacterItem::getPartsGraphicsOptions() const {
    return m_partsGraphicOptions;
}

void CharacterItem::setPartsGraphics(
    std::map<CharacterItem::CharacterPart, int> partsGraphicsOptions) {
    m_partsGraphicOptions = partsGraphicsOptions;
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

void CharacterItem::fixMyPosition() {
    if (scene()) {
        QPointF newPos = CharacterTile_UI_PlacementMediator::GetCharacterPosition(this);
        updateOrientation(pos(), newPos);
        setPos(newPos);
    }
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

    float characterHeight = SIDE_LENGTH * 80 / 100;

    pixmap = pixmap.transformed(flip);

    painter->drawPixmap(QRectF{0,
                               SIDE_LENGTH - characterHeight,
                               boundingRect().width(),
                               characterHeight},
                        pixmap,
                        pixmap.rect());

    QPixmap healthBarBackground(GUIPaths::HealthBarBackground);
    QPixmap healthBarInner(GUIPaths::HealthBarInner);

    healthBarBackground = healthBarBackground.transformed(flip);
    healthBarInner = healthBarInner.transformed(flip);

    QPainter healthPainter(&healthBarBackground);

    QRectF fillableHealthBarBackground = healthBarBackground.rect();
    fillableHealthBarBackground.setWidth(healthBarBackground.width() * m_healthPercentage / 100);
    healthPainter.drawPixmap(fillableHealthBarBackground, healthBarInner, healthBarInner.rect());

    painter->drawPixmap(QRectF(0, 0, boundingRect().width(), SIDE_LENGTH - characterHeight),
                        healthBarBackground,
                        healthBarBackground.rect());
}

int CharacterItem::getCurrentFrameID() const { return m_currentFrameId; }

void CharacterItem::setCurrentFrameID(int frameId) {
    m_currentFrameId = frameId;
    update();
}


void CharacterItem::setTile(TileItem* tile) { m_tile = tile; }

TileItem* CharacterItem::getTile() const { return m_tile; }
//Debugging

void CharacterItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    d_contextMenu->exec(event->screenPos());
}

void CharacterItem::d_createMenu() {
    d_contextMenu = new QMenu();
    QAction* d_restoreDefaultFlip = new QAction();
    d_restoreDefaultFlip->setText("Restore to default");
    d_contextMenu->addAction(d_restoreDefaultFlip);
}

CharacterItem::~CharacterItem() {
    delete m_currentAnimation;
    for (auto it = m_animationsQueue.begin(); it != m_animationsQueue.end(); it++) {
        delete *it;
        *it = nullptr;
        it = m_animationsQueue.erase(it);
    }
}
