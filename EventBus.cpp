#include "EventBus.h"

void EventBus::subscribeToEvent(Type eventType, EventListener* eventListener)
{

    switch (eventType){
    case CharacterDeath : {CharacterDeathEvent::registerListener(eventListener);break;}
    case CharacterHealthChange: {CharacterHealthChangeEvent::registerListener(eventListener);break;}
    case AnimateTile: {AnimateTileEvent::registerListener(eventListener);break;}
    }
}

EventBus::EventBus() {}
