//
// Created by madsy on 19.03.17.
//

#include "SystemCommonEvent.h"

//SysCmnEvtExclusive
SysCmnEvtExclusive::SysCmnEvtExclusive(unsigned int timestamp, int manufacturer_code, const std::vector<unsigned char> &data) : MidiEvent(timestamp){

}
int SysCmnEvtExclusive::getManufacturerID() const {
    return pManufacturerCode;
}
const std::vector<unsigned char> &SysCmnEvtExclusive::getData() const {
    return pData;
}



//SysCmnEvtSongPositionCounter
SysCmnEvtSongPositionCounter::SysCmnEvtSongPositionCounter(unsigned int timestamp, int pos) : MidiEvent(timestamp), pPosition(pos){

}
int SysCmnEvtSongPositionCounter::getSongPosition() const {
    return pPosition;
}



//SysCmnEvtSongSelect
SysCmnEvtSongSelect::SysCmnEvtSongSelect(unsigned int timestamp, int songid) : MidiEvent(timestamp), pSongID(songid){

}
int SysCmnEvtSongSelect::getSongID() const {
    return pSongID;
}



//SysCmnEvtTuneRequest
SysCmnEvtTuneRequest::SysCmnEvtTuneRequest(unsigned int timestamp) : MidiEvent(timestamp){

}



//SysCmnEvtEndExclusive
SysCmnEvtEndExclusive::SysCmnEvtEndExclusive(unsigned int timestamp) : MidiEvent(timestamp){

}
