#include <SDL/SDL.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <utility>
#include <sndfile.h>
#include "MidiFile.h"
#include "Midi.h"
#include "MidiDevice.h"
#include "ChannelEvent.h"
#include "synth/synth.h"

#define PLAYER


static bool done_playing = false;
static unsigned int fill_audio_pos = 0;


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

#if 0
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

#else
    MidiDevice dev;
    try {
        dev.open("default");
        while(1){
            std::shared_ptr<MidiEvent> ev = dev.read();
        }
    } catch(std::exception& ex){
        printf("Exception in main: %s\n", ex.what());
    }

#endif
    return 0;
}
