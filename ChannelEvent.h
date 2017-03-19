//
// Created by madsy on 19.03.17.
//

#ifndef MIDITEST_CHANNELEVENT_H
#define MIDITEST_CHANNELEVENT_H


#include "MidiEvent.h"
//#include "MidiEventFactory.h"

/*
    EVT_NOTE_OFF        = 0x0
    EVT_NOTE_ON         = 0x1
    EVT_AFTERTOUCH1     = 0x2
    EVT_CONTROL_CHANGE  = 0x3
    EVT_PROGRAM_CHANGE  = 0x4
    EVT_AFTERTOUCH2     = 0x5
    EVT_PITCH_WHEEL     = 0x6
*/



class MidiEventFactory;


//Base class for channel-based events
class ChannelEvent : public MidiEvent {
public:
    friend class MidiEventFactory;
    int getChannel() const;
protected:
    ChannelEvent(int ch);
private:
    int pChannel;
};






class ChEvtNoteOff : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getKey() const;
    int getVelocity() const;
protected:
    ChEvtNoteOff(int ch, int k, int v);
private:
    int pKey;
    int pVelocity;
};

class ChEvtNoteOn : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getKey() const;
    int getVelocity() const;
protected:
    ChEvtNoteOn(int ch, int k, int v);
private:
    int pKey;
    int pVelocity;
};

//Polyphonic Key Pressure
class ChEvtAfterTouch1 : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getKey() const;
    int getPressure() const;
protected:
    ChEvtAfterTouch1(int ch, int k, int p);
private:
    int pKey;
    int pPressure;
};

class ChEvtControlChange : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getControllerID() const;
    int getValue() const;
protected:
    ChEvtControlChange(int ch, int id, int val);
private:
    int pControllerID;
    int pValue;
};

class ChEvtProgramChange : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getPatchValue() const;
protected:
    ChEvtProgramChange(int ch, int patch);
private:
    int pPatch;
};

//Channel pressure
class ChEvtAfterTouch2 : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getPressure() const;
protected:
    ChEvtAfterTouch2(int ch, int p);
private:
    int pPressure;
};

class ChEvtPitchWheel : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getValue() const;
protected:
    ChEvtPitchWheel(int ch, int v);
private:
    int pValue;
};
#endif //MIDITEST_CHANNELEVENT_H

















