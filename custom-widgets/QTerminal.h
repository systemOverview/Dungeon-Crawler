#ifndef QTERMINAL_H
#define QTERMINAL_H

#include <QWidget>
#include <QtWidgets/qlabel.h>

class QTerminal : public QWidget
{
    Q_OBJECT
    std::vector<QString> strings;
    QLabel* label;
    QLabel* secondLabel;
    QColor color = Qt::white;
    void generateLabels();
public:
    explicit QTerminal(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void enable();
signals:
};

#endif // QTERMINAL_H
