#include "Event.h"
#include "EventBus.h"
#include <QtCore/qdebug.h>

EventListener::~EventListener()
{
    EventBus::unsubscribeFromAllEvents(this);
}

//Start of AnimateTileEvent definitions.
void AnimateTileEvent::notifyListeners(AnimateTileEvent* event) {
    for (auto it = EventListeners.begin(); it!=EventListeners.end(); it++){
        (*it)->onAnimateTile(event);
    }
}
AnimateTileEvent::AnimateTileEvent(Tile* tile,std::vector<Visualization> visualizations, std::string textToOverlay):
    m_affectedTile{tile}, m_visualizations{visualizations}, m_textToOverlay{textToOverlay}
{};

Tile* AnimateTileEvent::getAffectedTile() const {return m_affectedTile;}
std::string_view AnimateTileEvent::getOverlayText() const {return m_textToOverlay;}
std::vector<AnimateTileEvent::Visualization> AnimateTileEvent::getVisualizations(){return m_visualizations;}

//Start of CharacterHealthChangeEventEvent definitions.
CharacterHealthChangeEvent::CharacterHealthChangeEvent(Character *character): m_character{character}{};
Character* CharacterHealthChangeEvent::getCharacter(){return m_character;}

void CharacterHealthChangeEvent::registerListener(EventListener *eventListener, Character *characterToListenTo)
{
    assert(characterToListenTo!=nullptr && "Character supplied is nullptr");
    characterPreferenceRegister[eventListener].push_back(characterToListenTo);
    registerListener(eventListener);
}

void CharacterHealthChangeEvent::registerListener(EventListener *eventListener, std::vector<Character *> ListOfCharactersToListenTo)
{
    for (auto it = ListOfCharactersToListenTo.begin(); it!=ListOfCharactersToListenTo.end(); it++){
        assert(*it!=nullptr && "Character supplied is nullptr");
        characterPreferenceRegister[eventListener].push_back(*it);
    }
    registerListener(eventListener);

}
void CharacterHealthChangeEvent::notifyListeners(CharacterHealthChangeEvent* event){        auto check = [](Character* QChar, CharacterHealthChangeEvent* event){ return QChar == event->getCharacter();};
    auto criteriaFunction = [](Character* QChar, CharacterHealthChangeEvent* event){ return QChar == event->getCharacter();};
    notifyListenersAccordingToRegister(event, &characterPreferenceRegister, criteriaFunction);
}

void CharacterHealthChangeEvent::deregisterListener(EventListener *eventListener)
{
    bool didFindAndDelete = Event::deregisterListener(eventListener); // First remove it from the main EventListeners vector.
    if (!didFindAndDelete){return;}
    for (auto listenerCharactersPair = characterPreferenceRegister.begin(); listenerCharactersPair!=characterPreferenceRegister.end(); ){
        if ((*listenerCharactersPair).first==eventListener){
            listenerCharactersPair = characterPreferenceRegister.erase(listenerCharactersPair);
        }
        else{listenerCharactersPair++;}
    }
}
void CharacterHealthChangeEvent::notifySpecificListener(EventListener* eventListener, CharacterHealthChangeEvent *event){
    eventListener->onCharacterHealthChange(event);
}


//Start of TileChangeEvent definitions.
TileChangeEvent::TileChangeEvent(Tile *changedTile, ChangeType changeType) : m_changedTile{changedTile}, m_changeType{changeType}{};
Tile* TileChangeEvent::getChangedTile() const {return m_changedTile;}

TileChangeEvent::ChangeType TileChangeEvent::getChangeType() const{return m_changeType;}
void TileChangeEvent::registerListener(EventListener* eventListener, Tile *TileToListenTo)
{
    assert(TileToListenTo!=nullptr && "Tile supplied is nullptr");
    TilePreferenceRegister[eventListener].push_back(TileToListenTo);
    registerListener(eventListener);
}

void TileChangeEvent::registerListener(EventListener* eventListener, std::vector<Tile *> ListOfTilesToListenTo)
{
    for (auto it = ListOfTilesToListenTo.begin(); it!=ListOfTilesToListenTo.end(); it++){
        assert(*it!=nullptr && "Tile supplied is nullptr");
        TilePreferenceRegister[eventListener].push_back(*it);
    }
    registerListener(eventListener);
}

