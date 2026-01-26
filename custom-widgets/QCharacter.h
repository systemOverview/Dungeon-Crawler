#ifndef QCHARACTER_H
#define QCHARACTER_H
#include <QWidget>
#include "Character.h"
#include "Observer.h"
#include "EventBus.h"
#include "Subject.h"
class QCharacter : public Subject, public EventListener
{
private:
    Character* m_character; // The character it represents.
    QString m_texturePath;
    float m_healthPercentage;
    void paintEvent(QPaintEvent* event);


public:
    QCharacter(Character* character);
    QString getTexturePath();
    void recalculateHealthPercentage();
    QLinearGradient getHealthBarGradient(int width); // the tile width, the health bar must have full width of the tile
    void onCharacterHealthChange(CharacterHealthChangeEvent* event) override;
    ~QCharacter();
    Character *getCharacter() const;
};

class StaticQCharacter : public QWidget{ // for the explainer character
    QString m_texturePath;

public:
    StaticQCharacter(QString texturePath);
    void paintEvent(QPaintEvent* event);

};
#endif // QCHARACTER_H
