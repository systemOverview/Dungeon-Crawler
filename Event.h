#ifndef EVENT_H
#define EVENT_H
#include "tile.h"

//Start of Event declaration.
class EventListener;
template<typename eventType>
class Event{
protected:
    // Here I've decided to use CRTP so that each subclass can have its own static EventListeners.
    // and because of that definitions will have to be here.
    inline static std::vector<EventListener*> EventListeners = {};
    static void registerListener(EventListener* eventListener)
    {
        EventListeners.push_back(eventListener);
    }
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
    static void notifyListeners(eventType* event);
};

//Start of EventListener declaration.
class CharacterDeathEvent;
class AnimateTileEvent;
class CharacterHealthChangeEvent;
class EventListener{
public:
    virtual void onCharacterDeath(CharacterDeathEvent* event){};
    virtual void onCharacterHealthChange(CharacterHealthChangeEvent* event){};
    virtual void onAnimateTile(AnimateTileEvent* event){};
};


//Start of AnimateTileEvent declaration.
class AnimateTileEvent : private Event<AnimateTileEvent>{
public:
    enum Visualization{
        colorizeTile,
        overlayText
    };
private:
    friend class EventBus;
    Tile* m_affectedTile;
    std::string m_textToOverlay;
    std::vector<Visualization> m_visualizations;
    static void notifyListeners(AnimateTileEvent* event);

public:
    AnimateTileEvent(Tile* tile,std::vector<Visualization> visualizations, std::string textToOverlay="");
    Tile* getAffectedTile() const;
    std::string_view getOverlayText() const;

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


#endif // EVENT_H
