//
// Created by madsy on 20.03.17.
//

#ifndef MIDITEST_SYSTEMREALTIMEEVENT_H
#define MIDITEST_SYSTEMREALTIMEEVENT_H


#include "MidiEvent.h"

/* None of the currently implemented Realtime events have any data */
class SysRtEvtTimingClock : public MidiEvent {
    friend class MidiEventFactory;
protected:
    SysRtEvtTimingClock(unsigned int timestamp);
};

class SysRtEvtStart : public MidiEvent {
    friend class MidiEventFactory;
protected:
    SysRtEvtStart(unsigned int timestamp);
};

class SysRtEvtContinue : public MidiEvent {
    friend class MidiEventFactory;
protected:
    SysRtEvtContinue(unsigned int timestamp);
};

class SysRtEvtStop : public MidiEvent {
    friend class MidiEventFactory;
protected:
    SysRtEvtStop(unsigned int timestamp);
};

class SysRtEvtActiveSensing : public MidiEvent {
    friend class MidiEventFactory;
protected:
    SysRtEvtActiveSensing(unsigned int timestamp);
};




#endif //MIDITEST_SYSTEMREALTIMEEVENT_H
