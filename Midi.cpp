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

