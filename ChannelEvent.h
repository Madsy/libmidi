//
// Created by madsy on 19.03.17.
//

#ifndef MIDITEST_CHANNELEVENT_H
#define MIDITEST_CHANNELEVENT_H


#include "MidiEvent.h"


class MidiEventFactory;


//Base class for channel-based events. All channel-based events have a channel.
class ChannelEvent : public MidiEvent {
public:
    friend class MidiEventFactory;
    int getChannel() const;
    void print() override;
protected:
    ChannelEvent(unsigned int timestamp, int ch);
private:
    int pChannel;
};



class ChEvtNoteOff : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getKey() const;
    int getVelocity() const;
    void print() override;
protected:
    ChEvtNoteOff(unsigned int timestamp, int ch, int k, int v);
private:
    int pKey;
    int pVelocity;
};

class ChEvtNoteOn : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getKey() const;
    int getVelocity() const;
    void print() override;
protected:
    ChEvtNoteOn(unsigned int timestamp, int ch, int k, int v);
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
    void print() override;
protected:
    ChEvtAfterTouch1(unsigned int timestamp, int ch, int k, int p);
private:
    int pKey;
    int pPressure;
};

class ChEvtControlChange : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getControllerID() const;
    int getValue() const;
    void print() override;
protected:
    ChEvtControlChange(unsigned int timestamp, int ch, int id, int val);
private:
    int pControllerID;
    int pValue;
};

class ChEvtProgramChange : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getPatchValue() const;
    void print() override;
protected:
    ChEvtProgramChange(unsigned int timestamp, int ch, int patch);
private:
    int pPatch;
};

//Channel pressure
class ChEvtAfterTouch2 : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getPressure() const;
    void print() override;
protected:
    ChEvtAfterTouch2(unsigned int timestamp, int ch, int p);
private:
    int pPressure;
};

class ChEvtPitchWheel : public ChannelEvent {
public:
    friend class MidiEventFactory;
    int getValue() const;
    void print() override;
protected:
    ChEvtPitchWheel(unsigned int timestamp, int ch, int v);
private:
    int pValue;
};
#endif //MIDITEST_CHANNELEVENT_H

















