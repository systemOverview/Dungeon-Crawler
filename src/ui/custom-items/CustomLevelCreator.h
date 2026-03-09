#ifndef CUSTOMLEVELCREATOR_H
#define CUSTOMLEVELCREATOR_H

#include <QWidget>
#include <AppPageController.h>
class QGraphicsScene;
class QGraphicsView;
class TileItem;
class GameBoardView;
class DragAndDropGameItemEvent;
class Level;
class LineConnectingItems;
class CustomLevelCreator : public QWidget, public AppPageController
{
    Q_OBJECT
private:
    static inline int CHARACTERS_COUNT = 0;
    GameBoardView* m_levelVisualizer = nullptr;
    Level* m_levelModel = nullptr;
    QWidget* m_sidebar = nullptr;


    void showTilesMap();

    void setupSidebar();
    void createTileOptions();
    void createCharactersOptions();
    bool validateLevelAndShowMessage();
private slots:
    void createLevelCustomizer();
public slots:
    void dragDropEvent(DragAndDropGameItemEvent event);
    void portalsConnected(LineConnectingItems* portalsLine);

signals:
    void finished();

public:
    CustomLevelCreator(QWidget* parent = nullptr);

// AppPageController interface
public:
    QWidget* getWidget() override { return this; }

protected:
    void createActions() override;
};

#endif // CUSTOMLEVELCREATOR_H
