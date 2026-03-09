#ifndef PAGENAVIGATORBUTTON_H
#define PAGENAVIGATORBUTTON_H

#include <QPushButton>

class GameAction;
class AppPageController;

// this class finishes the page navigation, an AppPageController assosciates (knows) GameActions, and a
// PageNavigatorButton assosciates each GameAction to the according widget (view).
// When the button is triggered, it tells the action to trigger, and emits a showWidget to tell the
// MainAppStateController to show the assosciated widget.
class PageNavigatorButton : public QPushButton
{
    Q_OBJECT
private:
    QWidget* m_assosciatedWidget = nullptr;
    GameAction* m_assosciatedAction = nullptr;

signals:
    void showWidget(QWidget* widget);

public:
    PageNavigatorButton(QWidget* assosciatedWidget, GameAction* assosciatedAction);
};

#endif // PAGENAVIGATORBUTTON_H
