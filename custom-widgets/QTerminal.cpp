#include "QTerminal.h"
#include <QtGui/qpainter.h>
#include <QFontDatabase>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include "Utilities.h";
#include <sstream>
void QTerminal::generateLabels()
{
    QString loadingString ("Loading changes...");
    strings.push_back(loadingString);
    for (int i=0; i<100; i++){
        QString tileSaving;
        tileSaving += "Loading ";
    }
}

QTerminal::QTerminal(QWidget* parent)
    : QWidget{parent}
{
    QVBoxLayout* layout  = new QVBoxLayout(this);
    label = new QLabel(this);
    secondLabel = new QLabel(this);
    layout->addWidget(label);
    layout->addWidget(secondLabel);
    layout->addStretch(1);
    int result = QFontDatabase::addApplicationFont(":/fonts/dejavu");
    QFont f = QFontDatabase::font("DejaVu Sans Mono", "normal", 40);
    qDebug() << "result" << result;
    f.setPixelSize(20);
    label->setFont(f);
    secondLabel->setFont(f);

}

void QTerminal::paintEvent(QPaintEvent *event)
{
    QPainter painter (this);
    QBrush brush(Qt::black);
    painter.fillRect(rect(), brush);
    painter.setPen(Qt::white);
    // painter.setFont(label->font());
    // painter.drawText(label->rect(), label->text());

    // painter.setPen(Qt::green);
    // painter.drawText(secondLabel->rect(), secondLabel->text());

}

void QTerminal::enable()
{

    label->setStyleSheet("color: white;");
    secondLabel->setStyleSheet("color: green;");

    QString loading ("dungeon crawler add -A && dungeon crawler commit -m \"Killed some zombies\"\nLoading changes... \nLoading state of tile ");
    QString letters("");
    for (auto c : loading){
        letters+=c;
        label->setText(letters);
        Utilities::QtSleepMilliSeconds(1);
        update();
    }


    for (int i=0; i<100; i++){
        letters = loading;
        QString tileSaving;
        std::stringstream stream;
        stream << i/10 <<" : "<< i%10;
        letters+=QString::fromStdString(stream.str());
        label->setText(letters);
        Utilities::QtSleepMilliSeconds(1);
        update();
    }

    secondLabel->setText("Changes saved");


}
