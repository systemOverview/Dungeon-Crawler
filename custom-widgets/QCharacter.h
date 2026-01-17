#ifndef QCHARACTER_H
#define QCHARACTER_H
#include <QWidget>
#include "Character.h"
#include "Observer.h"
class QCharacter : public Subject, public Observer
{
private:
    Character* m_character; // The character it represents.
    QString m_texturePath;
    float m_healthPercentage;
    void paintEvent(QPaintEvent* event);


public:
    QCharacter(Character* character);
    QString getTexturePath();
    QLinearGradient getHealthBarGradient(int width); // the tile width, the health bar must have full width of the tile
    void reactToChange(std::string changedMemberName="") override;
    ~QCharacter();
};

#endif // QCHARACTER_H
