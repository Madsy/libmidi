//
// Created by madsy on 22.03.17.
//

#ifndef MIDITEST_MIDICHANNEL_H
#define MIDITEST_MIDICHANNEL_H

#include <vector>
#include <memory>
#include "MidiEvent.h"

class MidiFile;

class MidiChannel {
    friend class MidiFile;
public:
    MidiChannel();
    ~MidiChannel();
    std::vector<std::shared_ptr<MidiEvent>> getEventsWithinInterval(unsigned int startTick, unsigned int endTick) const;
    unsigned int getStartTick() const;
    unsigned int getEndTick() const;
    void update();
    const std::vector<std::shared_ptr<MidiEvent>>& getEvents() const;
    void addEvent(const std::shared_ptr<MidiEvent>& evt);
private:
    void sort();
    std::vector<std::shared_ptr<MidiEvent>> pEvents;
    unsigned int pChannelIndex;
    unsigned int pStartTick;
    unsigned int pEndTick;
};


#endif //MIDITEST_MIDICHANNEL_H
