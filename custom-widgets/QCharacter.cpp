#include "QCharacter.h"
#include <QStyleOption>
#include <QtGui/qpainter.h>
#include <sstream>

QCharacter::QCharacter(QWidget *parent, Character* character)
    : QWidget{parent}, m_character(character)
{
    std::ostringstream style;
    style << "border-image : url(:" << character->getTexturePath()
          << ") 20 0 0 0 stretch stretch;";
    QString bgStyleFull = QString::fromStdString(style.str());
    setStyleSheet(bgStyleFull);
    setFixedSize(50, 40);
    move(0, 10);
    float healthPercentage= (static_cast<float>(m_character->getCurrentHP())
                              / static_cast<float>(m_character->getMaxHP()))
                             * 100;
    m_healthBar = new QHealthBar(nullptr, healthPercentage);
    character->registerObserver(this);
    character->setQCharacter(this);

}



QHealthBar *QCharacter::getHealthBar()
{
    return m_healthBar;
}

void QCharacter::update(std::string changedMemberName)
{
    if (changedMemberName=="hitPoints"){
        float healthPercentage= (static_cast<float>(m_character->getCurrentHP())
               / static_cast<float>(m_character->getMaxHP()))
              * 100;
        m_healthBar->updateHealthBarPercentage(healthPercentage);
    }

    else if (changedMemberName=="isAlive"){
        delete this;
    }
}

QCharacter::~QCharacter()
{
    delete m_healthBar;
    // m_character->removeObserver(this);
}

void QCharacter::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

