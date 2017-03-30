//
// Created by madsy on 25.03.17.
//

#ifndef MIDITEST_SYNTH_H
#define MIDITEST_SYNTH_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include "../MidiFile.h"
#include "../Midi.h"
#include "../ChannelEvent.h"

extern int g_samplerate;

struct stereo_sample_interleaved {
    short left, right;
};

const int MIDI_START_NOTE = 0x90;
const int MIDI_STOP_NOTE = 0x80;
const int MIDI_PATCH_CHANGE = 0xC0;

const int HALFNOTES_PER_OCTAVE = 12;
const int NUM_OCTAVES = 9;
const int FIRST_OCTAVE = 0;
const int NUM_TONES = HALFNOTES_PER_OCTAVE * NUM_OCTAVES;
/* only used as A-4 reference */
const int MIDDLE_OCTAVE = 4;
const unsigned int NUM_CHANNELS = 17;
const unsigned int NUM_VOICES_PER_CHANNEL = 32;

extern float tone_table[NUM_TONES];
extern const char* note_names[12];
extern std::unique_ptr<MidiFile> f;
extern std::vector<short> audio_data;

void generate_tone_table();
void generate_song();
float square_wave(float frequency, float duty_cycle, float t);
float ADSR(float a,float d,float s,float r, float t, bool isPressed);

#endif //MIDITEST_SYNTH_H
