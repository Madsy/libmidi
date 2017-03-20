//
// Created by madsy on 19.03.17.
//

#include "MidiEventFactory.h"

std::unique_ptr<MidiEvent>
MidiEventFactory::createChannelEvent(
        unsigned int& bytesread,
        std::ifstream& strm,
        MidiEvent::ChannelEventType type,
        unsigned int timestamp,
        int channel){

    MidiEvent* result = nullptr;
    unsigned char arg1, arg2;

    switch(type){
        case MidiEvent::ChannelEventType::EVT_NOTE_OFF: {
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            result = new ChEvtNoteOff(timestamp, channel, arg1, arg2);
            bytesread += 2;
            break;
        }
        case MidiEvent::ChannelEventType::EVT_NOTE_ON:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            result = new ChEvtNoteOn(timestamp, channel, arg1, arg2);
            bytesread += 2;
            break;
        }
        case MidiEvent::ChannelEventType::EVT_AFTERTOUCH1:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            result = new ChEvtAfterTouch1(timestamp, channel, arg1, arg2);
            bytesread += 2;
            break;
        }
        case MidiEvent::ChannelEventType::EVT_CONTROL_CHANGE:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            result = new ChEvtControlChange(timestamp, channel, arg1, arg2);
            bytesread += 2;
            break;
        }
        case MidiEvent::ChannelEventType::EVT_PROGRAM_CHANGE:{
            strm.read((char *) &arg1, 1);
            result = new ChEvtProgramChange(timestamp, channel, arg1);
            break;
        }
        case MidiEvent::ChannelEventType::EVT_AFTERTOUCH2:{
            strm.read((char *) &arg1, 1);
            result = new ChEvtAfterTouch2(timestamp, channel, arg1);
            bytesread += 1;
            break;
        }
        case MidiEvent::ChannelEventType::EVT_PITCH_WHEEL:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            unsigned short wheel = (arg2<<7)|arg1;
            result = new ChEvtPitchWheel(timestamp, channel, (int)wheel);
            bytesread += 2;
            break;
        }
    }
    return std::unique_ptr<MidiEvent>(result);
}

std::unique_ptr<MidiEvent>
MidiEventFactory::createSystemCommonEvent(
        unsigned int& bytesread,
        std::ifstream& strm,
        MidiEvent::SystemCommonEventType type,
        unsigned int timestamp){
    switch(type){
        case MidiEvent::SystemCommonEventType::EVT_SYSTEM_EXCLUSIVE:
        case MidiEvent::SystemCommonEventType::EVT_SONG_POSITION_POINTER:
        case MidiEvent::SystemCommonEventType::EVT_SONG_SELECT:
        case MidiEvent::SystemCommonEventType::EVT_TUNE_REQUEST:
        case MidiEvent::SystemCommonEventType::EVT_END_OF_EXCLUSIVE:
            break;
    }
    return std::make_unique<MidiEvent>();
}

std::unique_ptr<MidiEvent>
MidiEventFactory::createSystemRealtimeEvent(
        unsigned int& bytesread,
        std::ifstream& strm,
        MidiEvent::SystemRealtimeEventType type,
        unsigned int timestamp){
    switch(type){
        case MidiEvent::SystemRealtimeEventType::EVT_TIMING_CLOCK:
        case MidiEvent::SystemRealtimeEventType::EVT_START:
        case MidiEvent::SystemRealtimeEventType::EVT_CONTINUE:
        case MidiEvent::SystemRealtimeEventType::EVT_STOP:
        case MidiEvent::SystemRealtimeEventType::EVT_ACTIVE_SENSING:
        case MidiEvent::SystemRealtimeEventType::EVT_RESET:
            break;
    }
    return std::make_unique<MidiEvent>();
}

std::unique_ptr<MidiEvent>
MidiEventFactory::createMidiEvent(
        unsigned int& bytesread,
        std::ifstream& strm){


    unsigned int timestamp = 0;
    unsigned char event_type_byte = 0;

    //Read timestamp before the event
    bytesread = readMidiVarLenInteger(timestamp, strm);
    strm.read((char*)&event_type_byte, 1);
    bytesread++;

    MidiEvent::MidiEventType evt = MidiEvent::getEventType(event_type_byte);

    switch(evt){
        case MidiEvent::MidiEventType::EVT_CHANNEL: {
            MidiEvent::ChannelEventType ChEv = MidiEvent::getChannelEventType(event_type_byte);
            return createChannelEvent(bytesread, strm, ChEv, timestamp, (event_type_byte & 0xF));
        }
        case MidiEvent::MidiEventType::EVT_SYSCOMMON: {
            MidiEvent::SystemCommonEventType SysCmn = MidiEvent::getSystemCommonEventType(event_type_byte);
            return createSystemCommonEvent(bytesread, strm, SysCmn, timestamp);
        }
        case MidiEvent::MidiEventType::EVT_SYSRT: {
            MidiEvent::SystemRealtimeEventType SysRt = MidiEvent::getSystemRealtimeEventtype(event_type_byte);
            return createSystemRealtimeEvent(bytesread, strm, SysRt, timestamp);
        }
    }
}

MidiEventFactory::~MidiEventFactory() {

}


