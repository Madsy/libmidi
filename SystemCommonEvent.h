//
// Created by madsy on 19.03.17.
//

#ifndef MIDITEST_SYSTEMCOMMONEVENT_H
#define MIDITEST_SYSTEMCOMMONEVENT_H


#include <vector>
#include "MidiEvent.h"

class SysCmnEvtExclusive : public MidiEvent {
public:
    int getManufacturerID() const;
    const std::vector<unsigned char>& getData() const;
protected:
    SysCmnEvtExclusive(unsigned int timestamp, int manufacturer_code, const std::vector<unsigned char>& data);
private:
    int pManufacturerCode;
    std::vector<unsigned char> pData;
};

class SysCmnEvtSongPositionCounter : public MidiEvent {
public:
    int getSongPosition() const;
protected:
    SysCmnEvtSongPositionCounter(unsigned int timestamp, int pos);
private:
    int pPosition;
};

class SysCmnEvtSongSelect : public MidiEvent {
public:
    int getSongID() const;
protected:
    SysCmnEvtSongSelect(unsigned int timestamp, int songid);
private:
    int pSongID;
};


//Oscillators should be tunes. No data in this class.
class SysCmnEvtTuneRequest : public MidiEvent {
protected:
    SysCmnEvtTuneRequest(unsigned int timestamp);
};

//Marks the end of an exclusive message. No data in this class.
class SysCmnEvtEndExclusive : public MidiEvent {
protected:
    SysCmnEvtEndExclusive(unsigned int timestamp);
};

#endif //MIDITEST_SYSTEMCOMMONEVENT_H
