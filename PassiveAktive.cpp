//
// Created by MBK on 26.11.25.
//

#include "PassiveAktive.h"

void Active::attach(Passive * element) {
    passiveObjects.push_back(element);
}

void Active::detach(Passive * element) {
    int index = 0;
    for (auto* item : passiveObjects) {
        if (item == element) {
            passiveObjects.erase(passiveObjects.begin()+index);
        }
    }
}

void Active::activate() {
    for (auto element : passiveObjects) {
        element->notify();
    }
}
