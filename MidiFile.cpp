#include <vector>
#include <cstdio>
#include <cstring>
#include "MidiIOUtils.h"
#include "MidiFile.h"
#include "MidiEvent.h"
#include "Midi.h"
#include "MidiEventFactory.h"


/*
<Track Chunk> = <chunk type><length><MTrk event>+
<MTrk event> = <delta-time><event>
<event> = <MIDI event> | <sysex event> | <meta-event>
<MIDI event> = <cmd and channel> <variable length args>
<sysex_event> = ?
<meta_event> = ?
*/

struct MidiChunkHeader {
    MidiChunkHeader() : type(0), length(0), fileoffs(0){}
    MidiChunkHeader(const MidiChunkHeader & h) : type(h.type), length(h.length), fileoffs(h.fileoffs){

    }
    MidiChunkHeader(unsigned int type, unsigned int length, unsigned long fileoffs) :
            type(type), length(length), fileoffs(fileoffs) {

    }
    ~MidiChunkHeader();
    unsigned int type;
    unsigned int length;
    unsigned long fileoffs;
};

MidiChunkHeader::~MidiChunkHeader() = default;

struct MidiHeader {
    unsigned short format;
    unsigned short ntrks;
    unsigned short division;
};

const unsigned int mThd = 0x4D546864;
const unsigned int mTrk = 0x4D54726B;



MidiFile::MidiFile(const std::string &name) : pStrm(name.c_str(), std::ios::binary){
    if(!pStrm.is_open()){
        throw std::runtime_error("File not found");
    }
    load(name);
}

void MidiFile::load(const std::string &name) {
    pMidiData = std::make_shared<Midi>();
    readChunkInfo();
    readHeader();
    readTracks();
}

const std::shared_ptr<Midi> MidiFile::data() const {
    return pMidiData;
}

void MidiFile::readChunkInfo() {
    unsigned long size = filesize(pStrm);
    unsigned long bytesread = 0;
    while(bytesread < size){
        MidiChunkHeader hdr;
        memset(&hdr, 0, sizeof(MidiChunkHeader));
        hdr.fileoffs = bytesread + 8;
        //the length of a midi chunk header is 8 bytes (two 32-bit integers).
        //The file offset field in MidiChunkHeader is implicit (not a part of the file)
        pStrm.read((char*)&hdr.type, 4);
        pStrm.read((char*)&hdr.length, 4);
        swap32(hdr.type);
        swap32(hdr.length);

        switch(hdr.type){
            case mThd:
                //pHeaderChunkInfo
                pHeaderChunkInfo = std::make_unique<MidiChunkHeader>(hdr);
                break;
            case mTrk:
                //pTrackChunkInfo
                pTrackChunkInfo.push_back(std::make_unique<MidiChunkHeader>(hdr));
                break;
            default:
                //skip
                break;
        }
        pStrm.seekg(hdr.length, pStrm.cur);
        //each chunk header takes 8 bytes and not taken into account by the 'length' field
        bytesread += hdr.length + 8;
    }
    //At this point the pHeaderChunkInfo struct is filled with information on where to find the MIDI file header,
    //and the pTrackChunkInfo struct array is filled with information on where to find the tracks.

    //rewind stream
    pStrm.seekg(0u, pStrm.beg);
}

void MidiFile::readHeader() {
    MidiHeader hdr;
    if(!pHeaderChunkInfo){
        throw std::runtime_error("MIDI file has no header!");
    }
    pStrm.seekg(pHeaderChunkInfo->fileoffs, pStrm.beg);
    pStrm.read((char*)&hdr.format, 2);
    pStrm.read((char*)&hdr.ntrks, 2);
    pStrm.read((char*)&hdr.division, 2);

    swap16(hdr.format);
    swap16(hdr.ntrks);
    swap16(hdr.division);

    switch(hdr.format){
        case 0:
            pMidiData->pMidiFileFormat = Midi::MidiFileFormat ::MIDI_FORMAT_0;
            break;
        case 1:
            pMidiData->pMidiFileFormat = Midi::MidiFileFormat ::MIDI_FORMAT_1;
            break;
        case 2:
            pMidiData->pMidiFileFormat = Midi::MidiFileFormat ::MIDI_FORMAT_2;
        default:
            throw std::runtime_error("Invalid MIDI format.");
    }

    if(hdr.division & 0x8000u){
        int negativeSMPTE = (hdr.division >> 8) & 0x7Fu;
        int ticksPerFrame = hdr.division & 0xFFu;
        if(negativeSMPTE >= 0){
            throw std::runtime_error("Invalid SMPTE frame value");
        }
        pMidiData->pMidiTimeFormat = Midi::MidiTimeFormat::MIDI_TIME_TYPE_SMPTE;
        pMidiData->pSMPTE_FramesPerSecond = -negativeSMPTE;
        pMidiData->pSMPTE_TicksPerFrame = ticksPerFrame;
        pMidiData->pTicksPerSecond = pMidiData->pSMPTE_TicksPerFrame * pMidiData->pSMPTE_FramesPerSecond;
        pMidiData->pTicksPerSecondError = 0;
    } else {
        pMidiData->pMidiTimeFormat = Midi::MidiTimeFormat::MIDI_TIME_TYPE_TPQN;
        pMidiData->pTPQN = hdr.division & 0x7FFFu; //ticks per beat
        pMidiData->pTPQN_BeatsPerMinute = 120; //default 120 bpm but can be changed by MIDI events
        pMidiData->pTicksPerSecond = pMidiData->pTPQN * pMidiData->pTPQN_BeatsPerMinute / 60;
        pMidiData->pTicksPerSecondError = (pMidiData->pTPQN * pMidiData->pTPQN_BeatsPerMinute) % 60;
    }

    //rewind stream
    pStrm.seekg(0u, pStrm.beg);
}

void MidiFile::readTracks() {
    if(!pTrackChunkInfo.size()){
        throw std::runtime_error("MIDI file has no tracks!");
    }

    int numTracks = (int)pTrackChunkInfo.size();
    printf("DEBUG: Number of tracks according to pTrackChunkInfo: %u\n", numTracks);
    printf("DEBUG: Number of tracks according to header: %zu\n", pMidiData->getTrackCount());

    for(int i = 0; i < numTracks; i++){
        Midi::MidiTrack track;
        pStrm.seekg(pTrackChunkInfo[i]->fileoffs, pStrm.beg);
        unsigned int bytesread = 0;
        while(bytesread < pTrackChunkInfo[i]->length){
            unsigned int len = 0;
            track.push_back(std::move(MidiEventFactory::createMidiEvent(len, pStrm)));
            bytesread += len;
        }
        pMidiData->pTracks.push_back(std::move(track));
    }
}

MidiFile::~MidiFile() {

}
