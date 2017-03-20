//
// Created by madsy on 18.03.17.
//

#include <stdexcept>
#include <cassert>
#include <fstream>
#include "MidiEvent.h"

MidiEvent::MidiEventType MidiEvent::getEventType(unsigned char event) {
    unsigned int highest_nibble = (event >> 4u) & 0xFu;
    unsigned int lowest_nibble = event & 0xFu;

    //MSB must be set
    if(!(highest_nibble & 0x8)){
        return MidiEvent::MidiEventType::EVT_INVALID;
    }

    MidiEvent::MidiEventType type = MidiEvent::MidiEventType::EVT_INVALID;
    if(highest_nibble < 0xF){
        type = MidiEvent::MidiEventType::EVT_CHANNEL;
    } else { //highest_nibble == 0xF
        if(lowest_nibble == 0xF){
            type = MidiEvent::MidiEventType ::EVT_META;
        } else if(lowest_nibble < 0x8){
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

MidiEvent::SystemRealtimeEventType MidiEvent::getSystemRealtimeEventType(unsigned char event) {
    unsigned int code = event & 0xFu;
    /* must be greater than 0x7 here */
    assert(code >= 0x8);
    if(code == 0x9 || code == 0xD){
        throw std::runtime_error("Undefined system realtime event");
    }
    return (MidiEvent::SystemRealtimeEventType)code;
}

MidiEvent::MetaEventType MidiEvent::getMetaEventType(unsigned int& bytesread, std::ifstream& strm) {
    MidiEvent::MetaEventType type = MidiEvent::MetaEventType::EVT_COPYRIGHT;

    unsigned char byte1, byte2, byte3;
    byte1 = byte2 = byte3 = 0;
    strm.read((char*)&byte1, 1);

    //if(byte1 == 0x0 || byte1 == 0x20 || byte1 == 0x2F || byte1 == 0x51 || byte1 == 0x54 || byte1 == 0x58){


    switch (byte1) {
        case 0x0:{
            strm.read((char*)&byte1, 1);
            if(byte1 != 0){ throw std::runtime_error("Invalid Meta Event"); }
            strm.read((char*)&byte1, 1);
            if(byte1 != 2){ throw std::runtime_error("Invalid Meta Event"); }
            type = MidiEvent::MetaEventType::EVT_SEQUENCE_NUMBER;
            break;
        }
        case 0x1:
            type = MidiEvent::MetaEventType::EVT_TEXT;
            break;
        case 0x2:
            type = MidiEvent::MetaEventType::EVT_COPYRIGHT;
            break;
        case 0x3:
            type = MidiEvent::MetaEventType::EVT_TRACK_NAME;
            break;
        case 0x4:
            type = MidiEvent::MetaEventType::EVT_INSTRUMENT_NAME;
            break;
        case 0x5:
            type = MidiEvent::MetaEventType::EVT_LYRICS;
            break;
        case 0x6:
            type = MidiEvent::MetaEventType::EVT_MARKER;
            break;
        case 0x7:
            type = MidiEvent::MetaEventType::EVT_CUE_POINT;
            break;
        case 0x20:{
            strm.read((char*)&byte1, 1);
            if(byte1 != 0x1){ throw std::runtime_error("Invalid Meta Event"); }
            type = MidiEvent::MetaEventType::EVT_MIDI_CHANNEL_PREFIX;
            break;
        }
        case 0x2F:{
            strm.read((char*)&byte1, 1);
            if(byte1 != 0x0){ throw std::runtime_error("Invalid Meta Event"); }
            type = MidiEvent::MetaEventType::EVT_END_OF_TRACK;
            break;
        }
        case 0x51:{
            strm.read((char*)&byte1, 1);
            if(byte1 != 0x3){ throw std::runtime_error("Invalid Meta Event"); }
            type = MidiEvent::MetaEventType::EVT_SET_TEMPO;
            break;
        }
        case 0x54:{
            strm.read((char*)&byte1, 1);
            if(byte1 != 0x5){ throw std::runtime_error("Invalid Meta Event"); }
            type = MidiEvent::MetaEventType::EVT_SMPTE_OFFSET;
            break;
        }
        case 0x58:{
            strm.read((char*)&byte1, 1);
            if(byte1 != 0x4){ throw std::runtime_error("Invalid Meta Event"); }
            type = MidiEvent::MetaEventType::EVT_TIME_SIGNATURE;
            break;
        }
        default:
            throw std::runtime_error("Invalid Meta Event");
    }
    return type;
}

MidiEvent::MidiEvent() {

}

float MidiEvent::getTimestamp() const {
    return 0;
}

void MidiEvent::print() {

}

MidiEvent::MidiEvent(unsigned int timestamp) : pTick(timestamp){

}














