#include "QTile.h"
#include <QStyleOption>
#include <QtGui/qpainter.h>
#include <QtWidgets/qlabel.h>
#include <QApplication>

// I am not very sure if fetching/adding a respectively unexisting/already existing tile should be an assertion or exception.
// But following the logic of the program, the tile (x,y) domain range is fixed (0,0 -> (9,9) and
// can not allow for a missing or an extra tile, so an attempt to fetch an unexisting tile or to add an existing one would
// mean that there is a flaw somewhere and even by catching it as an exception there is nothing to do about it, so an assertion
// made more sense.
QTile::QTile(QWidget* parent, Tile* tile, QGridLayout* gameBoard) : QWidget(parent), m_tile{tile}, m_gameBoard(gameBoard){
    assert(QTilesRegister.count(tile->getCordsAsPair())==0 && "QTile being constructed already exists.");
    tile->registerObserver(this);
    EventBus::subscribeToEvent<EventBus::TileChange>(this, tile);
    setMinimumSize(50, 50);
    m_texturePath = QString::fromStdString(m_tile->getTexturePath());
    if (tile->hasCharacter()) {
        setQCharacter( new QCharacter(tile->getCharacter()));
    }
    QTilesRegister[tile->getCordsAsPair()] = this;
}

QTile *QTile::getQTileByCords(std::pair<int, int> cords)
{
    assert(QTilesRegister.count(cords)!=0 && "QTile being fetched does not exist.");
    return (QTilesRegister[cords]);

}

QCharacter *QTile::getQCharacter()
{
    return m_QCharacter;
}

void QTile::setQCharacter(QCharacter *QChar)
{

    if (QChar){
        EventBus::subscribeToEvent<EventBus::QCharacterChange>(this, QChar);
    }
    if (m_QCharacter){
        EventBus::unsubscribeFromEvent<EventBus::QCharacterChange>(this, m_QCharacter);
    }
    m_QCharacter = QChar;
    auto a = m_QCharacter;
    repaint();
}
void QTile::onTileChange(TileChangeEvent *event)
{
    assert("QTile subscribed to wrong tile"&&event->getChangedTile()->getCordsAsPair()==m_tile->getCordsAsPair());
    switch (event->getChangeType()){
    case TileChangeEvent::TextureChange :{m_texturePath = QString::fromStdString(m_tile->getTexturePath());repaint();break;}
    case TileChangeEvent::DoorStatus : {break;} // QTile doesn't care about the door status, only its texture which is always reset when the door status changes, guaranteeing a TextureChange Event being transmitted.
    case TileChangeEvent::Character : {
        if (m_tile->hasCharacter()){
            setQCharacter(m_tile->getCharacter()->getQChatacter());
        }
        else{
            setQCharacter(nullptr);
        };
        return;
    }
    default : {throw std::runtime_error( "The type of the TileChange is not handled." );}
    }
}

void QTile::onQCharacterChange(QCharacterChangeEvent *event)
{
    switch(event->getChangeType()){
    case QCharacterChangeEvent::healthbar: {
        repaint();return;}
    case QCharacterChangeEvent::death: {
        setQCharacter(nullptr);
        return;
    }
    default : {throw std::runtime_error( "The type of the change is not handled." );}
    }

}

void QTile::reactToChange(std::string changedMemberName)
{
}

void QTile::markAsVisited()
{
    m_appliedEffect = new QGraphicsColorizeEffect;
    m_appliedEffect->setColor(QColor(0, 0, 255));
    m_appliedEffect->setStrength(0.6);
    setGraphicsEffect(m_appliedEffect);
    QTile::addTemporarelyAlteredTiles(this);
    graphicsEffect();
}

void QTile::unmarkAsVisited(){delete m_appliedEffect;}

void QTile::configurePainterForTextOverlay(QPainter *painter)
{
    QPen pen = painter->pen();
    pen.setColor(Qt::black);
    painter->setPen(pen);

    QFont font = painter->font();
    font.setPixelSize(48);
    painter->setFont(font);


}

std::string QTile::getTextOverlay()
{
    return m_textOverlay;
}

void QTile::setTextOverlay(std::string text)
{
    m_textOverlay = text;
    repaint();
}

void QTile::addTemporarelyAlteredTiles(QTile *Qtile)
{
    TemporarelyAlteredTiles.push_back(Qtile);
}

void QTile::removeEffectFromTemporarelyAlteredTiles()
{
    for (auto it = TemporarelyAlteredTiles.begin(); it!=TemporarelyAlteredTiles.end();){
        if ((*it)->graphicsEffect()){
        (*it)->graphicsEffect()->setEnabled(false);
        }
        it = TemporarelyAlteredTiles.erase(it);
    }
}


void QTile::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QImage textureImage (m_texturePath);

    QRect rect = this->rect();
    painter.setPen(QPen(Qt::red));
    painter.setFont(QFont("Times", 12, QFont::Bold));

    painter.drawImage(rect, textureImage);
    if (m_textOverlay!=""){
        configurePainterForTextOverlay(&painter);
        painter.drawText(rect, Qt::AlignCenter, QString::fromStdString(m_textOverlay));
    }

    if (m_QCharacter){
        assert(m_tile->getCharacter() == m_QCharacter->getCharacter() && "QCharacter differs from Character.");
        QPixmap characterPixMap (":"+m_QCharacter->getTexturePath());

        std::pair<QRect, QRect> rects = getRects();
        QRect healthRect = rects.first;
        QRect charRect = rects.second;

        QLinearGradient linearGrad = m_QCharacter->getHealthBarGradient(width());
        QBrush brush (linearGrad);

        painter.drawPixmap(charRect, characterPixMap);


        painter.fillRect(healthRect, brush);
    }

}

void QTile::mousePressEvent(QMouseEvent *event)
{
    qDebug() << m_tile->getCordsAsPair();
}

std::pair<QRect, QRect> QTile::getRects(){
    QRect healthRect = QRect(0,0,width(), 10);
    QRect charRect = QRect(0, 10, width(),height()-10);
    return {healthRect, charRect};
}
