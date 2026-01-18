#ifndef EVENTBUS_H
#define EVENTBUS_H
#include "Event.h"

class EventBus
{

public:
    enum Type {
        CharacterDeath,
        CharacterHealthChange,
        AnimateTile,
        TileChange
    };
    // Start of notifyListeners templates
    // Used templates for these to be able to overload based on enum, in case in future i had to implement
    // two overloaded functions that only differ on the enum value. I didn't want to make the Event transmitters
    // create the special event itself by themselves and use that to overload.

    // Start of transmitEvent templates.
    // They follow this syntax : EventBus::transmitEvent<EventBus::Type>(parameters).
    template <Type eventType>
    static typename std::enable_if<eventType==Type::CharacterDeath, void>::type transmitEvent() {
        CharacterDeathEvent::notifyListeners(new CharacterDeathEvent());
    }
    template <Type eventType>
    static typename std::enable_if<eventType==Type::CharacterHealthChange, void>::type transmitEvent() {
        CharacterHealthChangeEvent::notifyListeners(new CharacterHealthChangeEvent());
    }
    template <Type eventType>
    static typename std::enable_if<eventType==Type::AnimateTile, void>::type transmitEvent(Tile* affectedTile, std::vector<AnimateTileEvent::Visualization> visualizations, std::string textToOverlay="") {
        AnimateTileEvent::notifyListeners(new AnimateTileEvent(affectedTile, visualizations, textToOverlay));
    }
    template <Type eventType>
    static typename std::enable_if<eventType==Type::TileChange, void>::type transmitEvent(Tile* affectedTile, TileChangeEvent::ChangeType changeType) {
        TileChangeEvent::notifyListeners(new TileChangeEvent(affectedTile, changeType));
    }
    // Start of subscribeToEvent templates.
    // SubscribeToEvent allows objects to subscribe to certain events, and possibly set filters depending on the event type.
    template <Type eventType>
    inline static void subscribeToEvent(EventListener* listener)
    {

        switch (eventType){
        case CharacterDeath : {CharacterDeathEvent::registerListener(listener);break;}
        case CharacterHealthChange: {CharacterHealthChangeEvent::registerListener(listener);break;}
        case AnimateTile: {AnimateTileEvent::registerListener(listener);break;}
        case TileChange : {TileChangeEvent::registerListener(listener);break;}
        }
    }

    template <Type eventType>
    static typename std::enable_if<eventType==Type::TileChange, void>::type subscribeToEvent(EventListener* eventListener, Tile* TileToListenTo) {
        TileChangeEvent::registerListener(eventListener, TileToListenTo);
    }

    template <Type eventType>
    static typename std::enable_if<eventType==Type::TileChange, void>::type subscribeToEvent(EventListener* eventListener, std::vector<Tile*> TileToListenTo) {
        TileChangeEvent::registerListener(eventListener, TileToListenTo);
    }

    EventBus();
};

#endif // EVENTBUS_H
