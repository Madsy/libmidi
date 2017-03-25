#include <SDL/SDL.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <sndfile.h>
#include "MidiFile.h"
#include "Midi.h"
#include "ChannelEvent.h"

#define PLAYER


int g_samplerate = 44100;

struct stereo_sample_interleaved {
    short left, right;
};

std::unique_ptr<MidiFile> f;

const int MIDI_START_NOTE = 0x90;
const int MIDI_STOP_NOTE = 0x80;
const int MIDI_PATCH_CHANGE = 0xC0;

const int HALFNOTES_PER_OCTAVE = 12;
const int NUM_OCTAVES = 9;
const int FIRST_OCTAVE = 0;
const int NUM_TONES = HALFNOTES_PER_OCTAVE * NUM_OCTAVES;
/* only used as A-4 reference */
const int MIDDLE_OCTAVE = 4;

float tone_table[NUM_TONES];

const char* note_names[12] = {
        "C", "C#", "D", "D#", "E,", "F", "F#", "G", "G#", "A", "b", "H"
};

static void generate_tone_table(){
    /* Need offset because 2^(0/12) is A-4, so index 0 is A-4 */
    int octave_offset = (MIDDLE_OCTAVE - FIRST_OCTAVE)*HALFNOTES_PER_OCTAVE;
    for(int i = 0; i < NUM_TONES; i++){
        //add 3 to start from C-4 instead of A-4
        int idx = (i - octave_offset - 21);
        tone_table[i] = 440.0f * std::pow(2.0f,  (float)idx/12.0f);
    }
}

/* Duty cycle is a ratio from 0 to 1, controlling the high/low cycle duration ratio */
static float square_wave(float frequency, float duty_cycle, float t){
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

class Voice {
public:
    Voice() : pKey(0), pVolume(0), pTimeCounter(0), pEnabled(false){}
    void On(int key, int volume){
        pKey = key;
        pTimeCounter = 0;
        pVolume = volume;
        pEnabled = true;
    }
    void Off(){
        pKey = 0;
        pTimeCounter = 0;
        pVolume = 0;
        pEnabled = false;
    }
    bool isOn() const { return pEnabled; }
    int getKey() const { return pKey; }
    short getSample(){
        float t = (float)pTimeCounter / (float)g_samplerate;
        float sample = square_wave(tone_table[pKey], 0.6f, t);
        pTimeCounter++;
        return 32767.0f * sample * (float)pVolume / 255.0f;
    }
private:
    int pKey; //Key with values following the MIDI specification. 60 is a C-5
    int pVolume; //Key pressure. Used as volume currently, but that's not entirely correct. Pressure is how fast the key is pressed
    int pTimeCounter; //Time accumulator measured in samples
    bool pEnabled;
};

const unsigned int NUM_CHANNELS = 17;
const unsigned int NUM_VOICES_PER_CHANNEL = 9;

Voice g_voices[17][NUM_VOICES_PER_CHANNEL];
void fill_audio(void *udata, Uint8 *stream, int len);

static int init_audio(){
    SDL_AudioSpec wanted;
    wanted.freq = g_samplerate;
    wanted.format = AUDIO_S16;
    wanted.channels = 2;    /* 1 = mono, 2 = stereo */
    wanted.samples = 8192;  /* Good low-latency value for callback */
    wanted.callback = fill_audio;
    wanted.userdata = NULL;

    /* Open the audio device, forcing the desired format */
    if ( SDL_OpenAudio(&wanted, NULL) < 0 ) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return(-1);
    }
    return(0);
}

bool done_playing = false;
unsigned int fill_audio_pos = 0;
std::vector<short> audio_data;

/* SDL Audio callback. Currently just playing our audio which is generated offline.
 * A proper player should probably parse the stream and mix in realtime together with a working thread */
