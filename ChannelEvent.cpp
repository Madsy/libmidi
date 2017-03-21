//
// Created by madsy on 19.03.17.
//

#include "ChannelEvent.h"

ChannelEvent::ChannelEvent(unsigned int timestamp, int ch) : MidiEvent(timestamp), pChannel(ch){

}
//ChEvtNoteOff
ChEvtNoteOff::ChEvtNoteOff(unsigned int timestamp, int ch, int k, int v) :
        ChannelEvent(timestamp, ch), pKey(k), pVelocity(v){

}
//ChEvtNoteOn
ChEvtNoteOn::ChEvtNoteOn(unsigned int timestamp, int ch, int k, int v) :
        ChannelEvent(timestamp, ch), pKey(k), pVelocity(v){

}
//ChEvtAfterTouch1
ChEvtAfterTouch1::ChEvtAfterTouch1(unsigned int timestamp, int ch, int k, int p) :
        ChannelEvent(timestamp, ch), pKey(k), pPressure(p){

}
//ChEvtControlChange
ChEvtControlChange::ChEvtControlChange(unsigned int timestamp, int ch, int id, int val) :
        ChannelEvent(timestamp, ch), pControllerID(id), pValue(val){

}
ChEvtProgramChange::ChEvtProgramChange(unsigned int timestamp, int ch, int patch) :
        ChannelEvent(timestamp, ch), pPatch(patch){

}
ChEvtAfterTouch2::ChEvtAfterTouch2(unsigned int timestamp, int ch, int p) :
        ChannelEvent(timestamp, ch), pPressure(p){

}
ChEvtPitchWheel::ChEvtPitchWheel(unsigned int timestamp, int ch, int v) :
        ChannelEvent(timestamp, ch), pValue(v){

}




const char* notes[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "b", "H"
};

static std::string noteToString(unsigned int key){
    std::string note = std::string(notes[key % 12]);
    char octave = '0' + (key/12u);
    note += '-';
    note += octave;
    return note;
}

void ChannelEvent::print() {
    printf("ChannelEvent, Channel %u\t", this->getChannel());
}
void ChEvtNoteOff::print() {
    ChannelEvent::print();
    printf("NoteOff Event, Note %s, Velocity %u\n", noteToString(this->getKey()).c_str(), this->getVelocity());
}
void ChEvtNoteOn::print() {
    ChannelEvent::print();
    printf("NoteOn Event, Note %s, Velocity %u\n", noteToString(this->getKey()).c_str(), this->getVelocity());
}
void ChEvtAfterTouch1::print() {
    ChannelEvent::print();
    printf("AfterTouch Event, Note %s, Pressure %u\n", noteToString(this->getKey()).c_str(), this->getPressure());
}
void ChEvtControlChange::print() {
    ChannelEvent::print();
    printf("ControlChange Event, ControllerID %u, Value %u\n", this->getControllerID(), this->getValue());
}
void ChEvtProgramChange::print() {
    ChannelEvent::print();
    printf("ProgramChange Event, Patch Number %u\n", this->getPatchValue());
}
void ChEvtAfterTouch2::print() {
    ChannelEvent::print();
    printf("AfterTouch2 Event, Pressure %u\n", this->getPressure());
}
void ChEvtPitchWheel::print() {
    ChannelEvent::print();
    printf("PitchWheel Event, Position %u\n", this->getValue());
}










int ChannelEvent::getChannel() const {
    return pChannel;
}

int ChEvtNoteOff::getKey() const {
    return pKey;
}

int ChEvtNoteOff::getVelocity() const {
    return pVelocity;
}

int ChEvtNoteOn::getVelocity() const {
    return pVelocity;
}

int ChEvtNoteOn::getKey() const {
    return pKey;
}

int ChEvtAfterTouch1::getKey() const {
    return pKey;
}

int ChEvtAfterTouch1::getPressure() const {
    return pPressure;
}

int ChEvtControlChange::getControllerID() const {
    return pControllerID;
}

int ChEvtControlChange::getValue() const {
    return pValue;
}

int ChEvtProgramChange::getPatchValue() const {
    return pPatch;
}

int ChEvtAfterTouch2::getPressure() const {
    return pPressure;
}

int ChEvtPitchWheel::getValue() const {
    return pValue;
}

