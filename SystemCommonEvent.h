//
// Created by madsy on 19.03.17.
//

#ifndef MIDITEST_SYSTEMCOMMONEVENT_H
#define MIDITEST_SYSTEMCOMMONEVENT_H


#include "MidiEvent.h"

class SysCmnEvtExclusive : public MidiEvent {

};

class SysCmnEvtSongPositionCounter : public MidiEvent {

};

class SysCmnEvtSongSelect : public MidiEvent {

};

class SysCmnEvtTuneRequest : public MidiEvent {

};

class SysCmnEvtEndExclusive : public MidiEvent {

};

#endif //MIDITEST_SYSTEMCOMMONEVENT_H
