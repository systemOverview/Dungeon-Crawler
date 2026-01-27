#ifndef EVENT_H
#define EVENT_H
#include <QtCore/qdebug.h>
#include <QtCore/qlogging.h>
#include <map>
// new type checklist: add onEvent to EventListener, define notifyListener to use notifyListenersAccordingToRegister if you have a pref register
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
    static void notifyListeners(eventType* event);
    static void notifySpecificListener(EventListener* eventListener, eventType* event);
    template<typename vectorType, typename function>
    static void notifyListenersAccordingToRegister(eventType* event, std::map<EventListener*,std::vector<vectorType*>>* preferenceRegister, function criteriaFunction){
        for (auto eventListenerIterator = EventListeners.begin(); eventListenerIterator!=EventListeners.end(); eventListenerIterator++){
            if (preferenceRegister->count(*eventListenerIterator)){
                for (auto subscriptionIterator = (*preferenceRegister)[(*eventListenerIterator)].begin(); subscriptionIterator!=(*preferenceRegister)[(*eventListenerIterator)].end(); subscriptionIterator++){
                    if (criteriaFunction((*subscriptionIterator), event)){
                        eventType::notifySpecificListener(*eventListenerIterator, event);
                        return;
                    }
                }
            }
            else{
                eventType::notifySpecificListener(*eventListenerIterator, event);
            }
        }
    }
    template <typename vectorType> // Character, Tile, etc...
    static void deregisterListenerAccordingToRegister(EventListener* eventListener, vectorType* elementToRemove, std::map<EventListener*, std::vector<vectorType*>>* preferenceRegister){

    for (auto listenerPair = preferenceRegister->begin(); listenerPair!=preferenceRegister->end(); listenerPair++){
        if ((*listenerPair).first==eventListener){
            for (auto it = (*listenerPair).second.begin(); it!=(*listenerPair).second.end();){
                if ((*it)==elementToRemove){
                    it = (*listenerPair).second.erase(it);
                }
                else{
                    it++;
                }
            }
        }
    }
    }
};

//Start of EventListener declaration.
class AnimateTileEvent;
class CharacterHealthChangeEvent;
class TileChangeEvent;
class QCharacterChangeEvent;
class VisualizationStatusEvent;
class DjikstraSearchEvent;
class EventListener{
public:
    virtual void onCharacterHealthChange(CharacterHealthChangeEvent* event){};
    virtual void onAnimateTile(AnimateTileEvent* event){};
    virtual void onTileChange(TileChangeEvent* event){};
    virtual void onQCharacterChange(QCharacterChangeEvent* event){};
    virtual void onVisualizationChange(VisualizationStatusEvent* event){};
    virtual void onDjikstraSearch(DjikstraSearchEvent* event){};
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
    //notifyListeners declarations
    static void notifyListeners(QCharacterChangeEvent* event);
    static void deregisterListener(EventListener* eventListener);
    static void deregisterListener(EventListener* eventListener, QCharacter* QCharacter); //overloaded to deregister just from one QChar.
    static void notifySpecificListener(EventListener* eventListener, QCharacterChangeEvent* event);
    // registerListener declarations
    using Event::registerListener;
    static void registerListener(EventListener* eventListener, QCharacter* QCharacterToListenTo);
    static void registerListener(EventListener* eventListener, std::vector<QCharacter*> ListOfQCharactersToListenTo);
};

class VisualizationStatusEvent : private Event<VisualizationStatusEvent>
{
    friend class EventBus;
public:
    enum Status{
        Quit,
        Start
    };
private:
    Status m_status;
public:
    VisualizationStatusEvent(Status status) : m_status(status){};
    Status getStatus() const;
    static void notifyListeners(VisualizationStatusEvent* event);
    using Event::registerListener;
};


//Start of DjikstraSearchEvent declaration.
//This event contains the information about one Djikstra Loop (from the moment it picks a tile out of queue, till it finds the shortest path of that  run).
class DjikstraSearchEvent : private Event<DjikstraSearchEvent>
{
    friend class EventBus;
public:
    struct Loop{
        struct Neighbour{
        private:
            std::pair<int,int> m_cords;
            float m_djikstraValue;
            bool m_wasDjikstraValueUpdated;
        public :
            Neighbour(std::pair<int,int> cords, float djikstraValue, bool wasDjikstraValueUpdated);
            std::pair<int,int>  getCords();
            float getDjikstraValue();
            bool wasDjikstraValueUpdated();

        };
    private:
        std::pair<int,int> m_extractedTileCords = {}; // The (strictly one) tile that was extracted from the queue
        //ie the one with the shortest djikstra value in the queue.

        std::vector<Neighbour> m_neighbourTiles = {};
        std::map<std::pair<int,int>, std::pair<int,int>> m_previousRegister= {}; // stores the previous register state at loop end.
    public:
        Loop(std::pair<int,int> extractedTileCords = {}, std::vector<Neighbour> = {});;
        void setExtractedTile(std::pair<int,int> extractedTileCords);
        void addNeighbourTile(Neighbour neighbour);
        void setPreviousRegisterAtLoopEnd(std::map<std::pair<int,int>, std::pair<int,int>> reg);

        std::pair<int, int> getExtractedTileCords() const;
        std::vector<Neighbour> getNeighbourTiles() const;
        std::map<std::pair<int,int>, std::pair<int,int>> getPreviousRegister();
    };



private:
    std::vector<Loop> m_loops;
    std::vector<std::pair<int,int>> m_startingSearchRange = {}; //the cords of tiles that were added to the queue of Djikstra at the start. So the GUI can animate setting up the matrix for example.
    std::pair<int,int> m_startingTileCords;
    std::pair<int,int> m_targetTileCords;
public:
    DjikstraSearchEvent(std::vector<Loop> loops, std::vector<std::pair<int,int>> startingSearchRange, std::pair<int,int> startingTileCords, std::pair<int,int> targetTileCords);

    std::vector<std::pair<int,int>> getStartingSearchRange() const;
    std::vector<Loop> getLoops() const;
    std::pair<int,int> getStartingTileCords() const;
    std::pair<int,int> getTargetTileCords() const;
    static void notifyListeners(DjikstraSearchEvent* event);
    using Event::registerListener;
};


#endif // EVENT_H
