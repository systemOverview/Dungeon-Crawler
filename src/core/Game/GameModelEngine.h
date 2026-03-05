#ifndef GAMEMODELENGINE_H
#define GAMEMODELENGINE_H

#include <QObject>

class GameController;
class AbstractController;
class Level;
class Tile;
class Character;
class FightRound;

class GameModelEngine : public QObject
{
    Q_OBJECT
private:
    void createGame();
    inline static Level* CURRENT_LEVEL = nullptr;
    GameController* m_gameController = nullptr;

    inline static std::map<Character*, AbstractController*> CHARACTERS_CONTROLLERS = {};

    static bool IsTileInNeighbouringRange(Tile* from, Tile* to);
    static bool IsTileToTileMovementLegal(Tile* from, Tile* to);
    static Character* WhoIsOccupyingTile(Tile* tile);

    static bool AreCharactersEnemies(Character* firstCharacter, Character* secondCharacter);

    // Fighting functions.
    [[nodiscard]] static bool AttemptForcedTileTakeover(Character* attacker, Character* defender);
    [[nodiscard]] static FightRound HoldFightRound(Character* attacker, Character* defender);

public:
public slots:
    void askCharactersToMove();
signals:
    void move(); // alerts controllers to move their characters

public:
    GameModelEngine();
    static bool RequestMove(Character* character, Tile* wantedTile);
    Level* getCurrentLevel();
};

#endif // GAMEMODELENGINE_H
