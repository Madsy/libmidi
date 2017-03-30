//
// Created by madsy on 25.03.17.
//

#include "synth.h"

int g_samplerate = 44100;
float tone_table[NUM_TONES];
const char* note_names[12] = {
        "C", "C#", "D", "D#", "E,", "F", "F#", "G", "G#", "A", "b", "H"
};


std::unique_ptr<MidiFile> f;
std::vector<short> audio_data;

void disable_voice_with_key(int channel, int key);

void generate_tone_table(){
    /* Need offset because 2^(0/12) is A-4, so index 0 is A-4 */
    int octave_offset = (MIDDLE_OCTAVE - FIRST_OCTAVE)*HALFNOTES_PER_OCTAVE;
    for(int i = 0; i < NUM_TONES; i++){
        //add 3 to start from C-4 instead of A-4
        int idx = (i - octave_offset - 21);
        tone_table[i] = 440.0f * std::pow(2.0f,  (float)idx/12.0f);
    }
}

/* Duty cycle is a ratio from 0 to 1, controlling the high/low cycle duration ratio */
float square_wave(float frequency, float duty_cycle, float t){
    float dummy;
    float sample = 0.0f;
    /* 'wavelength' seconds is the duration of a single cycle */
    float wavelength = 1.0f/ frequency;
    /* remainder after N wavelengths */
    float t_frac = std::modf(t*frequency, &dummy);
    if(t_frac < duty_cycle) sample = 1.0f;
    else sample = -1.0f;

    return sample;
}

float square_wave_fine(float frequency, float t){
    const int num_iter = 20;
    float result = 0.0f;
    result = std::sin(2.0f * M_PI * frequency * t);
    for(int i = 1; i < num_iter; i+=2){
        float fi = (float)i;
        result += ((1.0f / (fi+1.0f)) * std::sin(2.0f * M_PI * frequency * t * fi));
    }
    return result;
}

class Voice {
public:
    Voice(int channel) : pChannel(channel), pKey(0), pVolume(0), pTimeCounter(0), pEnabled(false), pNotePressed(false){
        pAttack = 0.1f;
        pDecay = 0.0f;
        pSustain = 0.0f; //dB
        pRelease = 0.15f;
        tDebug = 0.0f;
        pReleaseTimeCounter = 0;
    }
    void Pressed(int key, int volume){
        pKey = key;
        pTimeCounter = 0;
        pReleaseTimeCounter = 0;
        pVolume = volume;
        pEnabled = true;
        pNotePressed = true;
    }
    void Released(){
        pReleaseTimeCounter = pTimeCounter;
        pNotePressed = false;
    }
    void Off(){
        pKey = 0;
        pVolume = 0;
        pTimeCounter = 0;
        pReleaseTimeCounter = 0;
        pVolume = 0;
        pEnabled = false;
        pNotePressed = false;
        tDebug = 0.0f;
    }
    bool isOn() const { return pEnabled; }
    bool isPressed() const { return pNotePressed; }
    int getKey() const { return pKey; }
    short getSample(){
        float t = (float)pTimeCounter / (float)g_samplerate;
        float t2 = (float)(pTimeCounter - pReleaseTimeCounter) / (float)g_samplerate;
        tDebug = t2;
        //float sample = square_wave(tone_table[pKey], 0.6f, t);
        float sample = square_wave_fine(tone_table[pKey], t);
        pTimeCounter++;
        sample = sample * ADSR(pAttack, pDecay, pSustain, pRelease, t2, pNotePressed);
        sample = 32767.0f * sample * (float)pVolume / 255.0f;
        /* disable this voice after release stage */
        if(!pNotePressed && (t2 >= pRelease)){
            disable_voice_with_key(pChannel, pKey);
        }
        return sample;
    }
private:
    int pChannel;
    int pKey; //Key with values following the MIDI specification. 60 is a C-5
    int pVolume; //Key pressure. Used as volume currently, but that's not entirely correct. Pressure is how fast the key is pressed
    int pTimeCounter; //Time accumulator measured in samples
    int pReleaseTimeCounter;
    float pAttack;
    float pDecay;
    float pSustain;
    float pRelease;
    float tDebug;
    bool pEnabled;
    bool pNotePressed; //Pressed or released. State for ADSR
};

unsigned int g_num_active_voices[NUM_CHANNELS] = {0};
std::vector<std::vector<Voice>> g_voices;


static void init_voices(){
    for(int i = 0; i < NUM_CHANNELS; i++){
        std::vector<Voice> voices;
        for(int j = 0; j < NUM_VOICES_PER_CHANNEL; j++){
            voices.push_back(Voice(i));
        }
        g_voices.push_back(voices);
    }
}

/* Find a new unused voice in our Voice pool */
Voice& find_unused_voice(int channel){
    /*
    for(int i = 0; i < NUM_VOICES_PER_CHANNEL; i++){
        if(!g_voices[channel][i].isOn()){
            return g_voices[channel][i];
        }
    }
     */
    if(g_num_active_voices[channel] == NUM_VOICES_PER_CHANNEL){
        throw std::runtime_error("Out of free channels!");
    }
    g_num_active_voices[channel]++;
    return g_voices[channel][g_num_active_voices[channel] - 1];
}

