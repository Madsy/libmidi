## libmidi

A simple library for reading .mid files. Work in progress.

#### What is already done?

+ Reading the chunk info
+ Reading the MIDI header
+ Parsing all the channel-based MIDI event types (note-off, note-on, change patch ..)

#### What is the roadmap?

+ Implement all the known MIDI events:
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
      * Song Position Pointer.
      * Song Select.
      * Tune Request.
      * End of Exclusive.
  * System Real-Time Messages:
     * Timing Clock.
     * Start.
     * Continue.
     * Stop.
     * Active Sensing.
     * Reset.
     
+ Recognize instruments from general MIDI
+ Convert timestamps into a consistent format no matter which convention the file uses. Should be either ticks or milisecons.
+ Support realtime MIDI streams in addition to .mid files and general MIDI.
+ Make the API easy to use with software synthesizers
+ Allow function callbacks into unassigned MIDI events so they can be used for custom code
