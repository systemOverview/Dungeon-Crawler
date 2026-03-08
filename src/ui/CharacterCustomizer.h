#ifndef CUSTOMCHARACTERCREATOR_H
#define CUSTOMCHARACTERCREATOR_H

#include <CharacterItem.h>
class QGraphicsView;
class QVBoxLayout;
class QToolBox;
class CharacterCustomizer : public QObject
{
    Q_OBJECT
private:
    QWidget* m_widget = nullptr;
    QGraphicsScene* m_scene = nullptr;
    QGraphicsView* m_view = nullptr;

    QWidget* m_sidebar = nullptr;
    QVBoxLayout* m_sidebarLayout = nullptr;
    QWidget* m_options = nullptr;
    QWidget* m_armorOptions = nullptr;

    QToolBox* m_sidebarToolBox = nullptr;

    CharacterItem* m_character;

    void createCharacterCustomizationOptions();

    void characterCustomizationClicked(CharacterItem::CharacterPart characterPart, int whichOption);

public:
    CharacterCustomizer();
    void ShowCharacterCustomizer();
    QWidget* getWidget() const;
    // std::map<CharacterItem::CharacterPart, int> getHumanPartsGraphics() const;
};

#endif // CUSTOMCHARACTERCREATOR_H
