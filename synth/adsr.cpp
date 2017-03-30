//
// Created by madsy on 25.03.17.
//
#include <cmath>
#include <algorithm>

float ADSR(float a,float d,float s,float r, float t, bool isPressed){
    //Attack: Time it takes to reach 0 dB from silence
    //Decay: Time it takes to reach sustain level from 0 dB
    //Sustain: The volume level in negative dB to keep when note is held down
    //Release: The time it takes to reach silence from sustain level

    //To convert sustain from decibels (dBFS) to amplitude:
    //dBFS = 20 * log10(amplitude)
    //amplitude = 10^(dB/20)
    //Assuming amplitude is normalized in range -1 to 1
    //Alternatively one can consider "1", "127", "32767" or whatever the highest sample value
    //to be the reference value, dBFS = 20 * log10(amplitude/max amplitude)

    s = std::pow(10.0f, (s/20.0f));

    if(isPressed){ //key is pressed
        //t is the time since the key was pressed
        if(t < a){
            //attack phase:
            return t/a;
        } else if(t < (a+d)){
            //decay phase:
            return 1.0f + (s-1.0f)*((t-a)/d);
        } else {
            //sustain phase
            return s;
        }
    } else { //key is released
        //t is the time since the key was released
        return s - s*(std::min(t,r)/r);
    }
}
