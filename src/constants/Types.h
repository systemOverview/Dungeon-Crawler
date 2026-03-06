#ifndef TYPES_H
#define TYPES_H

#include <QtCore/qobject.h>
#include <QtCore/qtmetamacros.h>

// This namespace stores the types of items as they are shard by both models and views, to avoid having to include model inside view
// or vice versa and creating coupling.
namespace Types {
    Q_NAMESPACE
    enum class TileType {
        Floor,
        Wall,
        Portal,
        OpenDoor,
        ClosedDoor,
        Switch,
        Pit,
        Ramp,
        LevelChanger,
        GameWinner,
        PAST_ENUM_END
    };

    enum class CharacterType { Human, StationaryZombie, GuardZombie, Attacker, PAST_ENUM_END };

    Q_ENUM_NS(TileType);
    Q_ENUM_NS(CharacterType);

}; // namespace Types

namespace TypesIdentifiers {
    const inline static std::map<char, Types::TileType> CHAR_TO_TILE_TYPE_LOOKUP_TABLE{
              {'.', Types::TileType::Floor},
              {'#', Types::TileType::Wall},
              {'&', Types::TileType::Portal},
              {'X', Types::TileType::ClosedDoor},
              {'/', Types::TileType::OpenDoor},
              {'?', Types::TileType::Switch},
              {'_', Types::TileType::Pit},
              {'<', Types::TileType::Ramp},
              {'$', Types::TileType::LevelChanger},
              {'!', Types::TileType::GameWinner},
    };

    const inline static std::map<char, Types::CharacterType> CHAR_TO_CHARACTER_TYPE_LOOKUP_TABLE{
              {'H', Types::CharacterType::Human},
              {'S', Types::CharacterType::StationaryZombie},
              {'G', Types::CharacterType::GuardZombie},
              {'A', Types::CharacterType::Attacker},
    };
} // namespace TypesIdentifiers

#endif // TYPES_H
