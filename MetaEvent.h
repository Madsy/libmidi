//
// Created by madsy on 20.03.17.
//

#ifndef MIDITEST_METAEVENT_H
#define MIDITEST_METAEVENT_H


#include "MidiEvent.h"

class MetaEvtSequenceNumber : public MidiEvent {

};

class MetaEvtTrackName : public MidiEvent {

};

class MetaEvtInstrumentName : public MidiEvent {

};

class MetaEvtLyrics : public MidiEvent {

};

class MetaEvtMarker : public MidiEvent {

};

class MetaEvtCuePoint : public MidiEvent {

};

class MetaEvtMidiChannelPrefix : public MidiEvent {

};

class MetaEvtEndOfTrack : public MidiEvent {

};

class MetaEvtSetTempo : public MidiEvent {

};

class MetaEvtSMPTEOffset : public MidiEvent {

};

class MetaEvtTimeSignature : public MidiEvent {

};

#endif //MIDITEST_METAEVENT_H
