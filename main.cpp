#include "MidiFile.h"

int main(int argc, char* argv[]) {
    if(argc != 2) return 0;
    MidiFile f(argv[1]);
    return 0;
}
