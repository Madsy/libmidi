#ifndef MIDIFILE_H_GUARD
#define MIDIFILE_H_GUARD

#include <string>
#include <fstream>
#include <memory>
#include <vector>

class Midi;
struct MidiChunkHeader;

class MidiFile {
public:
	MidiFile(const std::string& name);
	~MidiFile();
    std::shared_ptr<Midi> data();
protected:
	void load(const std::string& name);
	void readChunkInfo();
    void readHeader();
    void readTracks();
private:
	std::ifstream pStrm;
	std::vector<std::unique_ptr<MidiChunkHeader>> pTrackChunkInfo;
	std::unique_ptr<MidiChunkHeader> pHeaderChunkInfo;
    std::shared_ptr<Midi> pMidiData;
};

#endif