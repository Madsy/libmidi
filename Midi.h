//
// Created by madsy on 18.03.17.
//

#ifndef MIDITEST_MIDI_H
#define MIDITEST_MIDI_H

#include <vector>
#include <memory>
#include "MidiEvent.h"

class MidiFile;

class Midi {
public:
    friend class MidiFile;

    enum class MidiFileFormat {
        MIDI_FORMAT_0 = 0,
        MIDI_FORMAT_1,
        MIDI_FORMAT_2
    };
    enum class MidiTimeFormat {
        MIDI_TIME_TYPE_TPQN = 0,
        MIDI_TIME_TYPE_SMPTE
    };

    typedef std::unique_ptr<MidiEvent> MidiEventPtr;
    typedef std::vector<MidiEventPtr> MidiTrack;

    Midi();
    ~Midi();
    Midi(const Midi& m);
    MidiFileFormat getFileFormat() const;
    MidiTimeFormat getTimeFormat() const;
    size_t getTrackCount() const;
private:
    MidiFileFormat pMidiFileFormat;
    MidiTimeFormat pMidiTimeFormat;
    std::vector<MidiTrack> pTracks;
    //when using the Ticks Per Quarter Note format, compute pTicksPerSecond based on pTPQN_BeatsPerMinute
    //pTPQN_BeatsPerMinute defaults to 120bpm, but can be specified by tempo MIDI events
    int pTPQN_BeatsPerMinute;
    int pTPQN;
    int pSMPTE_FramesPerSecond;
    int pSMPTE_TicksPerFrame;
    // time/speed reference independent of time format
    // defined as a fraction to get 100% accuracy
    //pTicksPerSecond = pTPQN * pTPQN_BeatsPerMinute  / 60
    int pTicksPerSecond;

    //fractional ticks
    //pTicksPerSecondError = pTPQN * pTPQN_BeatsPerMinute mod 60
    //When pTicksPerSecondError == 60, increase tick accumulator by 1
    int pTicksPerSecondError;
};



#endif //MIDITEST_MIDI_H






















