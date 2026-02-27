//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_DUNGEONCRAWLER_H
#define PRAK_DUNGEONCRAWLER_H
#include "GraphicalUI.h"
#include "Level.h"
class AbstractController;
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

    inline static std::unordered_map<char, Character::CharacterType> CHAR_TO_TYPE_DICTIONARY
        = {{'H', Character::CharacterType::Human},
           {'A', Character::CharacterType::Attacker},
           {'G', Character::CharacterType::GuardZombie},
           {'S', Character::CharacterType::StationaryZombie}};

    inline static GraphicalUI* GUI = nullptr;

    inline static std::vector<AbstractController*> CHARACTERS_CONTROLLERS = {};

    // Move validation functions.
    static bool ValidateMove(Tile* from, Tile* to);
    static bool IsTileInNeighbouringRange(Tile* from, Tile* to);
    static AbstractController* CreateCharacterController(Character* character);

public slots:
    void buildGame();
signals:
    void move(); // alerts controllers to move their characters

public:
    DungeonCrawler();
    static void ConnectGeneratedCharacter(Character* characterModel, CharacterItem* characterView);
    static void CreateCharacter(char characterIdentifier, Tile* characterTile);
    static bool RequestMove(Character* character, Tile* tile);
};

#endif //PRAK_DUNGEONCRAWLER_H
