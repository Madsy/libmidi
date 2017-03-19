//
// Created by madsy on 18.03.17.
//

#include <stdexcept>
#include <cassert>
#include "MidiEvent.h"

MidiEvent::MidiEventType MidiEvent::getEventType(unsigned char event) {
    unsigned int highest_nibble = (event >> 4u) & 0xFu;
    unsigned int lowest_nibble = event & 0xFu;
    MidiEvent::MidiEventType type = MidiEvent::MidiEventType::EVT_CHANNEL;
    if(highest_nibble >= 0x8 && highest_nibble < 0xF){
        type = MidiEvent::MidiEventType::EVT_CHANNEL;
    } else { //highest_nibble == 0xF
        if(lowest_nibble < 0x8){
            type = MidiEvent::MidiEventType ::EVT_SYSCOMMON;
        } else {
            type = MidiEvent::MidiEventType ::EVT_SYSRT;
        }
    }
    return type;
}

MidiEvent::ChannelEventType MidiEvent::getChannelEventType(unsigned char event) {
    unsigned int code = (event >> 4) & ~0x8u;
    //Should not be 0x7 here. That would be a system event with prefix 0xFx
    assert(code != 0x7);
    return (MidiEvent::ChannelEventType)code;
}

MidiEvent::SystemCommonEventType MidiEvent::getSystemCommonEventType(unsigned char event) {
    unsigned int code = event & 0xFu;
    /* should not be greater than 0x7 here*/
    assert(code < 0x8);

    if(code == 0x1 || code == 0x4 || code == 0x5){
        throw std::runtime_error("Undefined system common event");
    }
    return (MidiEvent::SystemCommonEventType)code;
}

MidiEvent::SystemRealtimeEventType MidiEvent::getSystemRealtimeEventtype(unsigned char event) {
    unsigned int code = event & 0xFu;
    /* must be greater than 0x7 here */
    assert(code >= 0x8);
    if(code == 0x9 || code == 0xD){
        throw std::runtime_error("Undefined system realtime event");
    }
    return (MidiEvent::SystemRealtimeEventType)code;
}

MidiEvent::MidiEvent() {

}

float MidiEvent::getTimestamp() const {
    return 0;
}

void MidiEvent::setTimestamp(float t) {

}

void MidiEvent::print() {

}

std::unique_ptr<MidiEvent> createMidiEvent(std::ifstream &strm) {
    return std::unique_ptr<MidiEvent>();
}













