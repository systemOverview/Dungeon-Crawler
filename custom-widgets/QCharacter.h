#ifndef QCHARACTER_H
#define QCHARACTER_H
#include <QWidget>
#include "Character.h"
#include "QHealthBar.h"
#include "Observer.h"
class QCharacter : public QWidget, public Observer
{
    Q_OBJECT
private:
    Character* m_character; // The character it represents.
    QHealthBar* m_healthBar;
    void paintEvent(QPaintEvent* event); // QWidget subclasses must have this function explicity included to use stylesheets.


public:
    QCharacter(QWidget *parent, Character* character);
    QHealthBar* getHealthBar();
    void update(std::string changedMemberName="") override;
    ~QCharacter();
};

#endif // QCHARACTER_H
