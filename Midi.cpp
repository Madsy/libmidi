//
// Created by madsy on 18.03.17.
//

#include "Midi.h"

Midi::Midi() {

}

Midi::Midi(const Midi &m) {

}

Midi::MidiFileFormat Midi::getFileFormat() const {
    return pMidiFileFormat;
}

Midi::MidiTimeFormat Midi::getTimeFormat() const {
    return pMidiTimeFormat;
}

size_t Midi::getTrackCount() const {
    return pTracks.size();
}

Midi::~Midi() {

}

unsigned int Midi::getTicksPerSecond() const {
    return pTicksPerSecond;
}

unsigned int Midi::getTicksPerSecondError() const {
    return pTicksPerSecondError;
}

const Midi::MidiTrack &Midi::getTrack(unsigned int index) const {
    return pTracks[index];
}

const Midi::MidiEventArray &Midi::getAllEvents() const {
    return pAllEvents;
}

