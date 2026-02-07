#ifndef QTILE_H
#define QTILE_H
#include <QWidget>
#include <QtWidgets/qgridlayout.h>
#include "tile.h"
#include "QCharacter.h"
#include "Observer.h"
#include <QGraphicsColorizeEffect>
#include "EventBus.h"
class QTile : public QWidget, public Observer, public EventListener
{
    Q_OBJECT
private:
    inline static int counter = 0;
    inline static std::map<std::pair<int,int>, QTile*> QTilesRegister = {};
    Tile* m_tile; // The tile it represents, QTile is a view, and m_tile is the model.
    QString m_texturePath;
    QCharacter* m_QCharacter; // The character on that tile.
    QGridLayout* m_gameBoard;
    std::string m_textOverlay; //for algorithms visualization;
    QGraphicsColorizeEffect* m_appliedEffect;
public:
    QTile(QWidget* parent, Tile* tile, QGridLayout* gameBoard);
    static QTile* getQTileByCords(std::pair<int,int> cords);
    QCharacter* getQCharacter();
    void setQCharacter(QCharacter* QChar);
    std::pair<QRect, QRect> getRects();
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void reactToChange(std::string changedMemberName="") override;
    void changeStyleTemporarly();
    void markAsVisited();
    void unmarkAsVisited();
    void configurePainterForTextOverlay(QPainter* painter);
    std::string getTextOverlay();
    void setTextOverlay(std::string text);
    static void addTemporarelyAlteredTiles(QTile* Qtile);
    static void removeEffectFromTemporarelyAlteredTiles();
    void onTileChange(TileChangeEvent* event) override;
    virtual void onQCharacterChange(QCharacterChangeEvent* event) override;
    static void deleteAllQTiles();
    ~QTile();
};


#endif // QTILE_H
