#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QtCore/qobject.h>
#include <sstream>

namespace DjikstraStrings{
    enum DjikstraStringID {
    Intro,
    SetStartingToZero,
    CreateQueue,
    RemoveStartingFromQueue,
    ExplainDjikstraValues
};
    inline std::map<DjikstraStringID, std::string>  InitialSetup ={
            {Intro,
                    "Djikstra's algorithm goal is to find the shortest path to all nodes. "
                    "We start by giving each node an initial distance of ∞, signifying an unreached node"
            },
            {SetStartingToZero , "Then we set the starting vertex distance to 0. "},
               {CreateQueue, "Then we create a queue and just add all nodes in the graph to it. "},
               };

    inline std::map<DjikstraStringID, std::string>  Explainers ={
            {RemoveStartingFromQueue, "Now that we have everything setup, we start the algorithm. We extract "
                                      "from the queue, the node that has the minimum Djikstra value, which at start would be"
                                      " the starting node.  "},
            {ExplainDjikstraValues,
                    "Then we loop through the neighbours. For each neighbour, we calculate a simple equation. "
                    "Djikstra value = The Djikstra value of the node that led to it + the weight between them. If It is smaller than the current value,"
                    "update the assigned Djikstra value, otherwise keep it as it is."
            }


    };
    inline float StringReadingTime(std::string string){
        // Returns how many seconds should the GUI wait after showing text before displaying the next text.
        constexpr float averageReadingSpeedPerMinute = 250; // measured in words
        std::stringstream stream(string);//
        float numberOfWords = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
        return ((numberOfWords/averageReadingSpeedPerMinute)*60);
    }
}


namespace QtVisualiation { // all in milliseconds
// inline static int TypeWriterSleepTimeBetweenCharacters =  0;
// inline static int SleepTimeAfterText = 0;
// inline static int SleepTimeBetweenArrows = 0;
// inline static int SleepTimeAfterArrowRemoval = 0;
inline static int TypeWriterSleepTimeBetweenCharacters =  100;
inline static int SleepTimeAfterText = 300;
inline static int SleepTimeBetweenArrows = 0;
inline static int SleepTimeAfterArrowRemoval = 0;
}

namespace GameData{
    inline static std::string DataFilePath = "/Users/mbk/prak/gamedata/level.json";
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
