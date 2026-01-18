#ifndef QTILE_H
#define QTILE_H
#include <QWidget>
#include <QtWidgets/qgridlayout.h>
#include "tile.h"
#include "QCharacter.h"
#include "Observer.h"
#include <QGraphicsColorizeEffect>
class QTile : public QWidget, public Observer
{
    Q_OBJECT
private:
    Tile* m_tile; // The tile it represents, QTile is a view, and m_tile is the model.
    QString m_texturePath;
    QCharacter* m_QCharacter; // The character on that tile.
    QGridLayout* m_gameBoard;
    std::string m_textOverlay; //for algorithms visualization;
    inline static std::vector<QTile*> TemporarelyAlteredTiles; // stores qtiles whom style changed temporarly for algorithm visualizations
public:
    QTile(QWidget* parent, Tile* tile, QGridLayout* gameBoard);
    QCharacter* getQCharacter();
    void setQCharacter(QCharacter* QChar);
    std::pair<QRect, QRect> getRects();
    void paintEvent(QPaintEvent* event); // QWidget subclasses must have this function explicity included to use stylesheets.
    void reactToChange(std::string changedMemberName="") override;
    void changeStyleTemporarly();
    void colorize();
    void configurePainterForTextOverlay(QPainter* painter);
    std::string getTextOverlay();
    void setTextOverlay(std::string text);
    static void addTemporarelyAlteredTiles(QTile* Qtile);
    static void removeEffectFromTemporarelyAlteredTiles();


};


#endif // QTILE_H
