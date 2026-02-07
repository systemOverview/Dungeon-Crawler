#ifndef JSONOBJECTS_H
#define JSONOBJECTS_H

// #include <nlohmann/json.hpp>

#include "Level.h"
#include "list.h"
struct TileState {
    char texture;
    int row;
    int col;
    TileState() = default;
    TileState(char txt, int r, int c) : texture{txt}, row{r}, col{c}{};
};

namespace JsonGenerator{
    void generateTiles();
    void saveGameState(std::vector<Level *> levels);
}
// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TileState, texture, row, col)

// inline void generateTiles()
// {
//     for (auto c : GameData::GameStrings) {
//         // generate TileState here
//     }
// }

#endif
