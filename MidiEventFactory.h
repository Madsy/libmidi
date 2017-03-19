//
// Created by madsy on 19.03.17.
//

#ifndef MIDITEST_MIDIEVENTFACTORY_H
#define MIDITEST_MIDIEVENTFACTORY_H

#include <fstream>
#include "MidiEvent.h"
#include "MidiIOUtils.h"
#include "ChannelEvent.h"

class MidiEventFactory {
public:
    MidiEventFactory(){};
    ~MidiEventFactory();
    static std::unique_ptr<MidiEvent> createMidiEvent(unsigned int& bytesread, std::ifstream& strm);
private:
    static std::unique_ptr<MidiEvent>
    createChannelEvent(unsigned int& bytesread,
                       std::ifstream& strm,
                       MidiEvent::ChannelEventType type,
                       int channel);
    static std::unique_ptr<MidiEvent>
    createSystemCommonEvent(unsigned int& bytesread,
                            std::ifstream& strm,
                            MidiEvent::SystemCommonEventType type);
    static std::unique_ptr<MidiEvent>
    createSystemRealtimeEvent(unsigned int& bytesread,
                              std::ifstream& strm,
                              MidiEvent::SystemRealtimeEventType type);
};

#endif //MIDITEST_MIDIEVENTFACTORY_H
