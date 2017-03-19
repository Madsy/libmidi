//
// Created by madsy on 19.03.17.
//

#include "ChannelEvent.h"

ChannelEvent::ChannelEvent(int ch) : pChannel(ch){

}

int ChannelEvent::getChannel() const {
    return 0;
}



//ChEvtNoteOff
ChEvtNoteOff::ChEvtNoteOff(int ch, int k, int v) :
        ChannelEvent(ch), pKey(k), pVelocity(v){

}

int ChEvtNoteOff::getKey() const {
    return pKey;
}

int ChEvtNoteOff::getVelocity() const {
    return pVelocity;
}



//ChEvtNoteOn
ChEvtNoteOn::ChEvtNoteOn(int ch, int k, int v) :
        ChannelEvent(ch), pKey(k), pVelocity(v){

}

int ChEvtNoteOn::getVelocity() const {
    return 0;
}

int ChEvtNoteOn::getKey() const {
    return 0;
}



//ChEvtAfterTouch1
ChEvtAfterTouch1::ChEvtAfterTouch1(int ch, int k, int p) :
        ChannelEvent(ch), pKey(k), pPressure(p){

}

int ChEvtAfterTouch1::getKey() const {
    return pKey;
}

int ChEvtAfterTouch1::getPressure() const {
    return pPressure;
}



//ChEvtControlChange
ChEvtControlChange::ChEvtControlChange(int ch, int id, int val) :
        ChannelEvent(ch), pControllerID(id), pValue(val){

}

int ChEvtControlChange::getControllerID() const {
    return pControllerID;
}

int ChEvtControlChange::getValue() const {
    return 0;
}


ChEvtProgramChange::ChEvtProgramChange(int ch, int patch) :
        ChannelEvent(ch), pPatch(patch){

}

int ChEvtProgramChange::getPatchValue() const {
    return pPatch;
}


ChEvtAfterTouch2::ChEvtAfterTouch2(int ch, int p) :
        ChannelEvent(ch), pPressure(p){

}

int ChEvtAfterTouch2::getPressure() const {
    return pPressure;
}


ChEvtPitchWheel::ChEvtPitchWheel(int ch, int v) :
        ChannelEvent(ch), pValue(v){

}

int ChEvtPitchWheel::getValue() const {
    return pValue;
}

