//
// Created by madsy on 20.03.17.
//

#include "MetaEvent.h"

MetaEvtSequenceNumber::MetaEvtSequenceNumber(unsigned int timestamp, unsigned int sequenceNum) :
        MidiEvent(timestamp), pSequenceNumber(sequenceNum){

}
MetaEvtText::MetaEvtText(unsigned int timestamp, const std::string &text) :
        MidiEvent(timestamp), pText(text){

}
MetaEvtCopyright::MetaEvtCopyright(unsigned int timestamp, const std::string &copyright) :
        MidiEvent(timestamp), pCopyright(copyright) {

}
MetaEvtTrackName::MetaEvtTrackName(unsigned int timestamp, const std::string &trackName) :
        MidiEvent(timestamp), pTrackName(trackName) {

}
MetaEvtInstrumentName::MetaEvtInstrumentName(unsigned int timestamp, const std::string &instrumentName) :
        MidiEvent(timestamp), pInstrumentName(instrumentName) {

}
MetaEvtLyrics::MetaEvtLyrics(unsigned int timestamp, const std::string &lyrics) :
        MidiEvent(timestamp), pLyrics(lyrics) {

}
MetaEvtMarker::MetaEvtMarker(unsigned int timestamp, const std::string &marker) :
        MidiEvent(timestamp), pMarker(marker) {

}
MetaEvtMidiChannelPrefix::MetaEvtMidiChannelPrefix(unsigned int timestamp, unsigned int channel) :
        MidiEvent(timestamp), pChannel(channel) {

}

MetaEvtEndOfTrack::MetaEvtEndOfTrack(unsigned int timestamp) :
        MidiEvent(timestamp) {

}

MetaEvtSetTempo::MetaEvtSetTempo(unsigned int timestamp, unsigned int tempo) :
        MidiEvent(timestamp), pTempo(tempo){

}
MetaEvtCuePoint::MetaEvtCuePoint(unsigned int timestamp, const std::string &cue) :
        MidiEvent(timestamp), pCuePoint(cue) {

}
MetaEvtSMPTEOffset::MetaEvtSMPTEOffset(
        unsigned int timestamp,
        unsigned int hours,
        unsigned int minutes,
        unsigned int seconds,
        unsigned int frames,
        unsigned int frame_fraction
) :
        MidiEvent(timestamp),
        pHours(hours),
        pMinutes(minutes),
        pSeconds(seconds),
        pFrames(frames),
        pFractionFrames(frame_fraction)
{

}
MetaEvtTimeSignature::MetaEvtTimeSignature(
        unsigned int timestamp,
        unsigned int numerator,
        unsigned int denominator,
        unsigned int clocks_per_tick,
        unsigned int notes_per_clock
) :
        MidiEvent(timestamp),
        pNumerator(numerator),
        pDenominator(denominator),
        pClocksPerTick(clocks_per_tick),
        pNotesPerClock(notes_per_clock){

}
MetaEvtKeySignature::MetaEvtKeySignature(unsigned int timestamp, unsigned int sharpflats, unsigned int minormajor) :
        MidiEvent(timestamp), pSharpFlats(sharpflats), pMinorMajor(minormajor){

}
MetaEvtSequencerSpecific::MetaEvtSequencerSpecific(
        unsigned int timestamp,
        const std::vector<unsigned char> &data) :
        MidiEvent(timestamp), pData(data){
}









void MetaEvtSequenceNumber::print() {
    printf("Sequence Number\n");
}

void MetaEvtText::print() {
    printf("Text: \"%s\"\n", this->getText().c_str());
}

void MetaEvtCopyright::print() {
    printf("Copyright: %s\n", this->getCopyRight().c_str());
}

void MetaEvtTrackName::print() {
    printf("Track Name: %s\n", this->getTrackName().c_str());
}

void MetaEvtInstrumentName::print() {
    printf("Instrument Name: %s\n", this->getInstrumentName().c_str());
}