void TileChangeEvent::notifyListeners(TileChangeEvent* event){
    auto criteriaFunction = [](Tile* tile, TileChangeEvent* event){ return tile == event->getChangedTile();};
    notifyListenersAccordingToRegister(event, &TilePreferenceRegister, criteriaFunction);
}
void TileChangeEvent::deregisterListener(EventListener *eventListener)
{
    bool didFindAndDelete = Event::deregisterListener(eventListener); // First remove it from the main EventListeners vector.
    if (!didFindAndDelete){return;}
    for (auto listenerTilesPair = TilePreferenceRegister.begin(); listenerTilesPair!=TilePreferenceRegister.end(); ){
        if ((*listenerTilesPair).first==eventListener){listenerTilesPair = TilePreferenceRegister.erase(listenerTilesPair);}
        else{listenerTilesPair++;}
    }
}
void TileChangeEvent::notifySpecificListener(EventListener* eventListener, TileChangeEvent *event){eventListener->onTileChange(event);}

//Start of QCharacterChangeEvent definitions.

QCharacterChangeEvent::QCharacterChangeEvent(QCharacter* changedQCharacter, ChangeType changeType) : m_changedQCharacter(changedQCharacter), m_changeType(changeType){}
QCharacter *QCharacterChangeEvent::getChangedQCharacter() const {return m_changedQCharacter;}
QCharacterChangeEvent::ChangeType QCharacterChangeEvent::getChangeType() const {return m_changeType;}

void QCharacterChangeEvent::notifyListeners(QCharacterChangeEvent *event){
    auto criteriaFunction = [](QCharacter* QChar, QCharacterChangeEvent* event){ return QChar == event->getChangedQCharacter();};
    notifyListenersAccordingToRegister(event, &QCharacterPreferenceRegister, criteriaFunction);
}
void QCharacterChangeEvent::deregisterListener(EventListener *eventListener)
{
    bool didFindAndDelete = Event::deregisterListener(eventListener); // First remove it from the main EventListeners vector.
    if (!didFindAndDelete){return;}
    for (auto listenerQCharacterPair = QCharacterPreferenceRegister.begin(); listenerQCharacterPair!=QCharacterPreferenceRegister.end(); ){
        if ((*listenerQCharacterPair).first==eventListener){listenerQCharacterPair = QCharacterPreferenceRegister.erase(listenerQCharacterPair);}
        else{listenerQCharacterPair++;}
    }
}
void QCharacterChangeEvent::deregisterListener(EventListener* eventListener, QCharacter* QCharacter)
{
    // assert(QCharacterPreferenceRegister.count(eventListener)>0 && "Event listener tried to deregister from specific event it is not subscribed to");
    // for (auto listenerQCharacterPair = QCharacterPreferenceRegister.begin(); listenerQCharacterPair!=QCharacterPreferenceRegister.end(); listenerQCharacterPair++){
    //     if ((*listenerQCharacterPair).first==eventListener){
    //     for (auto it = (*listenerQCharacterPair).second.begin(); it!=(*listenerQCharacterPair).second.end();){
    //         if ((*it)==QCharacter){
    //             it = (*listenerQCharacterPair).second.erase(it);
    //         }
    //         else{
    //             it++;
    //         }
    //     }
    //     }
    // }
    //    inline static std::map<EventListener*, std::vector<QCharacter*>> QCharacterPreferenceRegister = {};

    deregisterListenerAccordingToRegister(eventListener,QCharacter, &QCharacterPreferenceRegister);
}

void QCharacterChangeEvent::notifySpecificListener(EventListener *eventListener, QCharacterChangeEvent *event){eventListener->onQCharacterChange(event);}
void QCharacterChangeEvent::registerListener(EventListener *eventListener, QCharacter* QCharacterToListenTo)
{
    assert(QCharacterToListenTo!=nullptr && "QCharacter supplied is nullptr");
    QCharacterPreferenceRegister[eventListener].push_back(QCharacterToListenTo);
    registerListener(eventListener);
}

void QCharacterChangeEvent::registerListener(EventListener *eventListener, std::vector<QCharacter *> ListOfQCharactersToListenTo)
{
    for (auto it = ListOfQCharactersToListenTo.begin(); it!=ListOfQCharactersToListenTo.end(); it++){
        assert(*it!=nullptr && "Tile supplied is nullptr");
        QCharacterPreferenceRegister[eventListener].push_back(*it);
    }
    registerListener(eventListener);
}

//Start of VisualizationStatusEvent definitions.

VisualizationStatusEvent::Status VisualizationStatusEvent::getStatus() const
{
    return m_status;
}

void VisualizationStatusEvent::notifyListeners(VisualizationStatusEvent* event) {
    for (auto it = EventListeners.begin(); it!=EventListeners.end(); it++){
        (*it)->onVisualizationChange(event);
    }
}