/* Find an unused voice and start a new one by initialising the state */
void enable_new_voice(int channel, int key, int volume){
    Voice& v = find_unused_voice(channel);
    v.Pressed(key, volume);
}

/* For a NoteOff event, find the voice with the corresponding channel and key from the NoteOn event.
 * There should be one NoteOn event for every NoteOff event and vice-versa
 * This does not disable (cut off) the voice, but rather starts the ADSR release */

void turnNoteOff(int channel, int key){
    for(int i = 0; i < NUM_VOICES_PER_CHANNEL; i++) {
        if (g_voices[channel][i].isOn() && g_voices[channel][i].isPressed() && g_voices[channel][i].getKey() == key) {
            g_voices[channel][i].Released();
            break;
        }
    }
}

/* Called by the Voice class when a voice is done with its release. Sends the voice back to the pool */

void disable_voice_with_key(int channel, int key){
    for(int i = 0; i < NUM_VOICES_PER_CHANNEL; i++){
        if(g_voices[channel][i].isOn() && !g_voices[channel][i].isPressed() && g_voices[channel][i].getKey() == key) {
            g_voices[channel][i].Off();
            int index_of_last_active_voice = g_num_active_voices[channel] - 1;
            if (index_of_last_active_voice > 0 && (index_of_last_active_voice != i)) {
                Voice &voice_to_disable = g_voices[channel][i];
                Voice &last_used = g_voices[channel][index_of_last_active_voice];
                std::swap(voice_to_disable, last_used);
            }
            g_num_active_voices[channel]--;
            return;
        }
    }
   // throw std::runtime_error("Wanted to turn off voice with key, but key not found!");
}

/* For all channels and voices, compute and mix a single sample and step one sample forward */
short advance_voices(){
    short sample = 0;
    for(int channel = 0; channel < NUM_CHANNELS; channel++){
        for(int voice = 0; voice < NUM_VOICES_PER_CHANNEL; voice++){
            if(g_voices[channel][voice].isOn()){
                //if(channel == 10) continue;
                sample += (g_voices[channel][voice].getSample() * (1.0f / 8.0f));
            }
        }
    }
    return sample;
}


/* Entrypoint for applying incoming events and apply them to the Voice polyphony state */
void handle_voice_event(std::shared_ptr<MidiEvent> evt){
    unsigned int channel = evt->getChannelIndex();
    auto noteOn = std::dynamic_pointer_cast<ChEvtNoteOn>(evt);
    if(noteOn){
        if(noteOn->getVelocity() != 0) {
            enable_new_voice(noteOn->getChannelIndex(), noteOn->getKey(), noteOn->getVelocity());
        } else {
            //This is really a note off
            disable_voice_with_key(noteOn->getChannelIndex(), noteOn->getKey());
        }
        return;
    }
    auto noteOff = std::dynamic_pointer_cast<ChEvtNoteOff>(evt);
    if(noteOff){
        //disable_voice_with_key(noteOff->getChannelIndex(), noteOff->getKey());
        turnNoteOff(noteOff->getChannelIndex(), noteOff->getKey());
        return;
    }
}

/* Filter out all events that are not KeyOn/KeyOff events.
 * The proper way to handle this is to use a visitor class instead of dynamic_pointer_cast */
Midi::MidiEventArray filterAllEvents(std::shared_ptr<Midi> midi){
    Midi::MidiEventArray src = midi->getAllEvents();
    Midi::MidiEventArray dst;

    for(size_t i = 0; i < src.size(); i++){
        auto noteOn = std::dynamic_pointer_cast<ChEvtNoteOn>(src[i]);
        if(noteOn){
            dst.push_back(src[i]);
            continue;
        }
        auto noteOff = std::dynamic_pointer_cast<ChEvtNoteOff>(src[i]);
        if(noteOff){
            dst.push_back(src[i]);
            continue;
        }
    }
    return dst;
}

void generate_song(){
    std::shared_ptr<Midi> midi = f->data();
    Midi::MidiEventArray evts = filterAllEvents(midi);
    unsigned int tempo = midi->getTicksPerSecond();
    unsigned int samplespertick = g_samplerate / tempo;
    unsigned int samplepos = 0;
    unsigned int tickStart = 0;
    unsigned int statusCounter = 0;
    printf("Generating song..\n");
    printf("Event array size: %zu\n", evts.size());
    size_t i = 0;

    init_voices();

    while(i < evts.size()){
        if((i - statusCounter) > 500){
            printf("Index %zu: %zu%% done\n", i, i*100/evts.size());
            statusCounter = i;
        }
        unsigned int tickEnd = evts[i]->getTimestamp();

        //fill time interval with data from current voice state
        unsigned int tickDelta = tickEnd - tickStart;
        unsigned int numSamples = tickDelta * samplespertick;

        for(unsigned int j = 0; j < numSamples; j++){
            short sample = advance_voices();
            audio_data.push_back(sample);
        }


        //change voice state
        while((i < evts.size()) && (evts[i]->getTimestamp() == tickEnd)){
            handle_voice_event(evts[i]);
            i++;
            if((i - statusCounter) > 500){
                printf("Index %zu: %zu%% done\n", i, i*100/evts.size());
                statusCounter = i;
            }
        }
        tickStart = tickEnd;
    }
}