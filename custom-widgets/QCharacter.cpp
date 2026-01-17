#include "QCharacter.h"
#include <QStyleOption>
#include <QtGui/qpainter.h>

QCharacter::QCharacter(Character* character)
    : m_character(character)
{
    m_texturePath = QString::fromStdString(character->getTexturePath());
    m_healthPercentage= (static_cast<float>(m_character->getCurrentHP())
                              / static_cast<float>(m_character->getMaxHP()));
    character->registerObserver(this);
    character->setQCharacter(this);

}

QString QCharacter::getTexturePath()
{
    return m_texturePath;
}


QLinearGradient QCharacter::getHealthBarGradient(int width){

    float whereWhiteStarts = m_healthPercentage+0.01; // white should start strictly after red for both of them not to intermix, so i add a small amount.
    QLinearGradient linearGrad(QPointF(0, 0), QPointF(width, 0));
    linearGrad.setColorAt(0, Qt::red);
    linearGrad.setColorAt(m_healthPercentage, Qt::red);

    if (whereWhiteStarts<1){
        linearGrad.setColorAt(whereWhiteStarts, Qt::white);
        linearGrad.setColorAt(1, Qt::white);
    }
    return linearGrad;
}

void QCharacter::reactToChange(std::string changedMemberName)
{
    if (changedMemberName=="hitPoints"){
        m_healthPercentage= (static_cast<float>(m_character->getCurrentHP())
                              / static_cast<float>(m_character->getMaxHP()));
        notifyObservers("hitPoints");
    }

    else if (changedMemberName=="isAlive"){
        notifyObservers("dead character");
        delete this;
    }
}

QCharacter::~QCharacter()
{
}

