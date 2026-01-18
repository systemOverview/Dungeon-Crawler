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

//Start of TileChangeEvent definitions.
TileChangeEvent::TileChangeEvent(Tile *changedTile, ChangeType changeType) : m_changedTile{changedTile}, m_changeType{changeType}{};

Tile* TileChangeEvent::getChangedTile() const {return m_changedTile;}

TileChangeEvent::ChangeType TileChangeEvent::getChangeType() const{return m_changeType;}

void TileChangeEvent::registerListener(EventListener* eventListener, Tile *TileToListenTo)
{

    TilePreferenceRegister[eventListener].push_back(TileToListenTo);
    registerListener(eventListener);
}

void TileChangeEvent::registerListener(EventListener* eventListener, std::vector<Tile *> ListOfTilesToListenTo)
{

    for (auto it = ListOfTilesToListenTo.begin(); it!=ListOfTilesToListenTo.end(); it++){
        ListOfTilesToListenTo.push_back(*it);
    }
    registerListener(eventListener);
}

void TileChangeEvent::notifyListeners(TileChangeEvent* event)
{
    for (auto eventListener = EventListeners.begin(); eventListener!=EventListeners.end(); eventListener++){
        if (TilePreferenceRegister.count(*eventListener)){
            for (auto subscribedTilesIterator = TilePreferenceRegister[*eventListener].begin(); subscribedTilesIterator!=TilePreferenceRegister[*eventListener].end(); subscribedTilesIterator++){
                if ((*subscribedTilesIterator)==event->getChangedTile()){
                    (*eventListener)->onTileChange(event);
                    return;
                }
            }
        }
        else{
            (*eventListener)->onTileChange(event);
        }
    }
}
