#include "QCharacter.h"
#include <QStyleOption>
#include <QtGui/qpainter.h>

Character *QCharacter::getCharacter() const
{
    return m_character;
}

QCharacter::QCharacter(Character* character)
    : m_character(character)
{
    m_texturePath = QString::fromStdString(character->getTexturePath());
    m_healthPercentage= (static_cast<float>(m_character->getCurrentHP())
                              / static_cast<float>(m_character->getMaxHP()));
    EventBus::subscribeToEvent<EventBus::CharacterHealthChange>(this,character);
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


void QCharacter::onCharacterHealthChange(CharacterHealthChangeEvent *event)
{
    assert(m_character!=nullptr && "Character is deleted but still attempted to access.");
    assert(event->getCharacter()==m_character && "QCharacter subscribed to the wrong character." );
    m_healthPercentage= (static_cast<float>(m_character->getCurrentHP())
                          / static_cast<float>(m_character->getMaxHP()));
    if (m_healthPercentage<=0){
        EventBus::transmitEvent<EventBus::QCharacterChange>(this, QCharacterChangeEvent::death);
        delete this;
    }
    else{
        EventBus::transmitEvent<EventBus::QCharacterChange>(this, QCharacterChangeEvent::healthbar);
    }
}

QCharacter::~QCharacter()
{
}

// Start of StaticQCharacter definitions.


StaticQCharacter::StaticQCharacter(QString texturePath) : m_texturePath{texturePath}
{
    setFixedSize(70,70);
}

void StaticQCharacter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QImage textureImage (m_texturePath);

    QRect rect = this->rect();
    painter.drawImage(rect, textureImage);
}
