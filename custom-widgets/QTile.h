#ifndef QTILE_H
#define QTILE_H
#include <QWidget>
#include <QtWidgets/qgridlayout.h>
#include "tile.h"
#include "QCharacter.h"
#include "Observer.h"
class QTile : public QWidget, public Observer
{
    Q_OBJECT
private:
    Tile* m_tile; // The tile it represents, QTile is a view, and m_tile is the model.
    QCharacter* m_character; // The character on that tile.
    QGridLayout* m_gameBoard;

public:
    QTile(QWidget* parent, Tile* tile, QGridLayout* gameBoard);
    QCharacter* getCharacter();
    void updateStyleSheetTexture();
    void paintEvent(QPaintEvent* event); // QWidget subclasses must have this function explicity included to use stylesheets.
    void update(std::string changedMemberName="") override;

};


#endif // QTILE_H
