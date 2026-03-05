#ifndef CUSTOMLEVELCREATOR_H
#define CUSTOMLEVELCREATOR_H

#include <QWidget>
class QGraphicsScene;
class QGraphicsView;
class TileItem;
class GameBoardView;
class DragAndDropGameItemEvent;
class CustomLevelCreator : public QWidget
{
    Q_OBJECT
private:
    static inline int CHARACTERS_COUNT = 0;
    GameBoardView* m_levelVisualizer = nullptr;
    QWidget* m_sidebar = nullptr;

    void createLevelCustomizer();

    void showTilesMap();

    void setupSidebar();
    void createTileOptions();
    void createCharactersOptions();
public slots:
    void characterDroppedOnTile(DragAndDropGameItemEvent event);

public:
    CustomLevelCreator(QWidget* parent = nullptr);
};

#endif // CUSTOMLEVELCREATOR_H
