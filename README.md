## libmidi

####This is a work in progress.

---
A simple library for reading .mid files (General Midi) and handling MIDI data from masters.
Custom instruments can be implemented as a software synthesizer.
Can make handlers for custom MIDI sequencer messages.

The overarching idea is to make a library which allows a user to easily create software synthesizers which can source
MIDI events from both trackers/sequencers as well as MIDI files; both for composing music and when creating new synths
and instruments.


#### Dependencies

+ libSDL (for the player example, main.cpp)
+ libsndfile

#### What is already done?

+ Reading the chunk info
+ Reading the MIDI header
+ *Parsing* all of all the events below (but not applying their *effects*)
+ Applying NoteOff/NoteOn events
+ Extracting the meta data such as song name and patch numbers
+ A simple player example which can play .mid files with format 0 (format 1 and 2 support is coming)

#### What is the roadmap?

+ Fully implement all the known MIDI events:
    * Channel Messages:
        * Polyphonic Key Pressure Event (Aftertouch).
        * Control Change Event
        * Program Change Event
        * Channel Pressure (After-touch) Event
        * Pitch Wheel Change Event
    * Meta Messages (For General Midi only):
        * Sequence Number Event
        * Text Event
        * Copyright Notice Event
        * Sequence/Track Name Event
        * Instrument Name Event
        * Lyric Event
        * Marker Event
        * Cue Point Event
        * MIDI Channel Prefix Event
        * End of Track Event
        * Set Tempo Event
        * SMTPE Offset Event
        * Time Signature Event
        * Key Signature Event
        * Sequencer-Specific Meta-Event
    * System Common Messages:
        * System Exclusive Event
        * Song Position Pointer Event
        * Song Select Event
        * Tune RequestEvent
        * End of Exclusive Event
    * System Real-Time Messages:
        * Timing Clock Event
        * Start Event
        * Continue Event
        * Stop Event
        * Active Sensing Event
        * Reset Event 
     
+ Recognize instruments from general MIDI
+ Convert timestamps into a consistent format no matter which convention the file uses. Should be either ticks or milliseconds.
+ Support real time MIDI streams in addition to .mid files and general MIDI.
+ Make the API easy to use with software synthesizers
+ Allow function callbacks into unassigned MIDI events so they can be used for custom code
