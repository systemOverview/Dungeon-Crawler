#include "QTile.h"
#include <QStyleOption>
#include <QtGui/qpainter.h>
#include <sstream>

// TODO : Fix healthbar not showing when first drawn
// TODO : Fix healthbar disappearing on fight
// TODO : Fix stationary doesn't have a health bar
// TODO : Fix fighting in a pit
QTile::QTile(QWidget* parent, Tile* tile, QGridLayout* gameBoard) : QWidget(parent), m_tile{tile}, m_gameBoard(gameBoard){
    tile->registerObserver(this);
    setMinimumSize(50, 50);
    setFixedSize(50, 50);
    updateStyleSheetTexture();
    if (tile->hasCharacter()) {
        m_character = new QCharacter(this, tile->getCharacter());
        m_character->show();
        gameBoard->addWidget(this, m_tile->getRow(), m_tile->getColumn());
        // m_gameBoard->addWidget(m_character->getHealthBar(), m_tile->getRow(), m_tile->getColumn(),Qt::AlignTop );


    }


}

QCharacter *QTile::getCharacter()
{
    return m_character;
}

void QTile::updateStyleSheetTexture()
{
    std::ostringstream style;
    style << "border-image : url(" << m_tile->getTexturePath()
          << ") 0 0 0 0 stretch stretch;";
    QString bgStyleFull = QString::fromStdString(style.str());
    setStyleSheet(bgStyleFull);

};


void QTile::update(std::string changedMemberName)
{
    qDebug () << changedMemberName;
    if (changedMemberName=="texture"){
        updateStyleSheetTexture();
    }
    else if (changedMemberName == "character"){
        if (m_tile->hasCharacter()){
            qDebug()<<"change";
            m_character = m_tile->getCharacter()->getQChatacter();
            m_character->setParent(this);
            m_character->show();
            m_gameBoard->addWidget(m_character->getHealthBar(), m_tile->getRow(), m_tile->getColumn(),Qt::AlignTop );

        }
        else{
            m_character=nullptr;
        }
    }
}

void QTile::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
