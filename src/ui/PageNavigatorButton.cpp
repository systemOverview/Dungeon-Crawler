#include "PageNavigatorButton.h"

#include "AppPageController.h"

PageNavigatorButton::PageNavigatorButton(QWidget* assosciatedWidget, GameAction* assosciatedAction)
    : m_assosciatedWidget(assosciatedWidget)
    , m_assosciatedAction(assosciatedAction) {
    connect(this, &QPushButton::clicked, assosciatedAction, &GameAction::trigger);
    connect(this, &QPushButton::clicked, this, [this]() { emit showWidget(m_assosciatedWidget); });

    setText(assosciatedAction->text());
}
