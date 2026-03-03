//
// Created by MBK on 13.11.25.
//

#ifndef PRAK_DUNGEONCRAWLER_H
#define PRAK_DUNGEONCRAWLER_H
#include "FightEvent.h"
#include "GraphicalUI.h"
#include "Level.h"

class AbstractController;
class FightRound;
class DungeonCrawler : public QObject
{
    Q_OBJECT
private:
    inline static DungeonCrawler* SINGLETON_INSTANCE = nullptr;

    inline static std::array<std::string, 2> GameStrings = {"##########"
                                                            "#....$...#"
                                                            "#.S.<....#"
                                                            "#..___.1.#"
                                                            "#.G......#"
                                                            "#G.......#"
                                                            "#######X##"
                                                            "#..1..A..#"
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

    inline static std::map<Character*, AbstractController*> CHARACTERS_CONTROLLERS = {};
    inline static std::vector<Character*> CHARACTERS;
    inline static std::map<Coordinates, Character*> CHARACTERS_TILES = {};

    // Move validation functions.
    static bool IsTileInNeighbouringRange(Tile* from, Tile* to);
    static bool IsTileToTileMovementLegal(Tile* from, Tile* to);
    static Character* WhoIsOccupyingTile(Tile* tile);
    static bool AreCharactersEnemies(Character* firstCharacter, Character* secondCharacter);
    // Fighting functions.
    [[nodiscard]] static bool AttemptForcedTileTakeover(Character* attacker, Character* defender);
    static FightRound HoldFightRound(Character* attacker, Character* defender);

public slots:
    void buildGame();
signals:
    void move(); // alerts controllers to move their characters

public:
    DungeonCrawler();
    static void ConnectGeneratedCharacter(Character* characterModel, CharacterItem* characterView);
    static void CreateCharacter(char letterRepresentingCharacterType, Tile* characterTile);
    static bool RequestMove(Character* character, Tile* wantedTile);
};

#endif //PRAK_DUNGEONCRAWLER_H
