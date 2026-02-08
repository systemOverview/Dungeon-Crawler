#ifndef QGRAPHMATRIX_H
#define QGRAPHMATRIX_H
#include <QWidget>
class QGraphMatrixElement : public QWidget{
    friend class QGraphMatrix; // so it can access the state (wow you dont need forward declaration for friend classes.)

protected:
    enum DjikstraState{
        // it does not know about Djikstra at all, but I found naming the enum and its options according to the practical use more fitting.
        NotCalculated, // default state for all widgets
        Calculated, // widget was already visited by djikstra and its shortest path and distance are calculated.
        Blocked, // widget will never be calculated or reached, i.e: wall.
        Visualized, // Unique color to visualize a certain element

        DJIKSTRASTATE_NR_ITEMS,
    };

private:
    DjikstraState m_djikstraState = NotCalculated;
    std::pair<bool, DjikstraState> m_djikstraVisualizationState; // needed for visualizeElement and UNvisualizeElement
    // first is "is element visualized right now?", second is what was it's state before visualization.

    QString m_text = "";
    inline static std::map<DjikstraState, Qt::GlobalColor> ColorForState = {
            {NotCalculated , Qt::GlobalColor::transparent},
            {Calculated , Qt::GlobalColor::green},
            {Blocked, Qt::GlobalColor::red},
            {Visualized, Qt::GlobalColor::cyan}
    };

protected:
    QGraphMatrixElement(); // does not need a a parent.
    void setDjikstraState(DjikstraState djikstraState);
    void paintEvent(QPaintEvent* event) override;
    void setText(QString text);
    QString getText() const;
    void resetElement();
    void visualizeElement();
    void UNvisualizeElement();



};

class QGraphMatrix : public QWidget
{
    Q_OBJECT;
private:
    std::map<std::pair<int,int>, QGraphMatrixElement*> m_elementsRegister;
public:
    using DjikstraState = QGraphMatrixElement::DjikstraState;
    explicit QGraphMatrix(QWidget* parent = nullptr);
    void initializeMatrix(std::vector<std::pair<int,int>> reachableTilesCords, std::string textForReachable, int milliSecsWaitBetweenElements = 0 );
    void setElementText(std::pair<int,int> elementCords, std::string text);
    void setElementState(std::pair<int,int> elementCords, DjikstraState djikstraState);
    void visualizeElement(std::pair<int,int> elementCords, int visualizationDurationsMilliseconds);
    void setMultipleElementsState(std::vector<std::pair<int,int>> labelsCords, DjikstraState djikstraState);
    void setTextlessElementsStatusToBlocked();
    void generateMatrixLegends(QWidget* LegendContainer);
    void resetMatrix();



};


#endif // QGRAPHMATRIX_H
