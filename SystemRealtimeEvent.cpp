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


void SysRtEvtTimingClock::print() {
    printf("Realtime Event: Timing Clock\n");
}

void SysRtEvtStart::print() {
    printf("Realtime Event: Start\n");
}

void SysRtEvtContinue::print() {
    printf("Realtime Event: Continue\n");
}

void SysRtEvtStop::print() {
    printf("Realtime Event: Stop\n");
}

void SysRtEvtActiveSensing::print() {
    printf("Realtime Event: Active Sensing\n");
}

