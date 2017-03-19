//
// Created by madsy on 19.03.17.
//

#ifndef MIDITEST_MIDIIOUTILS_H
#define MIDITEST_MIDIIOUTILS_H

#include <iosfwd>

void swap16(unsigned short& i);
void swap32(unsigned int& i);
unsigned long filesize(std::ifstream& strm);
size_t readMidiVarLenInteger(unsigned int& out, std::ifstream& strm);

#endif //MIDITEST_MIDIIOUTILS_H
