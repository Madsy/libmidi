//
// Created by madsy on 19.03.17.
//

#include <iterator>
#include "MidiEventFactory.h"

unsigned int MidiEventFactory::previousChannelEventCode = 0;

//Treat bytes as 8-bit and read until we encounter a 0xF7 terminator
static unsigned int readBuffer8BitsUntilF7(std::vector<unsigned char>& buffer, std::ifstream& strm){
    unsigned char data8bits = 0;
    unsigned int bytesread = 1;
    strm.read((char *) &data8bits, 1);
    do {
        buffer.push_back(data8bits);
        strm.read((char *) &data8bits, 1);
        bytesread++;
        //TODO: Assemble the bits (remove the MSB)
    } while(data8bits != 0xF7u);
    return bytesread;
}

static unsigned int readBuffer8BitsWithLength(std::vector<unsigned char>& buffer, std::ifstream& strm){
    unsigned char data8bits = 0;
    unsigned int bytesread = 0;
    unsigned char len = 0;
    strm.read((char *) &len, 1);
    for(unsigned int i = 0; i < len; i++){
        strm.read((char *) &data8bits, 1);
        buffer.push_back(data8bits);
    }
    return len + 1;
}

static unsigned int readMetaEvtString(std::string& str, std::ifstream& strm){
    unsigned char len = 0;
    char ch = 0;
    strm.read((char *)&len, 1);
    for(unsigned int i = 0; i < len; i++){
        strm.read(&ch, 1);
        str += ch;
    }
    return len + 1;
}









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
            bytesread +=1;
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




//readBuffer8BitsUntilF7
std::unique_ptr<MidiEvent>
MidiEventFactory::createMetaEvent(
        unsigned int &bytesread,
        std::ifstream &strm,
        MidiEvent::MetaEventType type,
        unsigned int timestamp) {

    std::string s;
    MidiEvent* result = nullptr;
    unsigned char arg1, arg2, arg3, arg4, arg5, data7bits;

    switch(type) {
        case MidiEvent::MetaEventType::EVT_SEQUENCE_NUMBER:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            int timecounter = (arg2<<8)|arg1;
            //16-bit sequence number
            result = new MetaEvtSequenceNumber(timestamp, timecounter);
            bytesread += 2;
            break;
        }
        case MidiEvent::MetaEventType::EVT_TEXT:{
            bytesread += readMetaEvtString(s, strm);
            result = new MetaEvtText(timestamp, s);
            break;
        }
        case MidiEvent::MetaEventType::EVT_COPYRIGHT:{
            bytesread += readMetaEvtString(s, strm);
            result = new MetaEvtCopyright(timestamp, s);
            break;
        }
        case MidiEvent::MetaEventType::EVT_TRACK_NAME:{
            bytesread += readMetaEvtString(s, strm);
            result = new MetaEvtTrackName(timestamp, s);
            break;
        }
        case MidiEvent::MetaEventType::EVT_INSTRUMENT_NAME:{
            bytesread += readMetaEvtString(s, strm);
            result = new MetaEvtInstrumentName(timestamp, s);
            break;
        }
        case MidiEvent::MetaEventType::EVT_LYRICS:{
            bytesread += readMetaEvtString(s, strm);
            result = new MetaEvtLyrics(timestamp, s);
            break;
        }
        case MidiEvent::MetaEventType::EVT_MARKER:{
            bytesread += readMetaEvtString(s, strm);
            result = new MetaEvtMarker(timestamp, s);
            break;
        }
        case MidiEvent::MetaEventType::EVT_CUE_POINT:{
            bytesread += readMetaEvtString(s, strm);
            result = new MetaEvtCuePoint(timestamp, s);
            break;
        }
        case MidiEvent::MetaEventType::EVT_MIDI_CHANNEL_PREFIX:{
            strm.read((char *) &arg1, 1);
            result = new MetaEvtMidiChannelPrefix(timestamp, arg1);
            bytesread += 1;
            break;
        }
        case MidiEvent::MetaEventType::EVT_END_OF_TRACK:{
            result = new MetaEvtEndOfTrack(timestamp);
            break;
        }
        case MidiEvent::MetaEventType::EVT_SET_TEMPO:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            strm.read((char *) &arg3, 1);
            unsigned int tempo = (arg1<<16)|(arg2<<8)|arg3;
            result = new MetaEvtSetTempo(timestamp, tempo);
            bytesread += 3;
            break;
        }
        case MidiEvent::MetaEventType::EVT_SMPTE_OFFSET:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            strm.read((char *) &arg3, 1);
            strm.read((char *) &arg4, 1);
            strm.read((char *) &arg5, 1);
            result = new MetaEvtSMPTEOffset(timestamp, arg1, arg2, arg3, arg4, arg5);
            bytesread += 5;
            break;
        }
        case MidiEvent::MetaEventType::EVT_TIME_SIGNATURE:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            strm.read((char *) &arg3, 1);
            strm.read((char *) &arg4, 1);
            result = new MetaEvtTimeSignature(timestamp, arg1, arg2, arg3, arg4);
            bytesread += 4;
            break;
        }
        case MidiEvent::MetaEventType::EVT_KEY_SIGNATURE:{
            strm.read((char *) &arg1, 1);
            strm.read((char *) &arg2, 1);
            result = new MetaEvtKeySignature(timestamp, arg1, arg2);
            bytesread += 2;
            break;
        }
        case MidiEvent::MetaEventType::EVT_SEQUENCER_SPECIFIC:{
            std::vector<unsigned char> data;
            bytesread += readBuffer8BitsWithLength(data, strm);
            result = new MetaEvtSequencerSpecific(timestamp, data);
            break;
        }

    }
    return std::unique_ptr<MidiEvent>(result);
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

    //first code
    MidiEvent::MidiEventType evt = MidiEvent::getEventType(event_type_byte);

    switch(evt){
        case MidiEvent::MidiEventType::EVT_CHANNEL: {
            MidiEvent::ChannelEventType ChEv = MidiEvent::getChannelEventType(event_type_byte);
            MidiEventFactory::previousChannelEventCode = event_type_byte;
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
        case MidiEvent::MidiEventType::EVT_META:{
            MidiEvent::MetaEventType Meta = MidiEvent::getMetaEventType(bytesread, strm);
            return createMetaEvent(bytesread, strm, Meta, timestamp);
        }
        case MidiEvent::MidiEventType::EVT_RUNNING_STATUS: {
            //MSB not set, so this is a running status without a code
            //use the previous code (and channel)

            //Go back one byte. The byte we read is a data byte
            bytesread--;
            strm.seekg(-1, strm.cur);
            MidiEvent::ChannelEventType ChEv = MidiEvent::getChannelEventType(MidiEventFactory::previousChannelEventCode);
            return createChannelEvent(bytesread, strm, ChEv, timestamp, (MidiEventFactory::previousChannelEventCode & 0xF));
        }
    }
}

MidiEventFactory::~MidiEventFactory() {

}




