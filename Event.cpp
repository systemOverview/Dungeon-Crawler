#include "Event.h"
#include <QtCore/qdebug.h>


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
    characterPreferenceRegister[eventListener].push_back(characterToListenTo);
    registerListener(eventListener);
}


//Start of TileChangeEvent definitions.
TileChangeEvent::TileChangeEvent(Tile *changedTile, ChangeType changeType) : m_changedTile{changedTile}, m_changeType{changeType}{};

Tile* TileChangeEvent::getChangedTile() const {return m_changedTile;}

bool TileChangeEvent::isListenerSubscribedToThisEvent(EventListener* eventListener, TileChangeEvent* event){
    assert(TilePreferenceRegister.count(eventListener)>0 && "isListenerSubscribedToThisEvent was supplied with an eventListener with no preferences");
    for (auto subscribedTilesIterator = TilePreferenceRegister[eventListener].begin(); subscribedTilesIterator!=TilePreferenceRegister[eventListener].end(); subscribedTilesIterator++){
        if ((*subscribedTilesIterator)==event->getChangedTile()){
            (eventListener)->onTileChange(event);
            return true;
        }
    }
    return false;
}

TileChangeEvent::ChangeType TileChangeEvent::getChangeType() const{return m_changeType;}

void TileChangeEvent::registerListener(EventListener* eventListener, Tile *TileToListenTo)
{

    TilePreferenceRegister[eventListener].push_back(TileToListenTo);
    registerListener(eventListener);
}

void TileChangeEvent::registerListener(EventListener* eventListener, std::vector<Tile *> ListOfTilesToListenTo)
{

    for (auto it = ListOfTilesToListenTo.begin(); it!=ListOfTilesToListenTo.end(); it++){
        TilePreferenceRegister[eventListener].push_back(*it);
    }
    registerListener(eventListener);
}

void TileChangeEvent::notifyListeners(TileChangeEvent* event){notifyListenersAccordingToRegister(event, &TilePreferenceRegister);}
void TileChangeEvent::notifySpecificListener(EventListener* eventListener, TileChangeEvent *event){eventListener->onTileChange(event);}

