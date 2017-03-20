//
// Created by madsy on 19.03.17.
//

#include "ChannelEvent.h"

ChannelEvent::ChannelEvent(unsigned int timestamp, int ch) : MidiEvent(timestamp), pChannel(ch){

}

int ChannelEvent::getChannel() const {
    return 0;
}



//ChEvtNoteOff
ChEvtNoteOff::ChEvtNoteOff(unsigned int timestamp, int ch, int k, int v) :
        ChannelEvent(timestamp, ch), pKey(k), pVelocity(v){

}

int ChEvtNoteOff::getKey() const {
    return pKey;
}

int ChEvtNoteOff::getVelocity() const {
    return pVelocity;
}



//ChEvtNoteOn
ChEvtNoteOn::ChEvtNoteOn(unsigned int timestamp, int ch, int k, int v) :
        ChannelEvent(timestamp, ch), pKey(k), pVelocity(v){

}

int ChEvtNoteOn::getVelocity() const {
    return 0;
}

int ChEvtNoteOn::getKey() const {
    return 0;
}



//ChEvtAfterTouch1
ChEvtAfterTouch1::ChEvtAfterTouch1(unsigned int timestamp, int ch, int k, int p) :
        ChannelEvent(timestamp, ch), pKey(k), pPressure(p){

}

int ChEvtAfterTouch1::getKey() const {
    return pKey;
}

int ChEvtAfterTouch1::getPressure() const {
    return pPressure;
}



//ChEvtControlChange
ChEvtControlChange::ChEvtControlChange(unsigned int timestamp, int ch, int id, int val) :
        ChannelEvent(timestamp, ch), pControllerID(id), pValue(val){

}

int ChEvtControlChange::getControllerID() const {
    return pControllerID;
}

int ChEvtControlChange::getValue() const {
    return 0;
}


ChEvtProgramChange::ChEvtProgramChange(unsigned int timestamp, int ch, int patch) :
        ChannelEvent(timestamp, ch), pPatch(patch){

}

int ChEvtProgramChange::getPatchValue() const {
    return pPatch;
}


ChEvtAfterTouch2::ChEvtAfterTouch2(unsigned int timestamp, int ch, int p) :
        ChannelEvent(timestamp, ch), pPressure(p){

}

int ChEvtAfterTouch2::getPressure() const {
    return pPressure;
}


ChEvtPitchWheel::ChEvtPitchWheel(unsigned int timestamp, int ch, int v) :
        ChannelEvent(timestamp, ch), pValue(v){

}

int ChEvtPitchWheel::getValue() const {
    return pValue;
}

