//
// Created by madsy on 20.03.17.
//

#include "SystemRealtimeEvent.h"

SysRtEvtTimingClock::SysRtEvtTimingClock(unsigned int timestamp) : MidiEvent(timestamp){

}

SysRtEvtStart::SysRtEvtStart(unsigned int timestamp) : MidiEvent(timestamp){

}

SysRtEvtContinue::SysRtEvtContinue(unsigned int timestamp) : MidiEvent(timestamp){

}

SysRtEvtStop::SysRtEvtStop(unsigned int timestamp) : MidiEvent(timestamp){

}

SysRtEvtActiveSensing::SysRtEvtActiveSensing(unsigned int timestamp) : MidiEvent(timestamp){

}

SysRtEvtReset::SysRtEvtReset(unsigned int timestamp) : MidiEvent(timestamp){

}
