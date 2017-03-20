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
     * Reset.
 */

#include <memory>

/*
 * MIDI Controller messages (Control change types) :
 *
 */

class MidiEvent {
public:
    enum class MidiEventType {
        EVT_CHANNEL,
        EVT_SYSCOMMON,
        EVT_SYSRT
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
        EVT_ACTIVE_SENSING          = 0xE,
        EVT_RESET                   = 0xF
    };

    MidiEvent(unsigned int timestamp);
    /* High level message group */
    static MidiEvent::MidiEventType getEventType(unsigned char event);
    /* Classify the sub-type of the events */
    static MidiEvent::ChannelEventType getChannelEventType(unsigned char event);
    static MidiEvent::SystemCommonEventType getSystemCommonEventType(unsigned char event);
    static MidiEvent::SystemRealtimeEventType getSystemRealtimeEventtype(unsigned char event);

    MidiEvent();
    virtual float getTimestamp() const;
    virtual void print();
private:
    unsigned int pTick; //used for timestamps
};

#endif //MIDITEST_MIDIEVENT_H
