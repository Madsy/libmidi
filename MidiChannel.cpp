//
// Created by madsy on 22.03.17.
//

#include <algorithm>
#include "MidiChannel.h"

MidiChannel::MidiChannel() : pChannelIndex(0), pStartTick(0), pEndTick(0) {

}

MidiChannel::~MidiChannel() {

}

std::vector<std::shared_ptr<MidiEvent>> MidiChannel::getEventsWithinInterval(unsigned int startTick, unsigned int endTick) const {
    std::vector<std::shared_ptr<MidiEvent>> result;
    for(size_t i = 0; i < pEvents.size(); i++){
        unsigned int t = pEvents[i]->getTimestamp();
        if(t >= startTick && t < endTick){
            result.push_back(pEvents[i]);
        }
    }
    return pEvents;
}

const std::vector<std::shared_ptr<MidiEvent>> &MidiChannel::getEvents() const {
    return pEvents;
}



void MidiChannel::addEvent(const std::shared_ptr<MidiEvent> &evt) {
    pEvents.push_back(evt);
}

void MidiChannel::sort() {
    std::stable_sort(pEvents.begin(),
                     pEvents.end(),
                     [](const std::shared_ptr<MidiEvent>& a, const std::shared_ptr<MidiEvent>& b) -> bool {
                         return a->getTimestamp() < b->getTimestamp();
                     }
    );
}

unsigned int MidiChannel::getStartTick() const {
    return pStartTick;
}

unsigned int MidiChannel::getEndTick() const {
    return pEndTick;
}

void MidiChannel::update() {
    //refresh start and end tick after adding all events
    pStartTick = 0xFFFFFFFF;
    pEndTick = 0;
    for(size_t i = 0; i < pEvents.size(); i++){
        if(pEvents[i] && pEvents[i]->getTimestamp() < pStartTick){ pStartTick = pEvents[i]->getTimestamp(); }
        if(pEvents[i] && pEvents[i]->getTimestamp() > pEndTick){ pEndTick = pEvents[i]->getTimestamp(); }
    }
}
