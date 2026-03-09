#include "CharacterCustomizer.h"

#include <QButtonGroup>
#include <QToolBox>
#include <QWidget>
#include "CharacterItem.h"
#include "QGraphicsView"
#include <ButtonItem.h>
#include <LoopingAnimation.h>
#include <SpriteManager.h>
#include <qboxlayout.h>
#include <qpushbutton.h>

CharacterCustomizer::CharacterCustomizer() {
    m_widget = new QWidget;

    m_scene = new QGraphicsScene(m_widget);
    m_view = new QGraphicsView(m_scene, m_widget);
}

void CharacterCustomizer::createActions() {
    GameAction* action = new GameAction(GameAction::ActionID::CustomizeCharacter);
    connect(action, &QAction::triggered, this, &CharacterCustomizer::showCharacterCustomizer);
    action->setText("Customize your character.");
    m_actions.push_back(action);
}
void CharacterCustomizer::showCharacterCustomizer() {
    QHBoxLayout* mainLayout = new QHBoxLayout(m_widget);

    mainLayout->addWidget(m_view);

    m_sidebar = new QWidget();
    m_sidebarLayout = new QVBoxLayout();
    m_sidebar->setLayout(m_sidebarLayout);

    m_sidebarToolBox = new QToolBox;
    m_sidebarLayout->addWidget(m_sidebarToolBox);

    mainLayout->addWidget(m_sidebar);

    m_character = new CharacterItem(Types::CharacterType::Human, -100);
    m_character->addAnimationToQueue(new LoopingAnimation(m_character));

    createCharacterCustomizationOptions();
    m_sidebarToolBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_sidebar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    mainLayout->setStretch(0, 2);
    mainLayout->setStretch(1, 1);

    m_scene->addItem(m_character);
}

void CharacterCustomizer::createCharacterCustomizationOptions() {
    for (int i = 0; i < int(CharacterItem::CharacterPart::PAST_ENUM_END); i++) {
        CharacterItem::CharacterPart characterPart = CharacterItem::CharacterPart(i);
        QWidget* options = new QWidget();
        QGridLayout* optionsLayout = new QGridLayout();

        options->setLayout(optionsLayout);

        QButtonGroup* optionsButtonGroup = new QButtonGroup();
        std::vector<QPixmap> variants = SpriteManager::GetIdleFrameVariants(
            Types::CharacterType::Human,
            characterPart,
            SpriteManager::ImageProcessingMode::TrimTransparent);
        int counter = 0;
        for (QPixmap& optionImage : variants) {
            QPushButton* optionButton = new QPushButton();
            optionButton->setStyleSheet(
                "border-color: gray; border-width : 2px; background-color:white");
            optionImage = optionImage.scaledToHeight(100);

            QIcon optionIcon(optionImage);
            optionButton->setIcon(optionIcon);
            optionButton->setIconSize({50, 100});

            optionsButtonGroup->addButton(optionButton, counter);
            optionsLayout->addWidget(optionButton, counter / 3,
                                     (counter) % 3); // 3 images per row

            connect(optionButton, &QPushButton::clicked, this, [counter, characterPart, this]() {
                characterCustomizationClicked(characterPart, counter);
            });
            counter++;
        }
        m_sidebarToolBox->addItem(options, "Choose");
    }
}

void CharacterCustomizer::characterCustomizationClicked(CharacterItem::CharacterPart characterPart,
                                                        int whichOption) {
    m_character->assignPart(characterPart, whichOption);
}

QWidget* CharacterCustomizer::getWidget() { return m_widget; }