void MetaEvtLyrics::print() {
    printf("Lyrics: %s\n", this->getLyrics().c_str());
}

void MetaEvtMarker::print() {
    printf("Marker: %s\n", this->getMarker().c_str());
}
void MetaEvtCuePoint::print() {
    printf("Cue Point: %s\n", this->getCuePoint().c_str());
}

void MetaEvtMidiChannelPrefix::print() {
    printf("Channel Prefix: %u\n", this->getChannel());
}

void MetaEvtEndOfTrack::print() {
    printf("End Of Track\n");
}

void MetaEvtSetTempo::print() {
    printf("New Tempo: %u\n", this->getTempo());
}

void MetaEvtSMPTEOffset::print() {
    printf("SMPTEOffset: %u:%u:%u:%u:%u\n", this->getHours(), this->getMinutes(), this->getSeconds(), this->getFrames(), this->getSubFrames());
}

void MetaEvtTimeSignature::print() {
    printf("Time Signature: %u/%u, CLK/Tick %u, Notes/Clock %u\n", this->getNumerator(), this->getDenominator(), this->getClocksPerTick(),
           this->getNotesPerClock());
}

void MetaEvtSequencerSpecific::print() {
    printf("Sequencer Specific Info: ");
    const std::vector<unsigned char> d = this->getData();
    if(d.size()) printf("%02X", d[0]);
    for(size_t i = 1; i <d.size(); i++){
        printf(", %02X", d[i]);
    }
    printf("\n");
}

void MetaEvtKeySignature::print() {
    MidiEvent::print();
}











unsigned int MetaEvtSequenceNumber::getSequenceNumber() const {
    return pSequenceNumber;
}

const std::string &MetaEvtText::getText() const {
    return pText;
}

const std::string &MetaEvtCopyright::getCopyRight() const {
    return pCopyright;
}

const std::string &MetaEvtTrackName::getTrackName() const {
    return pTrackName;
}

const std::string &MetaEvtInstrumentName::getInstrumentName() const {
    return pInstrumentName;
}

const std::string &MetaEvtLyrics::getLyrics() const {
    return pLyrics;
}

const std::string &MetaEvtMarker::getMarker() const {
    return pMarker;
}

const std::string &MetaEvtCuePoint::getCuePoint() const {
    return pCuePoint;
}

unsigned int MetaEvtMidiChannelPrefix::getChannel() const {
    return pChannel;
}

unsigned int MetaEvtSetTempo::getTempo() const {
    return pTempo;
}

unsigned int MetaEvtSMPTEOffset::getHours() const {
    return pHours;
}

unsigned int MetaEvtSMPTEOffset::getMinutes() const {
    return pMinutes;
}

unsigned int MetaEvtSMPTEOffset::getSeconds() const {
    return pSeconds;
}

unsigned int MetaEvtSMPTEOffset::getFrames() const {
    return pFrames;
}

unsigned int MetaEvtSMPTEOffset::getSubFrames() const {
    return pFractionFrames;
}

unsigned int MetaEvtTimeSignature::getNumerator() const {
    return pNumerator;
}

unsigned int MetaEvtTimeSignature::getDenominator() const {
    return pDenominator;
}

unsigned int MetaEvtTimeSignature::getClocksPerTick() const {
    return pClocksPerTick;
}

unsigned int MetaEvtTimeSignature::getNotesPerClock() const {
    return pNotesPerClock;
}

unsigned int MetaEvtKeySignature::getSharpFlats() const {
    return pSharpFlats;
}

unsigned int MetaEvtKeySignature::getMinorMajor() const {
    return pMinorMajor;
}

unsigned int MetaEvtSequencerSpecific::getManufacturerID() const {
    /* Not implemented yet, because it's not obvious how to parse the manufacturer field. It can be 1 or 3 bytes
     * The length field of a sequencer specific event is the length of the manufacturer id and the data combined,
     * so the id is in the data array */
    return 0;
}

const std::vector<unsigned char> &MetaEvtSequencerSpecific::getData() const {
    return pData;
}



