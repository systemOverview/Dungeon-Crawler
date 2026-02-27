//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_DUNGEONCRAWLER_H
#define PRAK_DUNGEONCRAWLER_H
#include "GraphicalUI.h"
#include "Level.h"
#include "List.tpp"
class StartScreen;
class DungeonCrawler : public QObject
{
    Q_OBJECT
private:
    inline static DungeonCrawler* SINGLETON_INSTANCE = nullptr;
    std::vector<Level*>::iterator currentLevel;
    List<Level*> m_levels;
    std::string m_gameString = "##########"
                               "#........#"
                               "#...<....#"
                               "#..___...#"
                               "#........#"
                               "#........#"
                               "#######.##"
                               "#........#"
                               "#........#"
                               "##########";

    std::array<std::string, 2> gameStrings = {"##########"
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
    GraphicalUI* m_GUI;
    StartScreen* m_startScreen = nullptr;
    int m_numberOfRemainingNPCs = 0;
    Level* m_lastLevel = nullptr;
    static bool IsTileInNeighbouringRange(Tile* from, Tile* to);
public slots:
    void buildGame();
signals:
    void move(); // alerts controllers to move their characters

public:
    enum GameSourceOption{
        FromStrings,
        FromFile
    };

    DungeonCrawler();
    static void ConnectGeneratedCharacter(Character* characterModel, CharacterItem* characterView);
    static bool ValidateMove(Tile* from, Tile* to);
    //old:
    std::pair<int, int> translateMove(int step) const;
    bool turn();
    void levelUp();
    void moveCharacter(Character* character, Tile* tile, bool giveway = false);
    //an attacker can ask the other NPCs to switch spots if he wants to access a tile controlled by them
    // as the other NPCs don't know a path towards the character but move in predetermined paths

    Tile* getEffectiveTile(Tile* currentTile, Tile* destinationTile);
    // When moving to tile B, a player can actually land on tile C instead (portals).
    // This function finds the actual tile the player would land on to be able to schedule resolution fights.

    void holdFight(Character* attacker, Character* defender, Tile* disputedTile);
    // attacker is the one that is trying to enter a tile already held by the defender.
    // it deletes the loser and updates the tile and the winner, or keeps things as they were if it ended in a draw.
    void build(GameSourceOption option);

};

#endif //PRAK_DUNGEONCRAWLER_H
