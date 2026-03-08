#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QMap>
#include <QtCore/qobject.h>
#include "Types.h"
#include <qstandardpaths.h>
#include <sstream>
struct Coordinates
{
public:
    struct OverflowFlags
    {
        bool rowOverFlow = false;
        bool columnOverFlow = false;

        OverflowFlags() {};
    };

private:
    friend Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs);
    inline static OverflowFlags
        DefaultOverFlow; // just to use as a default argument for CoordinatesSumClamped

public:
    int row = 0;
    int column = 0;

    bool operator==(const Coordinates& rhs) const;
    bool operator!=(const Coordinates& rhs) const;
    bool operator<(const Coordinates& rhs) const;
    operator QString() const;
    static Coordinates CoordinatesSumClamped(const Coordinates& lhs,
                                             const Coordinates& rhs,
                                             int maxRow,
                                             int maxColumn,
                                             OverflowFlags& overflowFlags = DefaultOverFlow);
    friend Coordinates operator+(const Coordinates& lhs, const Coordinates::OverflowFlags& rhs);
};
namespace CharactersAttributes {
    struct Attributes
    {
        int strength = 0;
        int stamina = 0;
        int healthPoints = 0;
        Attributes(int strengthValue, int staminaValue)
            : strength{strengthValue}
            , stamina{staminaValue} {
            healthPoints = 20 + (stamina * 5);
        }
        Attributes() {};
    };

    const inline Attributes HumanAttributes{20, 20};
    const inline Attributes ZombieAttributes{30, 5};
    const inline Attributes AttackerAttributes{10, 5};

} // namespace CharactersAttributes
// namespace CharactersAttributes
namespace GUIPaths {
    const inline static QString StartButton{":/gui/buttons/start.png"};
    const inline static QString ArrowUpButton{":/pics/gui/buttons/arrow-up.png"};

    const inline static QMap<char, QString> TileCharToPathRegister{
             {'.', ":/pics/textures/floor/floor1.png"},
             {'#', ":/pics/textures/wall/wall1.png"},
             {'X', ":/pics/textures/doors/door1.png"},
             {'/', ":/pics/textures/doors/door2.png"},
             {'?', ":/pics/textures/other tiles/switch.png"},
             {'_', ":/pics/textures/other tiles/pit.png"},
             {'<', ":/pics/textures/other tiles/ramp.png"},
             {'$', ":/pics/textures/extra/levelchanger.png"},
             {'!', ":/pics/textures/other tiles/winner.png"},
             {'1', ":/pics/textures/portal/portal1.png"},
             {'2', ":/pics/textures/portal/portal2.png"},
             {'3', ":/pics/textures/portal/portal3.png"},

    };

    const inline static std::map<Types::TileType, QString> TileTypeToPathRegister{
            {Types::TileType::Floor, ":/pics/textures/floor/floor1.png"},
            {Types::TileType::Wall, ":/pics/textures/wall/wall1.png"},
            {Types::TileType::ClosedDoor, ":/pics/textures/doors/door1.png"},
            {Types::TileType::OpenDoor, ":/pics/textures/doors/door2.png"},
            {Types::TileType::Switch, ":/pics/textures/other tiles/switch.png"},
            {Types::TileType::Pit, ":/pics/textures/other tiles/pit.png"},
            {Types::TileType::Ramp, ":/pics/textures/other tiles/ramp.png"},
            {Types::TileType::LevelChanger, ":/pics/textures/extra/levelchanger.png"},
            {Types::TileType::GameWinner, ":/pics/textures/other tiles/winner.png"},
            {Types::TileType::Portal, ":/pics/textures/portal/portal1.png"}

    };
    const extern std::map<int, QString> PortalIdToPathRegister;

    const inline static QString HealthBarBackground(":/characters/healthbar/background.png");
    const inline static QString HealthBarInner(":/characters/healthbar/bar.png");

} // namespace GUIPaths
namespace GameSettings {
    constexpr inline static int FRAMES_PER_SECOND = 1;
    constexpr inline static int TILES_PER_SIDE = 10;
} // namespace GameSettings

class DataPaths
{
public:
    const inline static QString WritableLocationCustomLevelPostfix = "/custom-level.json";
    const inline static QString DefaultLevelPath = ":/default/default-level.json";

    static const QString GetCustomLevelPath() {
        QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        return (writableLocation + WritableLocationCustomLevelPostfix);
    }
};

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
    inline static std::array<std::string, 2> GameStrings = {"##########"
                                                            "#....$...#"
                                                            "#.S.<....#"
                                                            "#..___.&.#"
                                                            "#.G......#"
                                                            "#G.......#"
                                                            "#######X##"
                                                            "#..&..A..#"
                                                            "#.H......#"
                                                            "##########",
                                                            "##########"
                                                            "#....!...#"
                                                            "#1A.<..S.#"
                                                            "#..___...#"
                                                            "#0.___...#"
                                                            "#........#"
                                                            "#######X##"
                                                            "#.G.?..1.#"
                                                            "#H.0...S.#"
                                                            "##########"};
}

#endif // CONSTANTS_H