void fill_audio(void *udata, Uint8 *stream, int len){

    int samplesize = 4; //16 -bit samples, stereo
    int num_samples = len / samplesize;
    stereo_sample_interleaved* sampleBuffer = (stereo_sample_interleaved*)stream;
    unsigned int remaining = audio_data.size() - fill_audio_pos;
    if(remaining >= num_samples){
        for(int i = 0; i < num_samples; i++){
            short sample = audio_data[fill_audio_pos + i];
            sampleBuffer[i].left = sample;
            sampleBuffer[i].right = sample;
        }
        fill_audio_pos += num_samples;
    } else {
        memset(stream, 0, len);
        done_playing = true;
    }
}

/* Find a new unused voice in our Voice pool */
Voice& find_unused_voice(int channel){
    for(int i = 0; i < NUM_VOICES_PER_CHANNEL; i++){
        if(!g_voices[channel][i].isOn()){
            return g_voices[channel][i];
        }
    }
    throw std::runtime_error("Out of free channels!");
}

/* Find an unused voice and start a new one by initialising the state */
void enable_new_voice(int channel, int key, int volume){
    Voice& v = find_unused_voice(channel);
    v.On(key, volume);
}

/* For a NoteOff event, find the voice with the corresponding channel and key from the NoteOn event.
 * There should be one NoteOn event for every NoteOff event and vice-versa */
void disable_voice_with_key(int channel, int key){
    for(int i = 0; i < NUM_VOICES_PER_CHANNEL; i++){
        if(g_voices[channel][i].isOn() && g_voices[channel][i].getKey() == key){
            g_voices[channel][i].Off();
            return;
        }
    }
    throw std::runtime_error("Wanted to turn off voice with key, but key not found!");
}

/* For all channels and voices, compute and mix a single sample and step one sample forward */
short advance_voices(){
    short sample = 0;
    for(int channel = 0; channel < NUM_CHANNELS; channel++){
        for(int voice = 0; voice < NUM_VOICES_PER_CHANNEL; voice++){
            if(g_voices[channel][voice].isOn()){
                if(channel == 10) continue;
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
        disable_voice_with_key(noteOff->getChannelIndex(), noteOff->getKey());
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

/* Dump the song as 16-bit Mono, 44100hz */
void dump(const std::string& name){
    SNDFILE	*file;
    SF_INFO	sfinfo;
    int		k ;

    memset (&sfinfo, 0, sizeof (sfinfo)) ;

    sfinfo.samplerate	= g_samplerate;
    sfinfo.frames		= audio_data.size();
    sfinfo.channels		= 1 ;
    sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_16);

    if (! (file = sf_open (name.c_str(), SFM_WRITE, &sfinfo))){
        throw std::runtime_error("Error : Not able to open output file.");
    }
    sf_writef_short (file, &audio_data[0], audio_data.size());
    sf_close (file) ;
}

void textdump(){
    std::shared_ptr<Midi> midi = f->data();
    Midi::MidiEventArray evts = midi->getAllEvents();
    unsigned int tempo = midi->getTicksPerSecond();

    size_t i = 0;

    while(i < evts.size()){
        unsigned int tick = evts[i]->getTimestamp();
        unsigned int channel = evts[i]->getChannelIndex();
        float t = (float)tick / (float)tempo;
        printf("Time %f, tick %u, index %zu, channel %u: ", t, tick, i, channel);
        evts[i]->print();
        i++;
    }
}

void usage(const std::string& program_name){
    printf("Usage: %s <midi file> <output .wav file>\n", program_name.c_str());
}

int main(int argc, char* argv[]) {
    if(argc != 3){
        usage(argv[0]);
        return 0;
    }

    try {
        f = std::move(std::unique_ptr<MidiFile>(new MidiFile(argv[1])));
    } catch(const std::exception& ex){
        printf("Error when parsing midi file: %s\n", ex.what());
        return 1;
    }

    if(!f) return 1;

    std::shared_ptr<Midi> midi = f->data();

    // Generate tempered scale frequencies
    generate_tone_table();
    generate_song();
    textdump();
    dump(argv[2]);
#ifdef PLAYER
    SDL_Init(SDL_INIT_AUDIO);
    if(init_audio()){
        SDL_Quit();
        return 1;
    }
    printf("Running..\n");
    SDL_PauseAudio(0);
    while(!done_playing){}
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    SDL_Quit();
#endif

    return 0;
}
