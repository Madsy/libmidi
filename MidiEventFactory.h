//
// Created by madsy on 19.03.17.
//

#ifndef MIDITEST_MIDIEVENTFACTORY_H
#define MIDITEST_MIDIEVENTFACTORY_H

#include <fstream>
#include "MidiEvent.h"
#include "MidiIOUtils.h"
#include "ChannelEvent.h"
#include "SystemCommonEvent.h"
#include "SystemRealtimeEvent.h"

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
                       unsigned int timestamp,
                       int channel);
    static std::unique_ptr<MidiEvent>
    createSystemCommonEvent(unsigned int& bytesread,
                            std::ifstream& strm,
                            MidiEvent::SystemCommonEventType type,
                            unsigned int timestamp);
    static std::unique_ptr<MidiEvent>
    createSystemRealtimeEvent(unsigned int& bytesread,
                              std::ifstream& strm,
                              MidiEvent::SystemRealtimeEventType type,
                              unsigned int timestamp);
    static std::unique_ptr<MidiEvent>
    createMetaEvent(unsigned int& bytesread,
                              std::ifstream& strm,
                              MidiEvent::MetaEventType type,
                              unsigned int timestamp);
};

#endif //MIDITEST_MIDIEVENTFACTORY_H
