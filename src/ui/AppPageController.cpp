#include "AppPageController.h"

QList<GameAction*> AppPageController::getActions() {
    if (m_actions.count() == 0) {
        createActions();
    }
    return m_actions;
}
