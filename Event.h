#ifndef EVENT_H
#define EVENT_H
#include <QtCore/qdebug.h>
#include <QtCore/qlogging.h>
#include <map>
class Tile;
class Character;
class QCharacter;
class EventListener;
//Start of Event declaration.
template<typename eventType>
class Event{
protected:
    // Here I've decided to use CRTP so that each subclass can have its own static EventListeners.
    // and because of that definitions will have to be here.
    inline static std::vector<EventListener*> EventListeners = {};
    static void registerListener(EventListener* eventListener) {if (!isListenerSubscribedToMe(eventListener)){EventListeners.push_back(eventListener);};}
    static bool deregisterListener(EventListener* eventListener) {
        // Base implementation for classes that only have a general EventListeners vector.
        // Can be overridden by just redeclaring it to cause name hiding.
        // returns true if it found and deleted it, false otherwise. So functions that have one or more registers don't loop needlessly if the main one doesn't have it.
        for (auto it = EventListeners.begin(); it!=EventListeners.end(); ){
            if ((*it)==eventListener){
                it = EventListeners.erase(it);
                return true;
            }
            else{
                it++;
            }
        }
        return false;
    }
    static bool isListenerSubscribedToMe(EventListener* eventListener){
        for (auto it = EventListeners.begin(); it!=EventListeners.end(); it++ ){
            if ((*it)==eventListener){return true;}
        }
        return false;
    }


    static bool isListenerSubscribedToThisEvent(EventListener* eventListener, eventType* event);
    // Some event types provide functionality to filter events to certain criteria (specific tiles or characters for example).
    // This is needed for the notifyListenersAccordingToRegister that would otherwise have to get the specific member of the event
    // and compare that to the eventListener. But since this a template class, it doesn't know what's the actual get function for that specific
    // event, so this function allows each Event type to just implement a small function instead of re-defining the notifyListenersAccordingToRegister
    // function and losing on reusability.
    //notifyListeners functions
    static void notifyListeners(eventType* event);
    static void notifySpecificListener(EventListener* eventListener, eventType* event);
    // Same thing as isListenerSubscribedToThisEvent: eventListener.onWhat? Let the specific event handle it.
    template<typename vectorType>
    static void notifyListenersAccordingToRegister(eventType* event, std::map<EventListener*, std::vector<vectorType*>>* preferenceRegister){
        for (auto eventListenerIterator = EventListeners.begin(); eventListenerIterator!=EventListeners.end(); eventListenerIterator++){
            if (preferenceRegister->count(*eventListenerIterator)){
                if (eventType::isListenerSubscribedToThisEvent(*eventListenerIterator, event)){
                    eventType::notifySpecificListener(*eventListenerIterator, event);
                    return;
                }
            }
            else{
                eventType::notifySpecificListener(*eventListenerIterator, event);
            }
        }
    }
};

//Start of EventListener declaration.
class AnimateTileEvent;
class CharacterHealthChangeEvent;
class TileChangeEvent;
class QCharacterChangeEvent;
class EventListener{
public:
    virtual void onCharacterHealthChange(CharacterHealthChangeEvent* event){};
    virtual void onAnimateTile(AnimateTileEvent* event){};
    virtual void onTileChange(TileChangeEvent* event){};
    virtual void onQCharacterChange(QCharacterChangeEvent* event){};
    ~EventListener();
};


//Start of AnimateTileEvent declaration.
class AnimateTileEvent : private Event<AnimateTileEvent>{
    friend class EventBus;
public:
    enum Visualization{
        colorizeTile,
        overlayText
    };
private:
    Tile* m_affectedTile;
    std::string m_textToOverlay;
    std::vector<Visualization> m_visualizations;
    static void notifyListeners(AnimateTileEvent* event);

public:
    AnimateTileEvent(Tile* tile,std::vector<Visualization> visualizations, std::string textToOverlay="");
    Tile* getAffectedTile() const;
    std::string_view getOverlayText() const;
    std::vector<Visualization> getVisualizations();
};

