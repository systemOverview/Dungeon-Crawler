#include "QGraphMatrix.h"
#include <QtGui/qpainter.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qlabel.h>
#include <QThread>
#include "Utilities.h"

QGraphMatrix::QGraphMatrix(QWidget* parent)
    : QWidget{parent}
{

    QGridLayout* myLayout = new QGridLayout(this);
    for (int i=0; i<11; i++){
        for (int j=0; j<11; j++){
            QGraphMatrixElement* element = new QGraphMatrixElement;
            element->setFixedSize(20,20);

            if ((i==0 && j!=0)){
                // numbering is (-1) because we do not account for the element at (0,0), its just a space holder
                // this makes it also conform to the game cords system.
                element->setText(QString::fromStdString(std::to_string(j-1)));
            }
            else if (i!=0 && j==0){
                element->setText(QString::fromStdString(std::to_string(i-1)));
            }
            else{
                if (i!=0 && j!=0){
                    element->setStyleSheet("border: 1px solid black");
                    m_elementsRegister[{i-1,j-1}] = element;
                    // elementsRegister shouldn't care about the numbering elements cords.
                    // this makes it also conform to the game cords system.
                }
            }
            myLayout->addWidget(element, i ,j );
        }
    }
    assert(m_elementsRegister.size()==100 && "Elements register does not map 10*10 matrix.");
}

void QGraphMatrix::initializeMatrix(std::vector<std::pair<int, int> > reachableTilesCords, std::string textForReachable, int milliSecsWaitBetweenElements)
{
    assert(m_elementsRegister.size()==100 && "Elements register does not map 10*10 matrix.");
    assert(this->layout()!=nullptr && "QGraphMatrix does not have a layout.");
    for (int i = 0; i<10; i++){
        for (int j=0; j<10; j++){
            std::pair<int,int> cords = {i,j};
            QGraphMatrixElement* element = m_elementsRegister[{cords.first, cords.second}];
            assert(element!=nullptr && "widget in matrix is nullptr");
            if(std::find(reachableTilesCords.begin(), reachableTilesCords.end(),cords ) != reachableTilesCords.end()) {
                element->setText(QString::fromStdString(textForReachable));

            }
            else{
                element->setDjikstraState(QGraphMatrixElement::Blocked);
            }
            if (milliSecsWaitBetweenElements>0){
                Utilities::QtSleepMilliSeconds(milliSecsWaitBetweenElements);
            }
        }
    }
    for (auto cord : reachableTilesCords){
    }

}

void QGraphMatrix::setElementText(std::pair<int, int> elementCords, std::string text)
{
    assert(m_elementsRegister.count(elementCords)>0 && "QGraphMatrixElement is out of range.");
    QGraphMatrixElement* element = m_elementsRegister.at(elementCords);
    assert( element!=nullptr && "QGraphMatrixElement is nullptr.");
    element->setText(QString::fromStdString(text));


}

void QGraphMatrix::setElementState(std::pair<int, int> elementCords, DjikstraState djikstraState)
{
    assert(m_elementsRegister.count(elementCords)>0 && "QGraphMatrixElement is out of range.");
    QGraphMatrixElement* element = m_elementsRegister.at(elementCords);
    assert( element!=nullptr && "QGraphMatrixElement is nullptr.");
    element->setDjikstraState(djikstraState);

}

void QGraphMatrix::visualizeElement(std::pair<int, int> elementCords, int visualizationDurationsMilliseconds)
{
    assert(m_elementsRegister.count(elementCords)>0 && "QGraphMatrixElement is out of range.");
    QGraphMatrixElement* element = m_elementsRegister.at(elementCords);
    assert( element!=nullptr && "QGraphMatrixElement is nullptr.");
    element->visualizeElement();
    Utilities::QtSleepMilliSeconds(visualizationDurationsMilliseconds);
    element->UNvisualizeElement();
}

void QGraphMatrix::setMultipleElementsState(std::vector<std::pair<int, int> > labelsCords, DjikstraState djikstraState)
{

}

void QGraphMatrix::setTextlessElementsStatusToBlocked()
{
    for (auto const [position, element]: m_elementsRegister){
        if (element->getText()==""){
            element->setDjikstraState(QGraphMatrixElement::Blocked);
        }
    }
}

void QGraphMatrix::resetMatrix()
{
    for (auto& element  : m_elementsRegister){
        assert(element.second!=nullptr && "Element to be reset is nullptr");
        element.second->resetElement();
    }
}



// Start of QGraphMatrixElement definitions.

QGraphMatrixElement::QGraphMatrixElement(){
    assert(DJIKSTRASTATE_NR_ITEMS==ColorForState.size() && "State count differs than assigned colors count.");
};

void QGraphMatrixElement::setDjikstraState(DjikstraState djikstraState){
    assert(djikstraState!=DJIKSTRASTATE_NR_ITEMS && "djikstraState passed is counter.");
    m_djikstraState = djikstraState;
    update();
}

void QGraphMatrixElement::paintEvent(QPaintEvent *event)
{
    Qt::GlobalColor backgroundColor = ColorForState[m_djikstraState];
    QPainter painter(this);
    painter.setBrush(backgroundColor);
    painter.fillRect(rect(), painter.brush());

    if (m_text!=""){
        painter.drawText(rect(), Qt::AlignCenter, m_text);
    }

}

void QGraphMatrixElement::setText(QString text)
{
    m_text = text;
    update();
}

QString QGraphMatrixElement::getText() const
{
    return m_text;
}

void QGraphMatrixElement::resetElement()
{
    m_djikstraState = NotCalculated;
    m_text="";
}

void QGraphMatrixElement::visualizeElement()
{
    m_djikstraVisualizationState = {true, m_djikstraState};
    setDjikstraState(Visualized);
}

void QGraphMatrixElement::UNvisualizeElement()
{
    assert (m_djikstraVisualizationState.first==true && "Trying to unvisualize an already visualized element. ");
    setDjikstraState(m_djikstraVisualizationState.second);
}
