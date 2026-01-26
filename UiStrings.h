#ifndef UISTRINGS_H
#define UISTRINGS_H
#include <QtCore/qobject.h>
#include <sstream>

namespace DjikstraStrings{
    enum DjikstraStringID {
    Intro,
    SetStartingToZero,
    RemoveStartingFromQueue,
    ExplainDjikstraValues
};
    std::map<DjikstraStringID, std::string>  InitialSetup ={
            {Intro,
                    "Djikstra's algorithm goal is to find the shortest path to all nodes. "
                    "We start by giving each node an initial distance of ∞, signifying an unreached node"
            },
            {SetStartingToZero , "Then we set the starting vertex distance to 0. "},
    };

    std::map<DjikstraStringID, std::string>  Explainers ={
            {RemoveStartingFromQueue, "Now that we have everything setup, we start the algorithm. We extract "
                                      "from the queue, the node that has the minimum Djikstra value, which at start would be ﬁ"
                                      "the starting node.  "},
            {ExplainDjikstraValues,
                    "Then we loop through the neighbours. For each neighbour, we calculate a simple equation. "
                    "Djikstra value = The node that led to it + the weight between them. If It is smaller than the current value,"
                    "update the assigned Djikstra value, othrwise keep it as it is."
            }
    };
    float StringReadingTime(std::string string){
        // Returns how many seconds should the GUI wait after showing text before displaying the next text.
        constexpr float averageReadingSpeedPerMinute = 250; // measured in words
        std::stringstream stream(string);//
        float numberOfWords = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
        return ((numberOfWords/averageReadingSpeedPerMinute)*60);
    }
}

#endif // UISTRINGS_H
