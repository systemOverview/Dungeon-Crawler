#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QMap>
#include <QtCore/qobject.h>
#include "CharacterItem.h"
#include <sstream>
struct Coordinates
{
    int row = 0;
    int column = 0;

    bool operator==(const Coordinates& rhs) const {
        return (this->row == rhs.row && this->column == rhs.column);
    }
    bool operator!=(const Coordinates& rhs) const { return ((*this) == rhs) == false; }
    bool operator<(const Coordinates& rhs) const {
        return (this->row < rhs.row || this->column < rhs.column);
    }
    operator QString() const { return QString("Row: %1, Column : %2").arg(row).arg(column); }
};
namespace CharactersAttributes {
    struct Attributes
    {
        int strength = 0;
        int stamina = 0;
        int initialHealthPoints = 0;
        Attributes(int strengthValue, int staminaValue)
            : strength{strengthValue}
            , stamina{staminaValue} {
            initialHealthPoints = 20 + (stamina * 5);
        }
        Attributes() {};
    };

    const inline Attributes HumanAttributes{20, 20};
    const inline Attributes ZombieAttributes{30, 5};
    const inline Attributes AttackerAttributes{10, 5};

} // namespace CharactersAttributes
namespace GUIPaths {
    const inline static QString StartButton{":/gui/buttons/start.png"};
    const inline static QString ArrowUpButton{":/pics/gui/buttons/arrow-up.png"};
    const inline static QString GoblinBase{":/characters/goblin/base.png"};

    const inline static QMap<char, QString>
        TileCharToPathRegister{{'.', ":/pics/textures/floor/floor1.png"},
                               {'#', ":/pics/textures/wall/wall1.png"},
                               {'X', ":/pics/textures/doors/door1.png"},
                               {'/', ":/pics/textures/doors/door2.png"},
                               {'?', ":/pics/textures/other tiles/switch.png"},
                               {'_', ":/pics/textures/other tiles/pit.png"},
                               {'<', ":/pics/textures/other tiles/ramp.png"},
                               {'$', ":/pics/textures/extra/levelchanger.png"},
                               {'!', ":/pics/textures/other tiles/winner.png"}

        };
} // namespace GUIPaths
namespace GameSettings {
    constexpr inline static int FPS = 2;
}

namespace CharacterWearables {
    enum WearableType {
        Armor,
    };

    inline std::map<CharacterItem::CharacterPart, QString> CustomizationButtonsTexts = {
              {CharacterItem::CharacterPart::Base, "Select a base color for your player!"},
              {CharacterItem::CharacterPart::Head, "You can even swap heads.."},
              {CharacterItem::CharacterPart::Outfit, "Pick an outfit for the prom"},
              {CharacterItem::CharacterPart::Weapon, "Lot of evil creatures, chose a weapon"},

    };
    inline static std::map<CharacterItem::CharacterPart, QString> HUMAN_SPRITE_PATH_BASE
        = {{CharacterItem::CharacterPart::Base, ":/characters/human/base/"},
           {CharacterItem::CharacterPart::Head, ":/characters/human/head/"},
           {CharacterItem::CharacterPart::Outfit, ":/characters/human/outfit/"},
           {CharacterItem::CharacterPart::Weapon, ""}};

    inline static std::map<CharacterItem::CharacterPart, QString> GOBLIN_SPRITE_PATH_BASE = {
              {CharacterItem::CharacterPart::Base, "://characters/goblin/base/"},
    };
    extern std::map<WearableType, QString> WEARABLES_CUT_PATH_BASE;
    extern std::map<WearableType, QString> WEARABLES_SPRITE_PATH_BASE;
} // namespace CharacterWearables

namespace DjikstraStrings {
    enum DjikstraStringID {
        Intro,
        SetStartingToZero,
        CreateQueue,
        RemoveStartingFromQueue,
        ExplainDjikstraValues
    };
    inline std::map<DjikstraStringID, std::string> InitialSetup = {
              {Intro,
               "Djikstra's algorithm goal is to find the shortest path to all nodes. "
               "We start by giving each node an initial distance of ∞, signifying an unreached "
               "node"},
              {SetStartingToZero, "Then we set the starting vertex distance to 0. "},
              {CreateQueue, "Then we create a queue and just add all nodes in the graph to it. "},
    };

    inline std::map<DjikstraStringID, std::string> Explainers
        = {{RemoveStartingFromQueue,
            "Now that we have everything setup, we start the algorithm. We extract "
            "from the queue, the node that has the minimum Djikstra value, which at start would be"
            " the starting node.  "},
           {ExplainDjikstraValues,
            "Then we loop through the neighbours. For each neighbour, we calculate a simple "
            "equation. "
            "Djikstra value = The Djikstra value of the node that led to it + the weight between "
            "them. If It is smaller than the current value,"
            "update the assigned Djikstra value, otherwise keep it as it is."}

    };
    inline float StringReadingTime(std::string string) {
        // Returns how many seconds should the GUI wait after showing text before displaying the next text.
        constexpr float averageReadingSpeedPerMinute = 250; // measured in words
        std::stringstream stream(string);                   //
        float numberOfWords = std::distance(std::istream_iterator<std::string>(stream),
                                            std::istream_iterator<std::string>());
        return ((numberOfWords / averageReadingSpeedPerMinute) * 60);
    }
} // namespace DjikstraStrings

namespace QtVisualiation { // all in milliseconds
    // inline static int TypeWriterSleepTimeBetweenCharacters =  0;
    // inline static int SleepTimeAfterText = 0;
    // inline static int SleepTimeBetweenArrows = 0;
    // inline static int SleepTimeAfterArrowRemoval = 0;
    inline static int TypeWriterSleepTimeBetweenCharacters = 100;
    inline static int SleepTimeAfterText = 300;
    inline static int SleepTimeBetweenArrows = 0;
    inline static int SleepTimeAfterArrowRemoval = 0;
} // namespace QtVisualiation

namespace GameData {
    inline static std::array<std::string, 2> GameStrings = {

              "##########"
              "#....$P..#"
              "#3A.<....#"
              "#..___...#"
              "#5.___...#"
              "#.4....4.#"
              "#######.##"
              "#...?..3.#"
              "#..5.....#"
              "##########",

              "##########"
              "#........#"
              "#...<....#"
              "#..___...#"
              "#..___...#"
              "#........#"
              "##########"
              "#P.......#"
              "#........#"
              "##########"

    };
}

#endif // CONSTANTS_H
