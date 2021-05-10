/*
  ==============================================================================

    SynthVoice.cpp
    Created: 10 May 2021 7:59:27pm
    Author:  jiten

  ==============================================================================
*/

#include "SynthVoice.h"

//CONSTRUCTOR
SynthVoice::SynthVoice() {
    active = false;
    freq = INITIAL_FREQ;
    phase = INITIAL_PHASE;
    activeTime = INITIAL_TIME;
    state = ADSRState::ATTACK;
}

/*
    GETTERS
*/

bool SynthVoice::isActive() {
    return active;
}

ADSRState SynthVoice::getState() {
    return state;
}

float SynthVoice::getPhase() {
    return phase;
}

float SynthVoice::getFreq() {
    return freq;
}

float SynthVoice::getActiveTime() {
    return activeTime;
}

/*
    SETTERS
*/

void SynthVoice::setFreq(float value) {
    freq = value;
}

void SynthVoice::activate() {
    active = true;
}

void SynthVoice::deactivate() {
    active = false;
}

void SynthVoice::setState(ADSRState newState) {
    state = newState;
}

/*
    FUNCTIONS
*/

float SynthVoice::computeCurrentOutputValue(float* oscGains, float* oscFreqRatio) {
    float output = 0.0;

    for (int i = 0; i < TOT_HARMONICS; i++) {
        output += computeHarmonicOutput(oscGains[i], oscFreqRatio[i]);
    }
    
    updatePhase();

    return output;
}

float SynthVoice::adjustPhase(float value) {

    if (value < M_PI*2) {
        return value;
    }

    return value - ((int)(value / M_PI / 2)) * (M_PI*2);
}

void SynthVoice::updatePhase() {
    phase += (float)(M_PI * 2. * (((double)freq / (double)SAMPLE_RATE)));

    if (phase >= M_PI * 2.) {
        phase -= M_PI * 2.;
    }
}

float SynthVoice::computeHarmonicOutput(float gain, float freqRatio) {
    return gain * (float)sin(((double)adjustPhase(phase * freqRatio)));
}