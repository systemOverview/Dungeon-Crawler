#include "QTile.h"
#include <QStyleOption>
#include <QtGui/qpainter.h>
#include <QtWidgets/qlabel.h>

QTile::QTile(QWidget* parent, Tile* tile, QGridLayout* gameBoard) : QWidget(parent), m_tile{tile}, m_gameBoard(gameBoard){
    tile->registerObserver(this);
    EventBus::subscribeToEvent<EventBus::TileChange>(this, tile);
    setFixedSize(50, 50);
    m_texturePath = QString::fromStdString(m_tile->getTexturePath());
    if (tile->hasCharacter()) {
        setQCharacter( new QCharacter(tile->getCharacter()));
    }
}

QCharacter *QTile::getQCharacter()
{
    return m_QCharacter;
}

void QTile::setQCharacter(QCharacter *QChar)
{
    if (!QChar){
        m_QCharacter = nullptr;
        return;
    }
    if (m_QCharacter){
        m_QCharacter->removeObserver(this);
        QChar->registerObserver(this);
        m_QCharacter=QChar;

    }
    else{
        QChar->registerObserver(this);
        m_QCharacter=QChar;
    }
}

#include <QThread>

void QTile::reactToChange(std::string changedMemberName)
{

    if (changedMemberName=="hitPoints"){
        repaint(); // to update the tile graphics (the healthbar especially)
        return;
    }
    else if (changedMemberName=="dead character"){
        setQCharacter(nullptr);
    }
    else if (changedMemberName == "character"){
        if (m_tile->hasCharacter()){
            setQCharacter(m_tile->getCharacter()->getQChatacter());
        }
        else{
            setQCharacter(nullptr);
        }
    }
    update();
}

void QTile::colorize()
{
    QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect;
    effect->setColor(QColor(0, 0, 255));
    effect->setStrength(0.6);
    setGraphicsEffect(effect);
    QTile::addTemporarelyAlteredTiles(this);


}

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

void QTile::onTileChange(TileChangeEvent *event)
{
    assert("QTile subscribed to wrong tile"&&event->getChangedTile()->getCordsAsPair()==m_tile->getCordsAsPair());
    switch (event->getChangeType()){
    case TileChangeEvent::TextureChange :{m_texturePath = QString::fromStdString(m_tile->getTexturePath());repaint();break;}
    case TileChangeEvent::DoorStatus : {break;} // QTile doesn't care about the door status, only its texture which is always reset when the door status changes, guaranteeing a TextureChange Event being transmitted.
    default : {throw std::runtime_error( "The type of the TileChange is not handled." );}
    }
}

void QTile::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QImage textureImage (m_texturePath);

    QRect rect = this->rect();
    painter.setPen(QPen(Qt::red));
    painter.setFont(QFont("Times", 12, QFont::Bold));
    painter.drawText(textureImage.rect(), Qt::AlignCenter, "Text");

    painter.drawImage(rect, textureImage);
    if (m_textOverlay!=""){
        configurePainterForTextOverlay(&painter);
        painter.drawText(rect, QString::fromStdString(m_textOverlay));
    }

    if (m_tile->hasCharacter()){
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

std::pair<QRect, QRect> QTile::getRects(){
    QRect healthRect = QRect(0,0,width(), 10);
    QRect charRect = QRect(0, 10, width(),height()-10);
    return {healthRect, charRect};
}
