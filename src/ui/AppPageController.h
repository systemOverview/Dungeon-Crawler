#ifndef APPPAGECONTROLLER_H
#define APPPAGECONTROLLER_H

#include <QObject>
#include <QStackedWidget>
#include <Qmap>

// This class is inherited by all views controllers. Each class defines its actions and the suggest text for buttons
// linking to them.
// Each actions is defined by an ID, this way navigating between pages would only need the actionID, without having
// to make classes know about each other.
// getWidget() returns the view widget of the controller.
// Actions are created on the first call to getActions() to avoid calling a pure virtual function in the constructor.

class GameAction : public QAction
{
public:
    enum ActionID { CustomizeCharacter, CustomizeLevel, LoadDefaultGame, LoadSavedGame };

protected:
    inline static QMap<ActionID, GameAction*> ACTIONS{};

private:
    ActionID m_actionID{};

public:
    GameAction(ActionID actionID, QObject* parent = nullptr)
        : QAction(parent)
        , m_actionID(actionID) {
        ACTIONS.insert(actionID, this);
    }

    inline static GameAction* GetActionById(ActionID id) { return (ACTIONS.find(id).value()); };
};

class AppPageController
{
public:

private:
protected:
    QList<GameAction*> m_actions;
    virtual void createActions() = 0;

signals:
    void showMe();

public:
    virtual QWidget* getWidget() = 0;
    QList<GameAction*> getActions();
};

#endif //APPPAGECONTROLLER_H
