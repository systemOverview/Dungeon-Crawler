#include "MainWindow.h"
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QStackedWidget>
#include <QTextEdit>
#include <Qlabel>
#include <QtCore/qtimer.h>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qtoolbar.h>
#include <QtWidgets/qtoolbutton.h>
#include "ButtonItem.h"
#include "LoopingAnimation.h"
#include "SpriteManager.h"
#include <CustomLevelCreator.h>
#include <TileItem.h>

void MainWindow::makeStartScreen() {
    QWidget* central = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout();
    central->setLayout(mainLayout);

    mainLayout->addWidget(m_view);

    m_sidebar = new QWidget();
    m_sidebarLayout = new QVBoxLayout();
    m_sidebar->setLayout(m_sidebarLayout);

    m_sidebarToolBox = new QToolBox;
    m_sidebarLayout->addWidget(m_sidebarToolBox);

    mainLayout->addWidget(m_sidebar);

    m_character = new CharacterItem(Types::CharacterType::Human, -1);
    m_character->addAnimationToQueue(new LoopingAnimation(m_character));

    createCharacterCustomizationOptions();
    m_sidebarToolBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_sidebar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    mainLayout->setStretch(0, 2);
    mainLayout->setStretch(1, 1);

    ButtonItem* startButton = new ButtonItem(QPixmap(GUIPaths::StartButton),
                                             ButtonItem::ButtonType::StartButton);
    connect(startButton, &ButtonItem::buttonClicked, this, &MainWindow::startGame);
    startButton->setParentItem(m_character);
    startButton->setPos((m_character->boundingRect().width() / 2)
                            - startButton->boundingRect().width() / 2,
                        m_character->boundingRect().height() + 20);
    m_scene->addItem(m_character);

    setCentralWidget(central);
}

void MainWindow::createCharacterCustomizationOptions() {
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
            QLabel l;
            counter++;
        }
        m_sidebarToolBox->addItem(options, "Choose");
    }
}


qreal MainWindow::calculateTextureDimension() {
    return std::min(m_view->rect().width() / GameSettings::TILES_PER_SIDE,
                    m_view->rect().height() / GameSettings::TILES_PER_SIDE);
}

std::map<CharacterItem::CharacterPart, int> MainWindow::getHumanPartsGraphics() const {
    return m_humanPartsGraphics;
}

void MainWindow::characterCustomizationClicked(CharacterItem::CharacterPart characterPart,
                                               int whichOption) {
    m_character->assignPart(characterPart, whichOption);
}

void MainWindow::startGame() {
    m_humanPartsGraphics = m_character->getPartsGraphicsOptions();
    centralWidget()->layout()->removeWidget(m_sidebarToolBox);
    m_scene->clear();
    m_sidebarToolBox->hide();
    emit gameStarted();
    return;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    QPushButton* customizeLevelButton = new QPushButton("Create custom level");
}


bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Resize) {
        QGraphicsView* view = qobject_cast<QGraphicsView*>(obj);
        if (view) {
            emit gameItemsSideLengthChanged(calculateTextureDimension());
        }
        m_scene->setSceneRect(m_scene->itemsBoundingRect());
    }
    return false;
}

MainWindow::~MainWindow() {}
