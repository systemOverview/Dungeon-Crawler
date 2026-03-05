#ifndef GAMELAYOUT_H
#define GAMELAYOUT_H

#include <QLayout>
#include <QObject>
#include <QtWidgets/qmainwindow.h>

class GameLayout : public QLayout
{
private:
    QList<QLayoutItem*>* m_items;

public:
    GameLayout();
    void addItem(QLayoutItem* item) override;
    bool hasHeightForWidth() const override;
    int count() const override;
    QLayoutItem* itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect& rect) override;
    QSize sizeHint() const override;
    QLayoutItem* takeAt(int index) override;
    void addWidget(QWidget* widget);
};

#endif // GAMELAYOUT_H
