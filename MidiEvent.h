//
// Created by madsy on 18.03.17.
//

#ifndef MIDITEST_MIDIEVENT_H
#define MIDITEST_MIDIEVENT_H

//Meta event: FF <type> <length> <bytes>
//Sysex event1: F0 <len> <data>
//Sysex event2: F7 <len> <data>

//Channel (voice/mode) message: 0x8x to Ex
//System Common message: 0xF0 to 0xF7
//System Real-Time Message: 0xF8 to 0xFF

/*
 * Channel Message
 *      -> Channel Voice Message
 *      -> Channel Mode message
 * System Message
 *      -> System Common Message
 *      -> System Real Time Message
 *      -> System Exclusive Message
 */

/*
 * Channel events:
     * Note Off event.
     * Note On event.
     * Polyphonic Key Pressure (Aftertouch).
     * Control Change.
     * Program Change.
     * Channel Pressure (After-touch).
     * Pitch Wheel Change.

  * System Common Messages:
      * System Exclusive
      * Undefined.
      * Song Position Pointer.
      * Song Select.
      * Undefined.
      * Undefined.
      * Tune Request.
      * End of Exclusive.

 * System Real-Time Messages:
     * Timing Clock.
     * Undefined.
     * Start.
     * Continue.
     * Stop.
     * Undefined.
     * Active Sensing.
 */

#include <memory>

/*
 * MIDI Controller messages (Control change types) :
 *
 */

class MidiEvent {
public:
    enum class MidiEventType {
        EVT_RUNNING_STATUS,
        EVT_CHANNEL,
        EVT_SYSCOMMON,
        EVT_SYSRT,
        EVT_META //0xFF is used for RESET in a MIDI stream, but marks the start of a meta event in MIDI files
    };

    enum class ChannelEventType {
        EVT_NOTE_OFF        = 0x0,
        EVT_NOTE_ON         = 0x1,
        EVT_AFTERTOUCH1     = 0x2, /* Polyphonic Key Pressure */
        EVT_CONTROL_CHANGE  = 0x3,
        EVT_PROGRAM_CHANGE  = 0x4,
        EVT_AFTERTOUCH2     = 0x5, /* Channel pressure */
        EVT_PITCH_WHEEL     = 0x6
    };

    enum class SystemCommonEventType {
        EVT_SYSTEM_EXCLUSIVE        = 0x0,
        /* UNDEFINED = 0x1 */
        EVT_SONG_POSITION_POINTER   = 0x2,
        EVT_SONG_SELECT             = 0x3,
        /* UNDEFINED = 0x4 */
        /* UNDEFINED = 0x5 */
        EVT_TUNE_REQUEST            = 0x6,
        EVT_END_OF_EXCLUSIVE        = 0x7
    };

    enum class SystemRealtimeEventType {
        EVT_TIMING_CLOCK            = 0x8,
        /* UNDEFINED = 0x9 */
        EVT_START                   = 0xA,
        EVT_CONTINUE                = 0xB,
        EVT_STOP                    = 0xC,
        /* UNDEFINED = 0xD */
        EVT_ACTIVE_SENSING          = 0xE
    };

    enum class MetaEventType {
        EVT_SEQUENCE_NUMBER = 0xFF0002,
        EVT_TEXT = 0x01,
        EVT_COPYRIGHT = 0x02,
        EVT_TRACK_NAME = 0x03,
        EVT_INSTRUMENT_NAME = 0x04,
        EVT_LYRICS = 0x5,
        EVT_MARKER = 0x6,
        EVT_CUE_POINT = 0x7,
        EVT_MIDI_CHANNEL_PREFIX = 0xFF2001,
        EVT_END_OF_TRACK = 0xFF2F00,
        EVT_SET_TEMPO = 0xFF5103,
        EVT_SMPTE_OFFSET = 0xFF5405,
        EVT_TIME_SIGNATURE = 0xFF5804,
        EVT_KEY_SIGNATURE = 0xFF5902,
        EVT_SEQUENCER_SPECIFIC = 0xFF7F
    };

    MidiEvent(unsigned int timestamp);
    unsigned int getChannelIndex() const;
    //unsigned int getChannelIndex() const;
    /* High level message group */
    static MidiEvent::MidiEventType getEventType(unsigned char event);
    /* Classify the sub-type of the events */
    static MidiEvent::ChannelEventType getChannelEventType(unsigned char event);
    static MidiEvent::SystemCommonEventType getSystemCommonEventType(unsigned char event);
    static MidiEvent::SystemRealtimeEventType getSystemRealtimeEventType(unsigned char event);
    //Special case. Meta event types are not fixed in length. Some codes are up to 3 bytes.
    static MidiEvent::MetaEventType getMetaEventType(unsigned int& bytesread, std::ifstream& strm);
    MidiEvent();
    virtual unsigned int getTimestamp() const;
    virtual void print();
protected:
    virtual unsigned int pGetChannelIndex() const; //returns channel 0 for all non-channel events
private:
    unsigned int pTick; //used for timestamps
};

#endif //MIDITEST_MIDIEVENT_H
