#ifndef EVENTBUS_H
#define EVENTBUS_H
#include "Event.h"
// new type checklist :add to Type, transmitEvent(notifyListeners),
// subscribeToEvent(if it has a custom one) , unsubscribeFromAllEvents, registerListener,
class EventBus
{

public:
    enum Type {
        CharacterHealthChange,
        AnimateTile,
        TileChange,
        QCharacterChange,
        VisualizationStatus,
        DjikstraSearch,
        PortalCreation
    };
    // Start of notifyListeners templates
    // Used templates for these to be able to overload based on enum, in case in future i had to implement
    // two overloaded functions that only differ on the enum value. I didn't want to make the Event transmitters
    // create the special event itself by themselves and use that to overload.

    // Start of transmitEvent templates.
    // They follow this syntax : EventBus::transmitEvent<EventBus::Type>(parameters).

    template <Type eventType>
    static typename std::enable_if<eventType==Type::CharacterHealthChange, void>::type transmitEvent(Character* character) {
        CharacterHealthChangeEvent::notifyListeners(new CharacterHealthChangeEvent(character));
    }
    template <Type eventType>
    static typename std::enable_if<eventType==Type::AnimateTile, void>::type transmitEvent(Tile* affectedTile, std::vector<AnimateTileEvent::Visualization> visualizations, std::string textToOverlay="") {
        AnimateTileEvent::notifyListeners(new AnimateTileEvent(affectedTile, visualizations, textToOverlay));
    }
    template <Type eventType>
    static typename std::enable_if<eventType==Type::TileChange, void>::type transmitEvent(Tile* affectedTile, TileChangeEvent::ChangeType changeType) {
        TileChangeEvent::notifyListeners(new TileChangeEvent(affectedTile, changeType));
    }
    template <Type eventType>
    static typename std::enable_if<eventType==Type::QCharacterChange, void>::type transmitEvent(QCharacter* QCharacter, QCharacterChangeEvent::ChangeType changeType) {
        QCharacterChangeEvent::notifyListeners(new QCharacterChangeEvent(QCharacter, changeType));
    }
    template <Type eventType>
    static typename std::enable_if<eventType==Type::VisualizationStatus, void>::type transmitEvent(VisualizationStatusEvent::Status status) {
        VisualizationStatusEvent::notifyListeners(new VisualizationStatusEvent(status));
    }

    template <Type eventType>
    static typename std::enable_if<eventType==Type::DjikstraSearch, void>::type transmitEvent(DjikstraSearchEvent* event) {
        DjikstraSearchEvent::notifyListeners(event);
    }

    template <Type eventType>
    static typename std::enable_if<eventType==Type::DjikstraSearch, void>::type transmitEvent(std::vector<DjikstraSearchEvent::Loop> loops, std::vector<std::pair<int,int>> startingSearchRange, std::pair<int,int> startingTileCords, std::pair<int,int> targetTileCords) {
        DjikstraSearchEvent::notifyListeners(new DjikstraSearchEvent(loops, startingSearchRange,startingTileCords,targetTileCords));
    }

    template <Type eventType>
    static typename std::enable_if<eventType==Type::PortalCreation, void>::type transmitEvent(Portal* portal) {
        PortalCreationEvent::notifyListeners(new PortalCreationEvent(portal));
    }

    //Start of unsubscribe templates.
    template <Type eventType>
    static typename std::enable_if<eventType==Type::QCharacterChange, void>::type unsubscribeFromEvent(EventListener* eventListener, QCharacter* QCharacterToStopListeningTo) {
        QCharacterChangeEvent::deregisterListener(eventListener, QCharacterToStopListeningTo);
    }
    inline static void unsubscribeFromAllEvents(EventListener* eventListener){
        CharacterHealthChangeEvent::deregisterListener(eventListener);
        AnimateTileEvent::deregisterListener(eventListener);
        TileChangeEvent::deregisterListener(eventListener);
        QCharacterChangeEvent::deregisterListener(eventListener);
        VisualizationStatusEvent::deregisterListener(eventListener);
        DjikstraSearchEvent::deregisterListener(eventListener);
        PortalCreationEvent::deregisterListener(eventListener);
    };

    // Start of subscribeToEvent(wrapper for Event::registerListener) definitions.
    // SubscribeToEvent allows objects to subscribe to certain events, and possibly set filters depending on the event type.
    // They follow this syntax :EventBus::subscribeToEvent<EventBus::type>(parameters);
    template <Type eventType>
    inline static void subscribeToEvent(EventListener* eventListener) // General template
    {
        switch (eventType){
        case CharacterHealthChange: {CharacterHealthChangeEvent::registerListener(eventListener);break;}
        case AnimateTile: {AnimateTileEvent::registerListener(eventListener);break;}
        case TileChange : {TileChangeEvent::registerListener(eventListener);break;}
        case QCharacterChange: {QCharacterChangeEvent::registerListener(eventListener);break;}
        case VisualizationStatus: {VisualizationStatusEvent::registerListener(eventListener);break;}
        case DjikstraSearch : {DjikstraSearchEvent::registerListener(eventListener); break;}
        case PortalCreation : {PortalCreationEvent::registerListener(eventListener); break;}
        default : {throw std::runtime_error( "subscribeToEvent template is of an unexisting enum value." );}
        }
    }
    //Only for event types with overloaded registerListener.
    //TileChangeEvent registerListener templates.
    template <Type eventType>
    static typename std::enable_if<eventType==Type::TileChange, void>::type subscribeToEvent(EventListener* eventListener, Tile* TileToListenTo) {
        TileChangeEvent::registerListener(eventListener, TileToListenTo);
    }

    template <Type eventType>
    static typename std::enable_if<eventType==Type::TileChange, void>::type subscribeToEvent(EventListener* eventListener, std::vector<Tile*> TilesToListenTo) {
        TileChangeEvent::registerListener(eventListener, TilesToListenTo);
    }
    //CharacterHealthChangeEvent registerListener templates.
    template <Type eventType>
    static typename std::enable_if<eventType==Type::CharacterHealthChange, void>::type subscribeToEvent(EventListener* eventListener, Character* characterToListenTo) {
        CharacterHealthChangeEvent::registerListener(eventListener, characterToListenTo);
    }

    template <Type eventType>
    static typename std::enable_if<eventType==Type::CharacterHealthChange, void>::type subscribeToEvent(EventListener* eventListener, std::vector<Character*> CharactersToListenTo) {
        CharacterHealthChangeEvent::registerListener(eventListener, CharactersToListenTo);
    }
    //QCharacterChangeEvent registerListener templates.
    template <Type eventType>
    static typename std::enable_if<eventType==Type::QCharacterChange, void>::type subscribeToEvent(EventListener* eventListener, QCharacter* QcharacterToListenTo) {
        QCharacterChangeEvent::registerListener(eventListener, QcharacterToListenTo);
    }

    template <Type eventType>
    static typename std::enable_if<eventType==Type::QCharacterChange, void>::type subscribeToEvent(EventListener* eventListener, std::vector<QCharacter*> QCharactersToListenTo) {
        QCharacterChangeEvent::registerListener(eventListener, QCharactersToListenTo);
    }

    //PortalCreationEvent registerListener templates.
    template <Type eventType>
    static typename std::enable_if<eventType==Type::PortalCreation, void>::type subscribeToEvent(EventListener* eventListener, int portalId) {
        PortalCreationEvent::registerListener(eventListener, portalId);
    }

    EventBus();
};

#endif // EVENTBUS_H
