//
// Created by madsy on 20.03.17.
//

#ifndef MIDITEST_SYSTEMREALTIMEEVENT_H
#define MIDITEST_SYSTEMREALTIMEEVENT_H


#include "MidiEvent.h"

/* None of the currently implemented Realtime events have any data */
class SysRtEvtTimingClock : public MidiEvent {
protected:
    SysRtEvtTimingClock(unsigned int timestamp);
};

class SysRtEvtStart : public MidiEvent {
protected:
    SysRtEvtStart(unsigned int timestamp);
};

class SysRtEvtContinue : public MidiEvent {
protected:
    SysRtEvtContinue(unsigned int timestamp);
};

class SysRtEvtStop : public MidiEvent {
protected:
    SysRtEvtStop(unsigned int timestamp);
};

class SysRtEvtActiveSensing : public MidiEvent {
protected:
    SysRtEvtActiveSensing(unsigned int timestamp);
};

class SysRtEvtReset : public MidiEvent {
protected:
    SysRtEvtReset(unsigned int timestamp);
};


#endif //MIDITEST_SYSTEMREALTIMEEVENT_H
