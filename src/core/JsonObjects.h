#ifndef JSONOBJECTS_H
#define JSONOBJECTS_H

// #include <nlohmann/json.hpp>

#include "Level.h"

namespace JsonGenerator{
    void saveGameState(std::vector<Level *> levels);
}

#endif
