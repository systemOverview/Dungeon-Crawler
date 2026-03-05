#include "CustomLevelCreator.h"
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qpushbutton.h>
#include "Constants.h"
#include "GameBoardView.h"
#include "JsonObjects.h"
#include "SquareGridLayout.h"
#include "TileItem.h"
#include "Types.h"
#include <MainWindow.h>
#include <qevent.h>
CustomLevelCreator::CustomLevelCreator(QWidget* parent)
    : QWidget{parent} {
    QHBoxLayout* layout = new QHBoxLayout(this);

    m_levelVisualizer = new GameBoardView();

    layout->addWidget(m_levelVisualizer->getViewWidget(), 2);

    m_sidebar = new QWidget(this);
    layout->addWidget(m_sidebar, 1);

    createLevelCustomizer();
}

void CustomLevelCreator::createLevelCustomizer() {
    showTilesMap();
    setupSidebar();
}

void CustomLevelCreator::showTilesMap() {
    m_levelModel = new Level();

    connect(m_levelModel,
            &Level::tileReplaced,
            [this](Coordinates replacedTileCoordinates, Tile* newTile) {
                m_levelVisualizer->replaceTileView(replacedTileCoordinates, newTile->getTileType());
            });

    for (int row = 0; row < m_levelModel->getTiles().size(); row++) {
        for (const std::vector<Tile*>& row : m_levelModel->getTiles()) {
            for (Tile* tile : row) {
                TileItem* tileView = m_levelVisualizer
                                         ->createTileView(Types::TileType::Floor,
                                                          tile->getCoordinates(),
                                                          TileItem::Mode::DragAndDropReceiver);
                connect(tileView,
                        &TileItem::dragAndDropGameItemEvent,
                        this,
                        &CustomLevelCreator::dragDropEvent);
            }
        }
    }
}

void CustomLevelCreator::setupSidebar() {
    qDebug() << DataPaths::GetCustomLevelPath().toStdString();

    QVBoxLayout* sidebarLayout = new QVBoxLayout(m_sidebar);
    createTileOptions();
    createCharactersOptions();

    QPushButton* saveLevel = new QPushButton("Save level", this);
    connect(saveLevel, &QPushButton::clicked, [this]() {
        JsonGenerator::SaveLevelToJson(m_levelModel, DataPaths::GetCustomLevelPath());
    });
    sidebarLayout->addWidget(saveLevel);
}

void CustomLevelCreator::createTileOptions() {
    QGraphicsScene* tileOptionsScene = new QGraphicsScene(this);
    QGraphicsView* tileOptionsView = new QGraphicsView(tileOptionsScene);

    m_sidebar->layout()->addWidget(tileOptionsView);
    SquareGridLayout* tileOptionsLayout = new SquareGridLayout(tileOptionsView);

    for (int typeIterator = 0; typeIterator < int(Types::TileType::PAST_ENUM_END); typeIterator++) {
        TileItem* tileOption = new TileItem(Types::TileType(typeIterator),
                                            0,
                                            typeIterator,
                                            GameItem::Mode::DragAndDropInitiator);
        tileOptionsLayout->addGameItem(tileOption, {typeIterator % 3, typeIterator / 3});
        connect(m_levelVisualizer,
                &GameBoardView::boardCellSizeChanged,
                tileOption,
                &TileItem::setDragAndDropPixmapSize);
    }
}

void CustomLevelCreator::createCharactersOptions() {
    QGraphicsScene* charactersOptionsScene = new QGraphicsScene(this);
    QGraphicsView* charactersOptionsView = new QGraphicsView(charactersOptionsScene);
    m_sidebar->layout()->addWidget(charactersOptionsView);
    SquareGridLayout* charactersOptionsLayout = new SquareGridLayout(charactersOptionsView);

    for (int typeIterator = 0; typeIterator < int(Types::CharacterType::PAST_ENUM_END);
         typeIterator++) {
        CharacterItem* characterOption = new CharacterItem(Types::CharacterType(typeIterator),
                                                           CHARACTERS_COUNT++,
                                                           CharacterItem::Mode::DragAndDropInitiator);
        connect(m_levelVisualizer,
                &GameBoardView::boardCellSizeChanged,
                characterOption,
                &CharacterItem::setDragAndDropPixmapSize);

        charactersOptionsLayout->addGameItem(characterOption, {typeIterator % 2, typeIterator / 2});
    }
}

void CustomLevelCreator::dragDropEvent(DragAndDropGameItemEvent event) {
    CharacterItem* draggedCharacter = dynamic_cast<CharacterItem*>(event.getDraggedItem());
    TileItem* tileDroppedOnto = dynamic_cast<TileItem*>(event.getDroppedOnItem());

    if (draggedCharacter != nullptr && tileDroppedOnto != nullptr) {
        m_levelVisualizer->createCharacterView(draggedCharacter->getCharacterType(),
                                               CHARACTERS_COUNT++,
                                               tileDroppedOnto->getCoordinates());
        return;
    }

    else {
        TileItem* draggedTile = dynamic_cast<TileItem*>(event.getDraggedItem());
        if (draggedTile != nullptr && tileDroppedOnto != nullptr) {
            m_levelModel->insertOrReplaceTile(draggedTile->getTileType(),
                                              tileDroppedOnto->getCoordinates());
        }
    }
}
