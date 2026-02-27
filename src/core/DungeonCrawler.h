//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_DUNGEONCRAWLER_H
#define PRAK_DUNGEONCRAWLER_H
#include "GraphicalUI.h"
#include "Level.h"
class DungeonCrawler : public QObject
{
    Q_OBJECT
private:
    inline static DungeonCrawler* SINGLETON_INSTANCE = nullptr;

    inline static std::array<std::string, 2> GameStrings = {"##########"
                                                            "#....$...#"
                                                            "#.S.<....#"
                                                            "#..___...#"
                                                            "#.G......#"
                                                            "#G.......#"
                                                            "#######.##"
                                                            "#.....A..#"
                                                            "#.P......#"
                                                            "##########",
                                                            "##########"
                                                            "#....!...#"
                                                            "#1A.<..S.#"
                                                            "#..___...#"
                                                            "#0.___...#"
                                                            "#........#"
                                                            "#######X##"
                                                            "#.G.?..1.#"
                                                            "#P.0...S.#"
                                                            "##########"};
    inline static GraphicalUI* GUI = nullptr;

    // Move validation functions.
    static bool ValidateMove(Tile* from, Tile* to);
    static bool IsTileInNeighbouringRange(Tile* from, Tile* to);

public slots:
    void buildGame();
signals:
    void move(); // alerts controllers to move their characters

public:
    DungeonCrawler();
    static void ConnectGeneratedCharacter(Character* characterModel, CharacterItem* characterView);
};

#endif //PRAK_DUNGEONCRAWLER_H
