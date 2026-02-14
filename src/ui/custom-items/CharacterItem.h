#ifndef CHARACTERITEM_H
#define CHARACTERITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QtCore/qobject.h>
#include <QtStateMachine/qstatemachine.h>
#include "CharacterAnimation.h"
#include "Constants.h"

class CharacterItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    // The character only stores the current frame id (the same for every part)
    // and the user-chosen graphics option for each part. When painting, it requests the images for each part from the
    // sprite manager, which has a cache system in place to avoid re-generating images.
    int m_currentFrameId = 0;
    std::map<CharacterGraphics::CharacterPart, int> m_partsGraphicOptions = {};
    qreal m_size = 300;

    QStateMachine* m_stateMachine = nullptr;
    QStateMachine* m_animationMachine = nullptr;

    QSequentialAnimationGroup* m_customizationAnimationLoop = nullptr;
    void setDefaultParts();

    void setupStateMachine();
    void setupAnimationMachine();

    void startAnimationLoop();

public slots:
    void resize(qreal newTileLength);
    void assignPart(CharacterGraphics::CharacterPart partType, int whichGraphicsOption);
    void moveTo(QPointF newPos);
signals:

public:
    enum { Type = UserType + 2 };
    int type() const override { return Type; }
    CharacterItem();
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void setCurrentFrameId(int frameId);
};

#endif // CHARACTERITEM_H
