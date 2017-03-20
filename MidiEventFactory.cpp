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


    MidiEvent* result = nullptr;
    unsigned char arg1, arg2, data7bits;

    switch(type){
        case MidiEvent::SystemCommonEventType::EVT_SYSTEM_EXCLUSIVE:{
            std::vector<unsigned char> data;
            strm.read((char *) &arg1, 1);

            do {
                strm.read((char *) &data7bits, 1);
                //TODO: Assemble the bits (remove the MSB)
                data.push_back(data7bits);
            } while(data7bits != 0xF7u);

            result = new SysCmnEvtExclusive(timestamp, arg1, data);
            bytesread += (data.size() + 1);
            break;
        }
        case MidiEvent::SystemCommonEventType::EVT_SONG_POSITION_POINTER:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            int timecounter = (arg2<<7)|arg1;
            //14-bit song position
            result = new SysCmnEvtSongPositionCounter(timestamp, timecounter);
            bytesread += 2;
            break;
        }
        case MidiEvent::SystemCommonEventType::EVT_SONG_SELECT:{
            strm.read((char *) &arg1, 1);
            //7-bit song select value
            result = new SysCmnEvtSongSelect(timestamp, arg1);
            bytesread += 1;
            break;
        }
        case MidiEvent::SystemCommonEventType::EVT_TUNE_REQUEST:{
            result = new SysCmnEvtTuneRequest(timestamp);
            //bytesread += 0;
            break;
        }
        //Not really an independent event, so kept unimplemented.
        //Used by EVT_SYSTEM_EXCLUSIVE
        case MidiEvent::SystemCommonEventType::EVT_END_OF_EXCLUSIVE:{
            break;
        }
    }
    return std::unique_ptr<MidiEvent>(result);
}

std::unique_ptr<MidiEvent>
MidiEventFactory::createSystemRealtimeEvent(
        unsigned int& bytesread,
        std::ifstream& strm,
        MidiEvent::SystemRealtimeEventType type,
        unsigned int timestamp){


    MidiEvent* result = nullptr;

    switch(type){
        case MidiEvent::SystemRealtimeEventType::EVT_TIMING_CLOCK:{
            result = new SysRtEvtTimingClock(timestamp);
            break;
        }
        case MidiEvent::SystemRealtimeEventType::EVT_START:{
            result = new SysRtEvtStart(timestamp);
            break;
        }
        case MidiEvent::SystemRealtimeEventType::EVT_CONTINUE:{
            result = new SysRtEvtContinue(timestamp);
            break;
        }
        case MidiEvent::SystemRealtimeEventType::EVT_STOP:{
            result = new SysRtEvtStop(timestamp);
            break;
        }
        case MidiEvent::SystemRealtimeEventType::EVT_ACTIVE_SENSING:{
            result = new SysRtEvtActiveSensing(timestamp);
            break;
        }
    }
    return std::unique_ptr<MidiEvent>(result);
}

std::unique_ptr<MidiEvent>
MidiEventFactory::createMetaEvent(
        unsigned int &bytesread,
        std::ifstream &strm,
        MidiEvent::MetaEventType type,
        unsigned int timestamp) {

    MidiEvent* result = nullptr;

    switch(type) {
        case MidiEvent::MetaEventType::EVT_SEQUENCE_NUMBER:
            break;
        case MidiEvent::MetaEventType::EVT_TEXT:
            break;
        case MidiEvent::MetaEventType::EVT_COPYRIGHT:
            break;
        case MidiEvent::MetaEventType::EVT_TRACK_NAME:
            break;
        case MidiEvent::MetaEventType::EVT_INSTRUMENT_NAME:
            break;
        case MidiEvent::MetaEventType::EVT_LYRICS:
            break;
        case MidiEvent::MetaEventType::EVT_MARKER:
            break;
        case MidiEvent::MetaEventType::EVT_CUE_POINT:
            break;
        case MidiEvent::MetaEventType::EVT_MIDI_CHANNEL_PREFIX:
            break;
        case MidiEvent::MetaEventType::EVT_END_OF_TRACK:
            break;
        case MidiEvent::MetaEventType::EVT_SET_TEMPO:
            break;
        case MidiEvent::MetaEventType::EVT_SMPTE_OFFSET:
            break;
        case MidiEvent::MetaEventType::EVT_TIME_SIGNATURE:
            break;
    }
    return std::unique_ptr<MidiEvent>();
}








//Event factory function
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
            MidiEvent::SystemRealtimeEventType SysRt = MidiEvent::getSystemRealtimeEventType(event_type_byte);
            return createSystemRealtimeEvent(bytesread, strm, SysRt, timestamp);
        }
    }
}

MidiEventFactory::~MidiEventFactory() {

}




