//
// Created by madsy on 20.03.17.
//

#ifndef MIDITEST_METAEVENT_H
#define MIDITEST_METAEVENT_H


#include <vector>
#include "MidiEvent.h"

class MetaEvtSequenceNumber : public MidiEvent {
    friend class MidiEventFactory;
public:
    unsigned int getSequenceNumber() const;
protected:
    MetaEvtSequenceNumber(unsigned int timestamp, unsigned int sequenceNum);
private:
    unsigned int pSequenceNumber;
};

class MetaEvtText : public MidiEvent {
    friend class MidiEventFactory;
public:
    const std::string& getText() const;
protected:
    MetaEvtText(unsigned int timestamp, const std::string& text);
private:
    std::string pText;
};

class MetaEvtCopyright : public MidiEvent {
    friend class MidiEventFactory;
public:
    const std::string& getCopyRight() const;
protected:
    MetaEvtCopyright(unsigned int timestamp, const std::string& copyright);
private:
    std::string pCopyright;
};

class MetaEvtTrackName : public MidiEvent {
    friend class MidiEventFactory;
public:
    const std::string& getTrackName() const;
protected:
    MetaEvtTrackName(unsigned int timestamp, const std::string& trackName);
private:
    std::string pTrackName;
};

class MetaEvtInstrumentName : public MidiEvent {
    friend class MidiEventFactory;
public:
    const std::string& getInstrumentName() const;
protected:
    MetaEvtInstrumentName(unsigned int timestamp, const std::string& instrumentName);
private:
    std::string pInstrumentName;
};

class MetaEvtLyrics : public MidiEvent {
    friend class MidiEventFactory;
public:
    const std::string& getLyrics() const;
protected:
    MetaEvtLyrics(unsigned int timestamp, const std::string& lyrics);
private:
    std::string pLyrics;
};

class MetaEvtMarker : public MidiEvent {
    friend class MidiEventFactory;
public:
    const std::string& getMarker() const;
protected:
    MetaEvtMarker(unsigned int timestamp, const std::string& marker);
private:
    std::string pMarker;
};

class MetaEvtCuePoint : public MidiEvent {
    friend class MidiEventFactory;
public:
    const std::string& getCuePoint() const;
protected:
    MetaEvtCuePoint(unsigned int timestamp, const std::string& cue);
private:
    std::string pCuePoint;
};

class MetaEvtMidiChannelPrefix : public MidiEvent {
    friend class MidiEventFactory;
public:
    unsigned int getChannel() const;
protected:
    MetaEvtMidiChannelPrefix(unsigned int timestamp, unsigned int channel);
private:
    unsigned int pChannel;
};

class MetaEvtEndOfTrack : public MidiEvent {
    friend class MidiEventFactory;
protected:
    MetaEvtEndOfTrack(unsigned int timestamp);
};

class MetaEvtSetTempo : public MidiEvent {
    friend class MidiEventFactory;
public:
    unsigned int getTempo() const;
    //24 bit tempo
protected:
    MetaEvtSetTempo(unsigned int timestamp, unsigned int tempo);
private:
    unsigned int pTempo;

};

class MetaEvtSMPTEOffset : public MidiEvent {
    friend class MidiEventFactory;
public:
    unsigned int getHours() const;
    unsigned int getMinutes() const;
    unsigned int getSeconds() const;
    unsigned int getFrames() const;
    unsigned int getSubFrames() const;
protected:
    MetaEvtSMPTEOffset(unsigned int timestamp, unsigned int hours, unsigned int minutes, unsigned int seconds, unsigned int frames, unsigned int frame_fraction);
private:
    unsigned int pHours;
    unsigned int pMinutes;
    unsigned int pSeconds;
    unsigned int pFrames;
    unsigned int pFractionFrames;
};

class MetaEvtTimeSignature : public MidiEvent {
    friend class MidiEventFactory;
public:
    unsigned int getNumerator() const;
    unsigned int getDenominator() const;
    unsigned int getClocksPerTick() const;
    unsigned int getNotesperClock() const;
    //notes_per_clock is number of 1/32 notes per 24 midi clocks. Usually 8, so 1/4 note = 24 midi clocks
protected:
    MetaEvtTimeSignature(unsigned int timestamp, unsigned int numerator, unsigned int denominator, unsigned int clocks_per_tick, unsigned int notes_per_clock);
private:
    unsigned int pNumerator;
    unsigned int pDenominator;
    unsigned int pClocksPerTick;
    unsigned int pNotesPerClock;
};

class MetaEvtKeySignature : public MidiEvent {
    friend class MidiEventFactory;
public:
    unsigned int getSharpFlats() const;
    unsigned int getMinorMajor() const;
protected:
    MetaEvtKeySignature(unsigned int timestamp, unsigned int sharpflats, unsigned int minormajor);
private:
    unsigned int pSharpFlats;
    unsigned int pMinorMajor;
};

class MetaEvtSequencerSpecific : public MidiEvent {
    friend class MidiEventFactory;
public:
    unsigned int getManufacturerID() const;
    const std::vector<unsigned char>& getData() const;
protected:
    MetaEvtSequencerSpecific(unsigned int timestamp, const std::vector<unsigned char>& data);
private:
    std::vector<unsigned char> pData;
};
/* TODO: implement Key Signature and Sequencer-Specific Meta-event */
#endif //MIDITEST_METAEVENT_H
