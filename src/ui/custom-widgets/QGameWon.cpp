#include "QGameWon.h"
#include "QCharacter.h"
#include "Utilities.h"
#include <QtGui/qpainter.h>
#include <QFontDatabase>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
QGameWon::QGameWon(QWidget* parent)
    : QWidget{parent}
{
    int result = QFontDatabase::addApplicationFont(":/fonts/Emulogic.ttf");
    QFont f = QFontDatabase::font("Emulogic", "normal", 40);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    label = new QLabel("");
    label->setFont(f);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: white;");
    StaticQCharacter* first = new StaticQCharacter(":/pics/textures/zombie/attacker.png");

    auto *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(first);
    buttonsLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(label);
    mainLayout->addLayout(buttonsLayout);

    if (result==-1){
        qDebug() << "Emulogic font missing.";
    }
}

void QGameWon::paintEvent(QPaintEvent *event)
{
    QPainter painter (this);
    QBrush brush(Qt::black);
    painter.fillRect(rect(), brush);
    painter.setPen(Qt::white);

}

void QGameWon::enable()
{
    QString gameOver("Game won");
    QString letters;
    for (auto c : gameOver){
        letters+=c;
        label->setText(letters);
        Utilities::QtSleepMilliSeconds(100);
    }

}
