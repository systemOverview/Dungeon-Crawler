#include "Event.h"
#include "EventBus.h"
#include "tile.h"
#include "QCharacter.h"
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
    auto criteriaFunction = [](QCharacter* QChar, QCharacterChangeEvent* event){
        if (QChar==nullptr){
            return false;
        }
        return QChar == event->getChangedQCharacter();
    };
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


//Start of DjikstraSearchEvent definitions.



DjikstraSearchEvent::DjikstraSearchEvent(std::vector<Loop> loops, std::vector<std::pair<int,int>> startingSearchRange, std::pair<int,int> startingTileCords, std::pair<int,int> targetTileCords)
    :m_loops{loops}, m_startingSearchRange{startingSearchRange}, m_startingTileCords {startingTileCords}, m_targetTileCords(targetTileCords){}


std::vector<std::pair<int,int>> DjikstraSearchEvent::getStartingSearchRange() const
{
    return m_startingSearchRange;
}

std::pair<int, int> DjikstraSearchEvent::getStartingTileCords() const{return m_startingTileCords;}

std::pair<int, int> DjikstraSearchEvent::getTargetTileCords() const {return m_targetTileCords; }

std::vector<DjikstraSearchEvent::Loop> DjikstraSearchEvent::getLoops() const{return m_loops;}



void DjikstraSearchEvent::notifyListeners(DjikstraSearchEvent *event)
{
    for (auto it = EventListeners.begin(); it!=EventListeners.end(); it++){
        (*it)->onDjikstraSearch(event);
    }
}



// Start of Loop Definitions

std::pair<int, int> DjikstraSearchEvent::Loop::getExtractedTileCords() const{return m_extractedTileCords;}

std::map<std::pair<int, int>, std::pair<int, int> > DjikstraSearchEvent::Loop::getPreviousRegister() {return m_previousRegister;}
std::vector<DjikstraSearchEvent::Loop::Neighbour> DjikstraSearchEvent::Loop::getNeighbourTiles() const{return m_neighbourTiles;}
DjikstraSearchEvent::Loop::Loop(std::pair<int, int> extractedTileCords, std::vector<DjikstraSearchEvent::Loop::Neighbour> neighbourTiles) :
    m_extractedTileCords{extractedTileCords}, m_neighbourTiles(neighbourTiles){}
void DjikstraSearchEvent::Loop::setExtractedTile(std::pair<int, int> extractedTileCords) {m_extractedTileCords = extractedTileCords;}
void DjikstraSearchEvent::Loop::addNeighbourTile (Neighbour neighbour) {m_neighbourTiles.push_back(neighbour);}

void DjikstraSearchEvent::Loop::setPreviousRegisterAtLoopEnd(std::map<std::pair<int, int>, std::pair<int, int> > reg) {m_previousRegister = reg;}
    // Start of Loop::Neighbour Definitions
DjikstraSearchEvent::Loop::Neighbour::Neighbour(std::pair<int, int> cords, float djikstraValue, bool wasDjikstraValueUpdated) : m_cords{cords}, m_djikstraValue{djikstraValue}, m_wasDjikstraValueUpdated{wasDjikstraValueUpdated}{};
std::pair<int, int> DjikstraSearchEvent::Loop::Neighbour::getCords() {return m_cords;}
float DjikstraSearchEvent::Loop::Neighbour::getDjikstraValue() {return m_djikstraValue;}
bool DjikstraSearchEvent::Loop::Neighbour::wasDjikstraValueUpdated(){return m_wasDjikstraValueUpdated;}

//Start of PortalCreationEvent definitions.
PortalCreationEvent::PortalCreationEvent(Portal* portal) : m_createdPortal{portal}, m_createdPortalId(portal->getPortalId()){}
Portal *PortalCreationEvent::getCreatedPortal(){return m_createdPortal;}
int PortalCreationEvent::getPortalId(){return m_createdPortalId;}
void PortalCreationEvent::notifyListeners(PortalCreationEvent *event)
{
    auto criteriaFunction = [](int portalId, PortalCreationEvent* event){ return portalId == event->getPortalId();};
    notifyListenersAccordingToRegister(event, &portalPreferenceRegister, criteriaFunction);
}

void PortalCreationEvent::notifySpecificListener(EventListener *eventListener, PortalCreationEvent *event)
{
    eventListener->onPortalCreation(event);
}

void PortalCreationEvent::deregisterListener(EventListener *eventListener)
{
    bool didFindAndDelete = Event::deregisterListener(eventListener); // First remove it from the main EventListeners vector.
    if (!didFindAndDelete){return;}
    for (auto listenerPortalPair = portalPreferenceRegister.begin(); listenerPortalPair!=portalPreferenceRegister.end(); ){
        if ((*listenerPortalPair).first==eventListener){
            listenerPortalPair = portalPreferenceRegister.erase(listenerPortalPair);
        }
        else{listenerPortalPair++;}
    }
}

void PortalCreationEvent::registerListener(EventListener *eventListener, int portalIdToListenTo)
{
    {
        portalPreferenceRegister[eventListener].push_back(portalIdToListenTo);
        registerListener(eventListener);
    }
}
