#ifndef EVENTBUS_H
#define EVENTBUS_H
#include "Event.h"

class EventBus
{

public:
    enum Type {
        CharacterDeath = 0,
        CharacterHealthChange = 1,
        AnimateTile=2,
    };

    // Used templates for these to be able to overload based on enum, in case in future i had to implement
    // two overloaded functions that only differ on the enum value. I didn't want to make the Event transmitters
    // create the special event itself by themselves and use that to overload.
    template <Type t>
    static typename std::enable_if<t==Type::CharacterDeath, void>::type transmitEvent() {
        CharacterDeathEvent::notifyListeners(new CharacterDeathEvent());
    }
    template <Type t>
    static typename std::enable_if<t==Type::CharacterHealthChange, void>::type transmitEvent() {
        CharacterHealthChangeEvent::notifyListeners(new CharacterHealthChangeEvent());
    }
    template <Type t>
    static typename std::enable_if<t==Type::AnimateTile, void>::type transmitEvent(Tile* affectedTile, std::vector<AnimateTileEvent::Visualization> visualizations, std::string textToOverlay="") {
        AnimateTileEvent::notifyListeners(new AnimateTileEvent(affectedTile, visualizations, textToOverlay));
    }
    static void subscribeToEvent(Type eventType, EventListener* listener);

    EventBus();
};

#endif // EVENTBUS_H
