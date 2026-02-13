#include "Constants.h"
using namespace CharacterWearables;
namespace CharacterWearables {
std::map<WearableType, QString> WEARABLES_CUT_PATH_BASE = {{{Armor}, ":/graphics/armor/cut/"}};
std::map<WearableType, QString> WEARABLES_SPRITE_PATH_BASE = {
         {Armor, ":/graphics/armor/sprite/"} // namespace CharacterWearables
};
} // namespace CharacterWearables
