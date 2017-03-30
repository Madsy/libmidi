cmake_minimum_required(VERSION 3.6)
project(miditest)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        main.cpp
        MidiFile.cpp
        MidiFile.h
        Midi.cpp
        Midi.h
        MidiEvent.cpp
        MidiEvent.h
        ChannelEvent.cpp
        ChannelEvent.h
        MidiEventFactory.cpp
        MidiIOUtils.cpp
        MidiIOUtils.h
        MidiEventFactory.h SystemCommonEvent.cpp SystemCommonEvent.h SystemRealtimeEvent.cpp SystemRealtimeEvent.h MetaEvent.cpp MetaEvent.h MidiChannel.cpp MidiChannel.h synth/adsr.cpp synth/synth.h synth/synth.cpp MidiDevice.cpp MidiDevice.h)
add_executable(miditest ${SOURCE_FILES})
target_link_libraries(miditest "SDL" "sndfile" "asound")