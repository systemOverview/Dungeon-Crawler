#include "QTile.h"
#include <QStyleOption>
#include <QtGui/qpainter.h>

QTile::QTile(QWidget* parent, Tile* tile, QGridLayout* gameBoard) : QWidget(parent), m_tile{tile}, m_gameBoard(gameBoard){
    tile->registerObserver(this);
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

    if (changedMemberName=="texture"){
        m_texturePath = QString::fromStdString(m_tile->getTexturePath());
    }
    else if (changedMemberName=="hitPoints"){
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

    painter.drawImage(rect, textureImage);
    if (m_tile->hasCharacter()){
        QPixmap characterPixMap (":"+m_QCharacter->getTexturePath());

        std::pair<QRect, QRect> rects = getRects();
        QRect healthRect = rects.first;
        QRect charRect = rects.second;

        QLinearGradient linearGrad = m_QCharacter->getHealthBarGradient(width());
        qDebug() << linearGrad;
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
