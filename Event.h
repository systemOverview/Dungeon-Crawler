#ifndef EVENT_H
#define EVENT_H
#include <map>
class Tile;
//Start of Event declaration.
class EventListener;
template<typename eventType>
class Event{
protected:
    // Here I've decided to use CRTP so that each subclass can have its own static EventListeners.
    // and because of that definitions will have to be here.
    inline static std::vector<EventListener*> EventListeners = {};
    static void registerListener(EventListener* eventListener) {if (!isListenerSubscribedToMe(eventListener)){EventListeners.push_back(eventListener);};}
    static void removeListener(EventListener* eventListener) {
        for (auto it = EventListeners.begin(); it!=EventListeners.end(); ){
            if ((*it)==eventListener){
                it = EventListeners.erase(it);
            }
            else{
                it++;
            }
        }
    }
    static bool isListenerSubscribedToMe(EventListener* eventListener){
        for (auto it = EventListeners.begin(); it!=EventListeners.end(); it++ ){
            if ((*it)==eventListener){return true;}
        }
        return false;
    }
    static void notifyListeners(eventType* event);
};

//Start of EventListener declaration.
class CharacterDeathEvent;
class AnimateTileEvent;
class CharacterHealthChangeEvent;
class TileChangeEvent;
class EventListener{
public:
    virtual void onCharacterDeath(CharacterDeathEvent* event){};
    virtual void onCharacterHealthChange(CharacterHealthChangeEvent* event){};
    virtual void onAnimateTile(AnimateTileEvent* event){};
    virtual void onTileChange(TileChangeEvent* event){};
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

//Start of CharacterDeathEvent declaration.
class CharacterDeathEvent : private Event<CharacterDeathEvent>{
    friend class EventBus;
    static void notifyListeners(CharacterDeathEvent* event);
};
//Start of CharacterHealthChangeEventEvent declaration.
class CharacterHealthChangeEvent : private Event<CharacterHealthChangeEvent>{
    friend class EventBus;
    static void notifyListeners(CharacterHealthChangeEvent* event);
};

//Start of TileTextureChangeEvent declaration.
class TileChangeEvent : private Event<TileChangeEvent>
{
    friend class EventBus;
public:
    enum ChangeType{
        TextureChange,
        DoorStatus
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
    using Event::registerListener; // Apparently name hiding doesn't care about functions signatures.
    // Overloaded function for those who want to register for a specific tile.
    static void registerListener(EventListener* eventListener, Tile* TileToListenTo);
    // Overloaded function for those who want to register to multiple tiles.
    static void registerListener(EventListener* eventListener, std::vector<Tile*> ListOfTilesToListenTo);
    static void notifyListeners(TileChangeEvent* event);

};

#endif // EVENT_H
