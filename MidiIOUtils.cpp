//
// Created by madsy on 19.03.17.
//

#include "MidiIOUtils.h"
#include <fstream>
#include <vector>
#include <exception>
#include <stdexcept>

void swap16(unsigned short& i){
    unsigned char b1 = i & 0xFF;
    unsigned char b2 = (i >> 8) & 0xFF;
    i = (b1<<8) | b2;
}

void swap32(unsigned int& i){
    unsigned char b1 = i & 0xFF;
    unsigned char b2 = (i >> 8) & 0xFF;
    unsigned char b3 = (i >> 16) & 0xFF;
    unsigned char b4 = (i >> 24) & 0xFF;
    i = (b1<<24) | (b2<<16) | (b3<<8) | b4;
}

unsigned long filesize(std::ifstream& strm){
    std::streampos oldpos = strm.tellg();
    strm.seekg (0, strm.end);
    std::streampos len = strm.tellg();
    //restore old position
    strm.seekg (oldpos, strm.beg);
    return (unsigned long)len;
}

//read variable length integers, such as delta times
size_t readMidiVarLenInteger(unsigned int& out, std::ifstream& strm){
    unsigned char b = 0;
    std::vector<unsigned char> data;
    //read until we find a byte with MSB = 0 which marks the end of the integer
    do {
        strm.read((char*)&b, 1);
        //Before pushing the byte, clear MSB
        data.push_back(b & ~0x80u);
    } while(b & 0x80);
    //Can't have more than 28 bits!
    size_t len = data.size();

    switch(len){
        case 1:
            //7 bit integer (up to 0x7F)
            out = data[0];
            break;
        case 2:
            //14 bit integer (up to 0x3FFF)
            out = (data[0]<<7u)|(data[1]);
            break;
        case 3:
            //21 bit integer (up to 0x1FFFFF)
            out = (data[0]<<14u)|(data[1]<<7u)|(data[2]);
            break;
        case 4:
            //28 bit integer (up to 0x0FFFFFFF)
            out = (data[0]<<21u) | (data[1]<<14u)|(data[2]<<7u)|(data[3]);
            break;
        default:
            throw std::runtime_error("Varlen integer greater than 0x0FFFFFFF!");
    }
    //return the number of bytes we consumed from the stream
    return len;
}