//Start of CharacterHealthChangeEventEvent declaration.
class CharacterHealthChangeEvent : private Event<CharacterHealthChangeEvent>{
    friend class EventBus;
    Character* m_character;
    inline static std::map<EventListener*, std::vector<Character*>> characterPreferenceRegister = {};
public:
    CharacterHealthChangeEvent(Character* character);
    Character* getCharacter();
    static bool isListenerSubscribedToThisEvent(EventListener* eventListener, CharacterHealthChangeEvent* event);
    static void notifySpecificListener(EventListener* eventListener, CharacterHealthChangeEvent* event);
    static void notifyListeners(CharacterHealthChangeEvent* event);
    static void deregisterListener(EventListener* eventListener);
    //registerListener declarations
    using Event::registerListener;
    static void registerListener(EventListener* eventListener, Character* characterToListenTo);
    static void registerListener(EventListener* eventListener, std::vector<Character*> ListOfCharactersToListenTo);

};

//Start of TileTextureChangeEvent declaration.
class TileChangeEvent : private Event<TileChangeEvent>
{
    friend class EventBus;
public:
    enum ChangeType{
        TextureChange,
        DoorStatus,
        Character
    };
private:
    // Default behavior is that those who use the normal function listen to all tiles updates
    // if the register has an entry for a certain amount of listeners, it respects it and only notifies
    // them of that.
    Tile* m_changedTile;
    ChangeType m_changeType;
    inline static std::map<EventListener*, std::vector<Tile*>> TilePreferenceRegister = {};
public:

    TileChangeEvent(Tile* changedTile, ChangeType changeType);
    Tile* getChangedTile() const;
    ChangeType getChangeType() const;
    static bool isListenerSubscribedToThisEvent(EventListener* eventListener, TileChangeEvent* event);
    //notifyListeners declarations
    static void notifyListeners(TileChangeEvent* event);
    static void deregisterListener(EventListener* eventListener);
    static void notifySpecificListener(EventListener* eventListener, TileChangeEvent* event);
    // registerListener declarations
    using Event::registerListener; // Apparently name hiding doesn't care about functions signatures.
    // Overloaded function for those who want to register for a specific tile.
    static void registerListener(EventListener* eventListener, Tile* TileToListenTo);
    // Overloaded function for those who want to register to multiple tiles.
    static void registerListener(EventListener* eventListener, std::vector<Tile*> ListOfTilesToListenTo);

};

class QCharacterChangeEvent : private Event<QCharacterChangeEvent>
{
    friend class EventBus;
public:
    enum ChangeType{
        healthbar,
        death
    };
private:

    QCharacter* m_changedQCharacter;
    ChangeType m_changeType;
    inline static std::map<EventListener*, std::vector<QCharacter*>> QCharacterPreferenceRegister = {};
public:
    QCharacterChangeEvent(QCharacter* changedCharacter, ChangeType changeType);
    QCharacter* getChangedQCharacter() const;
    ChangeType getChangeType() const;
    static bool isListenerSubscribedToThisEvent(EventListener* eventListener, QCharacterChangeEvent* event);
    //notifyListeners declarations
    static void notifyListeners(QCharacterChangeEvent* event);
    static void deregisterListener(EventListener* eventListener);
    static void deregisterListener(EventListener* eventListener, QCharacter* QCharacter); //overloaded to deregister just from one QChar.
    static void notifySpecificListener(EventListener* eventListener, QCharacterChangeEvent* event);
    // registerListener declarations
    using Event::registerListener;
    static void registerListener(EventListener* eventListener, QCharacter* QCharacterToListenTo);
    static void registerListener(EventListener* eventListener, std::vector<QCharacter*> ListOfQCharactersToListenTo);
    static void test(EventListener* el, std::pair<int,int> p);
};



#endif // EVENT_H
